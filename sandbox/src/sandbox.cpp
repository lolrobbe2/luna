#pragma once
#include <core/platform/platformUtils.h>
#include <project/projectManager.h>
#include <project/visualStudio/projectGeneratorVS.h>
#include <luna.h>
class sandbox : public luna::application::application
{
public:
	sandbox() //base function
	{
		LN_TRACE("started app");
		luna::assets::editorAssetManager* assetManager = new luna::assets::editorAssetManager();
		assetManager->importAsset("src/assets/media/statue.png", luna::assets::texture);
		assetManager->importAsset("src/assets/media/Roboto-Regular.ttf", luna::assets::font); 
		assetManager->loadImportedAssetsMetadata();
    
		//luna::project::projectManager::init();
	}
	virtual ~sandbox() //base breaker function
	{
		
	}

};

luna::application::application* luna::application::createApplication()
{
	//hello
	return new sandbox(); // returns the application
}