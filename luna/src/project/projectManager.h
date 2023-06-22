#pragma once
#include <core/core.h>
#include <project/project.h>

namespace luna
{
	namespace project
	{
		class LN_API projectManager
		{
		public:
			static void init();

			/**
			* @brief returns the directory of the active project.
			*/
			static const std::filesystem::path& getProjectDirectory();

			/**
			* @brief returns the assetdirectory of the active project.
			*/
			static std::filesystem::path getAssetDirectory();

			static std::filesystem::path getScriptModulePath();

			static std::filesystem::path getStartScenePath();
			
			static ref<project> createProject(const projectConfig& config);

			static ref<project> createProject(const std::string& name,const std::filesystem::path& startScene,const std::filesystem::path& assetDirectory,const std::filesystem::path& scriptModulePath,const std::filesystem::path& projectDirectory);
			
			static void setActive(const ref<project> project);
		};
	}
}

