#include <MATrace/MATraceInstance.hxx>

namespace MATools
{
	namespace MATrace
	{
		MATrace_point& get_ref_MATrace_point()
		{
			static MATrace_point instance;
			return instance;
		}

		MATrace_point& get_MATrace_point()
		{
			static MATrace_point instance;
			return instance;
		}
		
		Trace& get_local_MATrace()
		{
			static Trace instance;
			return instance;
		}

		std::vector<MATrace_point>& get_MATrace_omp_point()
		{
			static std::vector<MATrace_point> instance;
			return instance;
		}
		
		std::vector<Trace>& get_omp_MATrace()
		{
			static std::vector<Trace> instance;
			return instance;
		}
	};
};
