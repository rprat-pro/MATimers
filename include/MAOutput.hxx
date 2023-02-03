#pragma once

#include <MAToolsMPI.hxx>
#include <iostream>

namespace MATools
{
	namespace MAOutput
	{
		using namespace MATools::MPI;

		/**
		 * @brief Displays one message if the current mpi rank is 0
		 */
		template<typename Arg>
			void printMessage(Arg a_msg)
			{
				if(is_master())
				{
					std::cout << a_msg << std::endl;
				}
			}

		/**
		 * @brief Displays some messages if the current mpi rank is the master. 
		 */
		template<typename Arg, typename... Args>
			void printMessage(Arg a_msg, Args... a_msgs)
			{
				if(is_master())
				{
					std::cout << a_msg << " ";
					printMessage(a_msgs...);
				}
			}
	};
};
