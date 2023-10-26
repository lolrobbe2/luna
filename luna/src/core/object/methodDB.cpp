#include "methodDB.h"
#include <core/scripting/scriptingEngine.h>
#ifndef TYPED_METHOD_BIND
class __UnexistingClass;
#define MB_T __UnexistingClass
#else
#define MB_T T
#endif

#include <core/debug/log.h>
#include <core/platform/platformUtils.h>
#include <core/object/signal.h>
#include <core/scene/node.h>
#include <core/debug/debugMacros.h>
//networking
#include <core/networking/ipAddress.h>
#include <core/networking/streamPeerTCP.h>

namespace luna
{
	#ifdef TYPED_METHOD_BIND
		template <class T, class... P>
	#else
		template <class... P>
	#endif
	struct Method
	{
		MonoMethod* externalMethod;
		void (MB_T::*method)(P...) = nullptr;
		int argsCount;
	};

	struct methodDBData
	{
		//TODO change map with hashed objStorage.
		std::map<std::string,Method<>> methodMap;
	};

	static methodDBData* s_Data = new methodDBData();

	void methodDB::init()
	{
		bindFunctions();
		bindStaticFunctions();
	}

	void methodDB::bindObjectFunctions(const std::string& className) 
	{
		const std::string name = className == "node" ? "Node" : scripting::scriptingEngine::pascalToCamel(className);
		objectDB::classInfo* info = objectDB::getPtr(name);
		LN_ERR_FAIL_NULL_MSG(info,"object class not registered in objectDB: " + name);
		Node* node = (Node*)info->creation_func();
		node->bindMethods();
	}


	void methodDB::bindFunctions()
	{
		for (const std::string& objectName : scripting::scriptingEngine::getCoreClassNames()) 
		{
			bindObjectFunctions(objectName);
		}
	}

	void methodDB::bindStaticFunctions()
	{
		Log::RegisterMethods();
		Os::RegisterMethods();
		Signal::RegisterMethods();
		networking::Ip::RegisterMethods();
		networking::ipAddress::registerMethods();
		networking::netSocket().bindMethods();
		networking::streamPeerTCP().bindMethods();
	}


	/* internall functions*/
}
