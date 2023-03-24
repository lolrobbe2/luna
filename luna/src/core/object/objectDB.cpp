#include "objectDB.h"
#include <core/scene/scene.h>
namespace luna
{

	void objectDB::createInstance(const std::string& className, scene* scene)
	{
		classInfo* info = getPtr(className);
		if (!info) return;
		Node* node = (Node*)info->creation_func();
		node->init(scene);
		node->setName(className);
	}
	
}
