#include "projectGeneratorVS.h"
namespace luna 
{
	namespace project 
	{
		void projectGeneratorVS::generateProject(const std::string& name)
		{
			std::stringstream luaFile;
			luaFile << "startproject \""<< name << "\"" << std::endl;
			luaFile << "workspace \"" << name << "\"" << std::endl;
			luaFile << "architecture \"x64\"";

			luaFile << "configurations" << std::endl;
			luaFile << "{" << std::endl;
			luaFile << "	\"debug\"," << std::endl;
			luaFile << "	\"release\"," << std::endl;
			luaFile << "	\"distribution\"" << std::endl;
			luaFile << "}" << std::endl;
			luaFile << "project \"" << name <<"\"" << std::endl;
			luaFile << "	location \"sharpSandbox\"" << std::endl;
			luaFile << "	kind \"SharedLib\"" << std::endl;
			luaFile << "	language \"c#\""<< std::endl;
			luaFile << "    targetdir(\"%{wks.location}/bin/\" .. outputdir .. \"/x64/%{prj.name}\")" << std::endl;
			luaFile << "	objdir(\"%{wks.location}/bin-int/\" .. outputdir .. \"/x64/%{prj.name}\")" << std::endl;
			luaFile << "    links" << std::endl;
			luaFile << "		{" << std::endl;
			luaFile << "			scriptCore" << std::endl;
			luaFile << "		}" << std::endl;
		}
	}
}