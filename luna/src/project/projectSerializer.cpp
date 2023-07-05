#include "projectSerializer.h"
#include <core/platform/platformUtils.h>
#include <yaml-cpp/yaml.h>

namespace luna 
{
    namespace project
    {
        bool projectSerializer::serialize(ref<project> project)
        {
            YAML::Emitter out;
            out << YAML::BeginMap;
            out << YAML::Key << "projectName" << YAML::Value << project->getConfig().name;
            out << YAML::Key << "projectPaths" << YAML::Value << YAML::BeginMap;
            out << YAML::Key << "assetDirectory" << YAML::Value << project->getConfig().assetDirectory.string();
            out << YAML::Key << "projectDirectory" << YAML::Value << std::filesystem::absolute(project->getConfig().projectDirectory).string();
            out << YAML::Key << "scriptModulePath" << YAML::Value << project->getConfig().scriptModulePath.string();
            out << YAML::Key << "startScenePath" << YAML::Value << project->getConfig().startScene.string();
            out << YAML::EndMap;
            out << YAML::EndMap;
            if (out.good())
            {
                std::ofstream appDataProjectFile(platform::filesystem::getSystemFolderPath(platform::appData) + "\\luna\\projects\\" + project->getConfig().name + ".lprj");
                appDataProjectFile << out.c_str();

                std::ofstream localProjectFile(project->getConfig().projectDirectory.string() + "\\" + project->getConfig().name + ".lprj");
                localProjectFile << out.c_str();
                return true;
            }
            LN_CORE_ERROR("YAML write error");
            LN_CORE_ERROR(out.GetLastError());
            return false;
        }
        ref<project> projectSerializer::deSerialize(const std::filesystem::path& projectPath)
        {
            std::ifstream stream(projectPath);
            if (!stream.is_open()) return false;
            std::stringstream strstream;
            strstream << stream.rdbuf();
            stream.close();

            YAML::Node data = YAML::Load(strstream.str());
            
            

            projectConfig config;
            
            config.name = data["projectName"].as<std::string>();
            

            YAML::Node projectPaths = data["projectPaths"];

            config.projectDirectory = projectPaths["projectDirectory"].as<std::string>();
            config.assetDirectory = projectPaths["assetDirectory"].as<std::string>();
            config.scriptModulePath = projectPaths["scriptModulePath"].as<std::string>();
            config.startScene = projectPaths["startScenePath"].as<std::string>();
            
            if (config.assetDirectory.is_absolute())
                LN_CORE_WARN("asset directory path should be relative!");
            if (config.scriptModulePath.is_absolute())
                LN_CORE_WARN("script module path should be relative!");
            if (config.startScene.is_absolute())
                LN_CORE_WARN("start scene path should be relative!");
            if (config.projectDirectory.is_relative())
                LN_CORE_WARN("project directory path should be absolute");
            return ref<project>(new project(config));
        }
    }
}
