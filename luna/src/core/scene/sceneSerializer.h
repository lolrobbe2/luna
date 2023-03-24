#pragma once
#include <core/scene/scene.h>

namespace luna
{
	class LN_API sceneSerializer
	{
	public:	
		static scene* deSerialize(const std::string& filePath);
		static bool serialize(scene* scene,const std::string& filePath);
	
	private:

	};

}
