#include "projectGeneratorVS.h"
#include <core/platform/platformUtils.h>

namespace luna 
{
	namespace project 
	{
		void projectGeneratorVS::generateProject(const std::string& name,std::filesystem::path& dirPath)
		{
			std::string tempDirPath = dirPath.string();
			if (tempDirPath.back() == '\0') tempDirPath.pop_back(); //make non c-style string because we cannont append to that.
			dirPath = tempDirPath;
			generatePremakeFile(name, dirPath);

			if (!std::filesystem::is_directory(std::filesystem::absolute(dirPath)))
				return LN_CORE_ERROR("premake.lua directory is not a directory: {}", std::filesystem::absolute(dirPath).string());
			std::filesystem::path premakeFilePath = platform::os::getCurrentWorkingDirectory();
			premakeFilePath += "\\premake5.lua";
			std::ofstream buildFile(premakeFilePath);
			buildFile << generatePremakeFile(name, dirPath);
			buildFile.close();
			
			std::filesystem::path srcDir = dirPath;
			srcDir += "\\src";
			std::filesystem::create_directories(srcDir);

			std::filesystem::path libDir = dirPath;
			libDir += "\\mono\\lib";
			std::filesystem::create_directories(libDir);

			std::filesystem::path xmlDir = dirPath;
			xmlDir += "\\lib";

			std::filesystem::path exampleFilePath = srcDir;
			exampleFilePath += "\\Start.cs";
			std::ofstream exampleFile(exampleFilePath);
			exampleFile << generateExampleFile();
			exampleFile.close();

			
			if(std::filesystem::exists("mono\\lib\\scriptCore.dll"))
			{
				
				std::string dllPath = libDir.string();
				dllPath += "\\scriptCore.dll\0";
				if (std::filesystem::exists(dllPath)) std::filesystem::remove_all(dllPath); //precaustinary messure
				std::filesystem::copy_file("mono\\lib\\scriptCore.dll",dllPath);
			}

			if (std::filesystem::exists("mono\\lib\\scriptCore.xml"))
			{
				std::string xmlPath = libDir.string();
				xmlPath += "\\scriptCore.xml\0";
				if (std::filesystem::exists(xmlPath)) std::filesystem::remove_all(xmlPath); //precaustinary messure
				std::filesystem::copy_file("mono\\lib\\scriptCore.xml", xmlPath);
				
			}

			try
			{
				LN_CORE_INFO("creating visualStudio solution");
				std::string buildDir = platform::filesystem::getSystemFolderPath(platform::appData) + "\\luna\\build";
				std::string cmd = buildDir + "\\premake5 vs2022";
				std::system(cmd.c_str());
			}
			catch (const std::exception& e)
			{
				LN_CORE_ERROR("an error occured while trying to create visualStudio solution! {0}", e.what());
			}
			catch (...)
			{
				LN_CORE_CRITICAL("unkown error occured while trying to create visualStudio solution");
			}
			
			std::filesystem::remove_all(premakeFilePath);
		}
		std::string projectGeneratorVS::generatePremakeFile(const std::string& name, const std::filesystem::path& dirPath)
		{
			//the premake5.lua file needs to be made this way otherwise we cant enter some custom data like project name and such!
			std::stringstream luaFile;
			luaFile << "startproject \"" << name << "\"" << std::endl;
			luaFile << "workspace \"" << name << "\"" << std::endl;
			luaFile << "	location " << std::filesystem::absolute(dirPath) << std::endl;
			luaFile << "	architecture \"x64\"" << std::endl;
			luaFile << "	configurations" << std::endl;
			luaFile << "	{" << std::endl;
			luaFile << "		\"debug\"," << std::endl;
			luaFile << "		\"release\"," << std::endl;
			luaFile << "		\"distribution\"" << std::endl;
			luaFile << "	}" << std::endl;


			luaFile << "project \"" << name << "\"" << std::endl;
			luaFile << "	location " << std::filesystem::absolute(dirPath) << std::endl;

			luaFile << "	outputdir = \"%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\"" << std::endl;

			luaFile << "	LibraryDir = {}" << std::endl;
			luaFile << "	LibraryDir[\"scriptCore\"] = \"%{wks.location}/mono/lib/\"" << std::endl;

			luaFile << "	Library = {}" << std::endl;
			luaFile << "	Library[\"scriptCore\"] = \"%{LibraryDir.scriptCore}/scriptCore.dll\"" << std::endl;


			luaFile << "	kind \"SharedLib\"" << std::endl;
			luaFile << "	language \"c#\"" << std::endl;
			luaFile << "    targetdir(\"%{wks.location}/mono/lib\")" << std::endl;
			luaFile << "	objdir(\"%{wks.location}/build/bin-int/\" .. outputdir .. \"/x64/%{prj.name}\")" << std::endl;
			luaFile << "    files" << std::endl;
			luaFile << "	{" << std::endl;
			luaFile << "		\"%{wks.location}/src/**.cs\"" << std::endl;
			luaFile << "	}" << std::endl;

			luaFile << "    links" << std::endl;
			luaFile << "		{" << std::endl;
			luaFile << "			\"%{Library.scriptCore}\"" << std::endl;
			luaFile << "		}" << std::endl;

			luaFile << "	filter \"configurations:debug\"" << std::endl;
			luaFile << "		optimize \"Off\"" << std::endl;
			luaFile << "		symbols \"Default\"" << std::endl;

			luaFile << "	filter \"configurations:release\"" << std::endl;
			luaFile << "		optimize \"On\"" << std::endl;
			luaFile << "		symbols \"Default\"" << std::endl;

			luaFile << "	filter \"configurations:distribution\"" << std::endl;
			luaFile << "		optimize \"Full\"" << std::endl;
			luaFile << "		symbols \"Off\"" << std::endl;
			return luaFile.str();
		}
		std::string projectGeneratorVS::generateExampleFile() 
		{
			std::stringstream exampleFile;
			exampleFile << "using Luna;" << std::endl <<std::endl;
			exampleFile << "internal class start : Node" << std::endl;
			exampleFile << "{" << std::endl;
			exampleFile << "	override public void Ready()" << std::endl;
			exampleFile << "	{" << std::endl;
			exampleFile << "		Log.Info(\"hello world\");" << std::endl;
			exampleFile << "	}" << std::endl << std::endl;

			exampleFile << "	override public void Process(ulong delta)" << std::endl;
			exampleFile << "	{" << std::endl << std::endl;

			exampleFile << "	}" << std::endl;
			exampleFile << "}" << std::endl;
			return exampleFile.str();
		}
	}
}