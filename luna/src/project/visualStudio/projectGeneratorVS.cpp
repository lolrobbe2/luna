#include "projectGeneratorVS.h"
namespace luna 
{
	namespace project 
	{
		void projectGeneratorVS::generateProject(const std::string& name, const std::filesystem::path& dirPath)
		{
			//the premake5.lua file needs to be made this way otherwise we cant enter some custom data like project name and such!

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

			luaFile << "LibraryDir = {}" << std::endl;

			luaFile << "Library = {}" << std::endl;
			luaFile << "Library[\"scriptCore\"] = %{LibraryDir.scriptCore}/scriptCore.dll" << std::endl;

			luaFile << "project \"" << name <<"\"" << std::endl;
			luaFile << "	location \"sharpSandbox\"" << std::endl;
			luaFile << "	kind \"SharedLib\"" << std::endl;
			luaFile << "	language \"c#\""<< std::endl;
			luaFile << "    targetdir(\"%{wks.location}/bin/\" .. outputdir .. \"/x64/%{prj.name}\")" << std::endl;
			luaFile << "	objdir(\"%{wks.location}/bin-int/\" .. outputdir .. \"/x64/%{prj.name}\")" << std::endl;
			luaFile << "    links" << std::endl;
			luaFile << "		{" << std::endl;
			luaFile << "			%{Library.scriptCore}" << std::endl;
			luaFile << "		}" << std::endl;
			luaFile << "	filter \"configurations:debug\"" << std::endl;
			luaFile << "		optimize \"Off\"" << std::endl;
			luaFile << "		symbols \"Default\"" << std::endl;

			luaFile << "	filter \"configurations:release\""<< std::endl;
			luaFile << "		optimize \"On\""<< std::endl;
			luaFile << "		symbols \"Default\""<< std::endl;

			luaFile << "	filter \"configurations:distribution\""<< std::endl;
			luaFile << "		optimize \"Full\""<< std::endl;
			luaFile << "		symbols \"Off\""<< std::endl;

			if (!std::filesystem::is_directory(dirPath))
				return LN_CORE_ERROR("premake.lua directory is not a directory: {}", dirPath.string());
			std::ofstream buildFile(dirPath.string() + "\\premale5.lua");
			buildFile.write(luaFile.str().c_str(),luaFile.str().size());
			buildFile.close();
		}
	}
}