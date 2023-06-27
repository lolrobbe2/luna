#include "projectGeneratorVS.h"
#include <core/platform/platformUtils.h>
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
			luaFile << "	architecture \"x64\"" << std::endl;
			luaFile << "	configurations" << std::endl;
			luaFile << "	{" << std::endl;
			luaFile << "		\"debug\"," << std::endl;
			luaFile << "		\"release\"," << std::endl;
			luaFile << "		\"distribution\"" << std::endl;
			luaFile << "	}" << std::endl;


			luaFile << "project \"" << name <<"\"" << std::endl;
			luaFile << "	location " << std::filesystem::absolute(dirPath).concat("\\").concat(name) << std::endl;
		
			luaFile << "	outputdir = \"% {cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\"" << std::endl;

			luaFile << "	LibraryDir = {}" << std::endl;
			luaFile << "	LibraryDir[\"scriptCore\"] = \"%{wks.location}/lib/\"" << std::endl;

			luaFile << "	Library = {}" << std::endl;
			luaFile << "	Library[\"scriptCore\"] = \"%{LibraryDir.scriptCore}/scriptCore.dll\"" << std::endl;

			
			luaFile << "	kind \"SharedLib\"" << std::endl;
			luaFile << "	language \"c#\""<< std::endl;
			luaFile << "    targetdir(\"%{wks.location}/bin/\" .. outputdir .. \"/x64/%{prj.name}\")" << std::endl;
			luaFile << "	objdir(\"%{wks.location}/bin-int/\" .. outputdir .. \"/x64/%{prj.name}\")" << std::endl;
			luaFile << "    links" << std::endl;
			luaFile << "		{" << std::endl;
			luaFile << "			\"%{Library.scriptCore}\"" << std::endl;
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
			std::filesystem::path filePath = dirPath.native();
			filePath.concat("\\premake5.lua");
			std::ofstream buildFile(filePath);
			buildFile << luaFile.str();
			buildFile.close();
			
			try
			{
				LN_CORE_INFO("creating visualStudio solution");
				std::string buildDir = platform::filesystem::getSystemFolderPath(platform::appData) + "\\luna\\build";
				std::string cmd = buildDir + "\\premake5 vs2022";
				std::system(cmd.c_str());
			}
			catch (const std::exception& e)
			{
				LN_CORE_ERROR("an error occured while trying to create visualStudio solution! {}", e.what());
			}
			catch (...)
			{
				LN_CORE_CRITICAL("unkown error occured while trying to create visualStudio solution");
			}
		}
	}
}