#pragma once
#include <luna.h>
#include <core/assets/editorAssetManager.h>
#include <core/vulkan/rendering/vulkanTexture.h>
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