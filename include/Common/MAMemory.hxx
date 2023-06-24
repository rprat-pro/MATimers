#pragma once
#include <vector>
#include <sys/time.h>
#include <sys/resource.h>

namespace MATools
{
	namespace MAMemory
	{
		/**
		 * MAFootprint is a derived class of std::vector with rusage objects
		 */
		class MAFootprint : public std::vector<rusage>
		{
			public:
				/**
				 * Default constructor that calls default std::vector constructor 
				 */
				MAFootprint();

				/**
				 * This function create a memory checkpoint, i.e. a rusage, and insert it in the data storage 
				 */
				void add_memory_checkpoint();

				/**
				 * Reduce is called to get the total memory footprint among MPI processes for every memory checkpoints
				 * Data are gathered/reduced on the master mpi process 0.
				 * @return a vector with the total memory footprint by memory checkpoints  
				 */
				std::vector<long> reduce();
		};

		/**
		 * this function create a rusage variable
		 * @return rusage data type that get memory informations
		 */
		rusage make_memory_checkpoint();

		/*
		 * The memory footprint is printed for every memory checkpoints
		 * @param f is a mafootprint object that contains memory checkpoints
		 * @see mafootprint
		 */
		void print_checkpoints(MAFootprint& a_f);

		/*
		 * The memory footprint is written for every memory checkpoints
		 * @see mafootprint
		 */
		void write_memory_checkpoints(MAFootprint&, std::string="MAMemoryFootprint.mem");

		/*
		 * The memory footprint is printed where this function is called.
		 * @see MAFootprint
		 */
		void print_memory_footprint();

		/**
		 * @brief This function gets the "common" MAFootprint or create if necessary.
		 */
		MAFootprint& get_MAFootprint();
	};
};
