#include <core/platform/platformUtils.h>
#include <project/projectSerializer.h>
#include "projectManager.h"
namespace luna 
{
	namespace project 
	{
		static ref<project> activeProject = nullptr;
		static std::vector<ref<project>> projects;
		void projectManager::init()
		{
			std::string appDataPath = platform::filesystem::getSystemFolderPath(platform::appData);
			std::filesystem::path lunaDataPath = appDataPath + "\\luna";
			std::filesystem::path lunaProjectPath = appDataPath + "\\luna\\projects";
			if (!std::filesystem::exists(lunaProjectPath)) {
				std::filesystem::create_directories(lunaProjectPath);
			}
			for (auto localProject : std::filesystem::directory_iterator(lunaProjectPath))
			{
				std::filesystem::path projectPath = localProject.path();
				if (projectPath.extension() == ".lprj")
				{	
					projects.push_back(projectSerializer::deSerialize(projectPath));
				}
			}
		}


		std::string projectManager::getName()
		{
			if(activeProject) return activeProject->getConfig().name;
			return "sharpSandbox";
		}

		const std::filesystem::path& projectManager::getProjectDirectory()
		{
			return activeProject->getConfig().projectDirectory;
		}

		std::filesystem::path projectManager::getAssetDirectory()
		{
			return getProjectDirectory() / activeProject->getConfig().assetDirectory;
		}
		std::filesystem::path projectManager::getScriptModulePath()
		{
			return getProjectDirectory() / activeProject->getConfig().scriptModulePath;
		}
		std::filesystem::path projectManager::getStartScenePath()
		{
			return getProjectDirectory() / activeProject->getConfig().startScene;
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
		void projectManager::removeProject(const ref<project> project) 
		{
			std::filesystem::path projectAppdataPath = platform::filesystem::getSystemFolderPath(platform::appData) + "\\luna\\projects\\" + project->getConfig().name + ".lprj";
			std::filesystem::remove(projectAppdataPath);
		}

		std::vector<ref<project>> projectManager::getProjects() { return projects; }
	}
}