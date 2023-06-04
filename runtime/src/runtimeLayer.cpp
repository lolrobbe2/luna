#include "runtimeLayer.h"
#include <core/input.h>
#include <core/platform/platformUtils.h>

namespace luna
{
	runtimeLayer::runtimeLayer(const std::string& name)
	{
		platform::os::createConsole();
		LN_CORE_INFO("creating runtime layer");
		initConfig = ref<config::init>(new config::init());
		startupScene = ref<scene>(initConfig->getStartScene());
		utils::scriptUtils::setContext(startupScene.get());
		startupScene->onPlayScene();
		startupScene->m_IsRunning = true;
	}
	void runtimeLayer::onAttach()
	{
		
	}
	void runtimeLayer::onDetach()
	{

	}
	void runtimeLayer::onUpdate(utils::timestep ts)
	{
		startupScene->onUpdate(ts);
		startupScene->onUpdateEditor(ts);
	}
	
	void runtimeLayer::onEvent(Event& Event)
	{
		startupScene->onEvent(Event);
	}

}
