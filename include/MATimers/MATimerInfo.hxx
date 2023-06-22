/**
 * @file
 * @brief This file contains the MATimerInfo class definition.
 */

#pragma once

#include <MAOutput/MAOutput.hxx>
#include <Common/MAToolsDecoration.hxx>

namespace MATools
{
	namespace MATimer
	{
		/**
		 * @class MATimerInfo
		 * @brief Represents timer information.
		 * The MATimerInfo class represents information about a timer, including its name, number of iterations,
		 * duration, and mean duration (if compiled with MPI support).
		 */
		class MATimerInfo
		{
			public:

			// members
			std::string m_full_name; /**< The full name of the timer. */
			std::size_t m_n_iterations; /**< The number of calls of the timer. */
			double m_duration; /**< The accumulate duration of code section capture by the timer. This value is the maximum duration with MPI. */
#ifdef __MPI
			double m_mean_duration; /**< The mean duration of the timer (MPI-specific). */
#endif

			// default constructor -- not used
			MATools_DECORATION
			MATimerInfo() {};

			/**
			 * @brief Constructs a MATimerInfo object with specified values.
			 * @param a_full_path The full path of the timer.
			 * @param a_n_iterations The number of iterations of the timer.
			 * @param a_duration The duration of the timer.
			 * @param a_mean_duration The mean duration of the timer (optional, only used with MPI support).
			 */
			MATools_DECORATION
			MATimerInfo(std::string a_full_path, std::size_t a_n_iterations, double a_duration, [[maybe_unused]] double a_mean_duration = 0)
			{
				m_full_name = a_full_path;
				m_n_iterations = a_n_iterations;
				m_duration = a_duration;
#ifdef __MPI
				m_mean_duration = a_mean_duration;
#endif
			}

			// setters and getters

			/**
			 * @brief Gets the name of the timer.
			 * @return The name of the timer.
			 */
			MATools_DECORATION
			std::string get_name()
			{
				std::string ret = m_full_name;
				return ret;
			}

			/**
			 * @brief Gets the number of iterations done by the timer.
			 * @return The number of iterations done by the timer.
			 */
			MATools_DECORATION
			std::size_t get_number_of_iterations()
			{
				std::size_t ret = m_n_iterations;
				return ret;
			}

			/**
			 * @brief Gets the duration of the timer.
			 * @return The duration of the timer.
			 */
			MATools_DECORATION
			double get_duration()
			{
				double ret = m_duration;
				return ret;
			}

			/**
			 * @brief Gets the maximum duration of the timer.
			 * @return The maximum duration of the timer.
			 */
			MATools_DECORATION
			double get_max_duration()
			{
				double ret = get_duration();
				return ret;
			}

			/**
			 * @brief Gets the mean duration of the timer.
			 * If compiled with MPI support, this function returns the mean duration. Otherwise, it returns the duration.
			 * @return The mean duration of the timer (MPI-specific) or the duration.
			 */
			MATools_DECORATION
			double get_mean_duration()
			{
#ifdef __MPI
				double ret = m_mean_duration;
#else
				double ret = get_duration();
#endif
				return ret;
			}

			// outputs

			/**
			 * @brief Prints the header for timer information.
			 */
			void header()
			{
				using namespace MATools::MAOutput;
#ifdef __MPI
				printMessage("full name", "-", "number of iterations", "-", "mean time(s)", "-", "max time(s)");
#else
				printMessage("full name", "-", "number of iterations", "-", "time(s)");
#endif
			}

			/**
			 * @brief Prints the timer information.
			 */
			void print()
			{
				using namespace MATools::MAOutput;
#ifdef __MPI
				printMessage(get_name(), get_number_of_iterations(), get_mean_duration(), get_max_duration());	
#else
				printMessage(get_name(), get_number_of_iterations(), get_duration());	
#endif
			}
		};
	}
}

