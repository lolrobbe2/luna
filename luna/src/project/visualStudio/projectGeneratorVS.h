#pragma 
#include <core/core.h>
namespace luna 
{
	namespace project 
	{
		class LN_API projectGeneratorVS
		{
		public:
			static void generateProject(const std::string& name, const std::filesystem::path& dirPath);
			static const std::stringstream& generatePremakeFile(const std::string& name, const std::filesystem::path& dirPath);
		};
	}
}


