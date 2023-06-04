#include "init.h"
#include <yaml-cpp/yaml.h>
#include <core/scene/sceneSerializer.h>
namespace luna
{
	namespace config
	{
		init::init(const std::string& filePath)
		{
			YAML::Node startupConfig = YAML::LoadFile(filePath);
			YAML::Node startup = startupConfig["startup"];
			startupScenePath = startup["startscene"].as<std::string>();
		}
		scene* init::getStartScene()
		{
			return sceneSerializer::deSerialize(startupScenePath);
		}
	}
}