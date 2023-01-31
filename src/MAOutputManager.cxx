#include <MAOutputManager.hxx>

namespace MATools
{
	namespace MAOutputManager
	{
		std::string build_name()
		{
			using namespace MATools::MPI;
			std::string base_name = "MATimers";
#ifdef __MPI
			int mpiSize = get_mpi_size();
			MPI_Comm_size(MPI_COMM_WORLD,&mpiSize);
			std::string file_name = base_name + "." + std::to_string(mpiSize) + ".perf";
#else
			std::size_t nthreads=0;
#pragma omp parallel
			{
				nthreads = omp_get_num_threads();
			}
			std::string file_name = base_name + "." + std::to_string(nthreads) + ".perf";
#endif
			return file_name;
		}
		
		void write_file()
		{
			auto name = build_name();
			write_file(name);
		}

		void write_file(std::string a_name)
		{
			using namespace MATools::MAOutput;
			using namespace MATools::MPI;
			//using MATools::MPI::reduce_max;

			std::ofstream myFile (a_name, std::ofstream::out);	
			MATimerNode* root_timer = MATools::MATimer::get_MATimer_node<ROOT>();
			assert(root_timer != nullptr);
			auto rootTime = root_timer->get_duration();
			rootTime = MATools::MPI::reduce_max(rootTime);
			auto my_write = [rootTime](MATimerNode* a_ptr, std::ofstream& a_file)
			{
				std::string space;
				std::string motif = "   ";

				for(std::size_t i = 0 ; i < a_ptr->get_level() ; i++) space +=motif;

				const auto max_time = reduce_max(a_ptr->get_duration());

				if(is_master())
				{
					a_file << space << a_ptr->get_name() 
						<< " " << a_ptr->get_iteration()
						<< " " << max_time
						<< " " <<(max_time/rootTime)*100
						<< std::endl;
				}
			};

			recursive_call(my_write,root_timer,myFile);
		}

		/*
		 * @brief Create debugging file names for each MPI process
		 * @return local name
		 */
		std::string build_current_mpi_name()
		{
			using namespace MATools::MPI;
			std::string base_name = "MATimers";
#ifdef __MPI
			int mpi_rank = get_rank();
			std::string file_name = base_name + "." + std::to_string(mpi_rank) + ".debug";
#else
			std::size_t nthreads=0;
#pragma omp parallel
			{
				nthreads = omp_get_num_threads();
			}
			std::string file_name = base_name + "." + std::to_string(nthreads) + ".debug";
#endif
			return file_name;
		}

		/*
		 * @brief Write debug files (tree) for each MPI process
		 * @param[in]  a_name name of the ouput file (should be different for each MPI process)
		 */
		void write_debug_file(std::string a_name)
		{
			using namespace MATools::MAOutput;
			using namespace MATools::MPI;

			std::ofstream myFile (a_name, std::ofstream::out);	
			MATimerNode* root_timer = MATools::MATimer::get_MATimer_node<ROOT>();
			assert(root_timer != nullptr);
			auto rootTime = root_timer->get_duration();
			rootTime = MATools::MPI::reduce_max(rootTime);
			auto my_debuging_write_function = [rootTime](MATimerNode* a_ptr, std::ofstream& a_file)
			{
				std::string space;
				std::string motif = "   ";

				for(std::size_t i = 0 ; i < a_ptr->get_level() ; i++) space +=motif;
				// get timer value
				auto value = a_ptr->get_duration();
				a_file << space << a_ptr->get_name() 
					<< " " << a_ptr->get_iteration()
					<< " " << value
					<< std::endl;
			};

			recursive_call(my_debuging_write_function, root_timer, myFile);
		}

		/*
		 * @brief Write a debug file for each MPI process
		 */
		void write_debug_file()
		{
			auto name = build_current_mpi_name();
			write_debug_file(name);
		}

		std::vector<MATools::MATimer::MATimerInfo> get_filtered_timers(std::string a_name)
		{
			using namespace MATools::MAOutput;
			using namespace MATools::MPI;
			using namespace MATools::MATimer;
			std::vector<MATimerInfo> ret;
			MATimerNode* root_timer = MATools::MATimer::get_MATimer_node<ROOT>();
			auto my_fill = []	(MATimerNode* a_ptr, std::vector<MATimerInfo>& a_ret, std::string a_filter)
			{
				// filter
				if(a_ptr->get_name() == a_filter)
				{
					// build full path
					auto tmp_ptr = a_ptr->get_mother();
					std::string current_path = "";
					while(tmp_ptr != nullptr)
					{
						current_path = tmp_ptr->get_name() + "/" + current_path;
						tmp_ptr = tmp_ptr->get_mother();
					}
					// get information in MATimerNode
					auto name = current_path + a_filter;
					auto it = a_ptr->get_iteration();
					double duration = a_ptr->get_duration();
					double max = reduce_max(duration);
					double mean = reduce_mean(duration);
					// fill the vector of MATimerInfo
					MATimerInfo tmp(name, it, max, mean);
					a_ret.push_back(std::move(tmp));
				}
			};
			recursive_call(my_fill,root_timer, ret, a_name);
			return ret;
		}

		void print_filtered_timers(std::string a_name)
		{
			auto filtered_timers = get_filtered_timers(a_name);
			bool first = true;
			for(auto it : filtered_timers)
			{
				if(first)
				{
					it.header();
					first = false;
				}
				it.print();
			}
		}
	}
};
