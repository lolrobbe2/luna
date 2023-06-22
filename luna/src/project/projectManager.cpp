#include <core/platform/platformUtils.h>
#include "projectManager.h"
namespace luna 
{
	namespace project 
	{
		static project activeProject;
		static std::vector<project> projects;
		void projectManager::init()
		{
			std::string appDataPath = platform::filesystem::getSystemFolderPath(platform::appData);
			std::filesystem::path lunaDataPath = appDataPath + "\\luna";
			std::filesystem::path lunaProjectPath = appDataPath + "\\luna\\projects";
			if (!std::filesystem::exists(lunaProjectPath)) {
				std::filesystem::create_directories(lunaProjectPath);
			}
			for (auto projects : std::filesystem::directory_iterator(lunaProjectPath))
			{

			}
		}

		const std::filesystem::path& projectManager::getProjectDirectory()
		{
			return activeProject.getConfig().projectDirectory;
		}

		std::filesystem::path projectManager::getAssetDirectory()
		{
			return getProjectDirectory() / activeProject.getConfig().assetDirectory;
		}
		std::filesystem::path projectManager::getScriptModulePath()
		{
			return getProjectDirectory() / activeProject.getConfig().scriptModulePath;
		}
		std::filesystem::path projectManager::getStartScenePath()
		{
			return getProjectDirectory() / activeProject.getConfig().startScene;
		}
		ref<project> projectManager::createProject(const projectConfig& config)
		{
			return ref<project>(new project(config));
		}
		ref<project> projectManager::createProject(const std::string& name, const std::filesystem::path& startScene, const std::filesystem::path& assetDirectory, const std::filesystem::path& scriptModulePath, const std::filesystem::path& projectDirectory)
		{
			return ref<project>(new project({ name,startScene,assetDirectory,scriptModulePath,projectDirectory }));
		}
		void projectManager::setActive(const ref<project> project)
		{
			activeProject = project;
		}
	}
}