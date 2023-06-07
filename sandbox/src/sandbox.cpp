#pragma once
#include <luna.h>
#include <core/assets/editorAssetManager.h>
class sandbox : public luna::application::application
{
public:
	sandbox() //base function
	{
		LN_TRACE("started app");
		luna::assets::editorAssetManager* assetManager = new luna::assets::editorAssetManager();
		assetManager->importAsset("src/assets/media/statue.png", luna::assets::texture);
		//assetManager->loadImportedAssetsMetadata();
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