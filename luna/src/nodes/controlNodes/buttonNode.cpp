#include "buttonNode.h"
#include <core/assets/assetManager.h>
namespace luna
{
	namespace nodes
	{
		buttonNode::buttonNode(entt::entity handle, luna::scene* scene) : spriteNode(handle, scene)
		{

		}
		buttonNode::buttonNode(luna::scene* scene) : spriteNode(scene)
		{
			getComponent<spriteRendererComponent>().showInEditor = false;
			addComponent<buttonComponent>();

		}
		void buttonNode::init(luna::scene* scene)
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(buttonNode);
			addComponent<scriptComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*sprite Node Components*/
			addComponent<transformComponent>();
			addComponent<spriteRendererComponent>().showInEditor = false;
			auto& button = addComponent<buttonComponent>();
			CHAR currentDir[256] = { 0 };
			std::string currentDirectory;
			if (GetCurrentDirectoryA(256, currentDir))
				currentDirectory = currentDir;
			LN_CORE_INFO("working directory = {0}", currentDirectory);
			button.showInEditor = false;
			button.normalFilePath = "src/assets/media/buttons/button1.png";
			button.hoverFilePath = "src/assets/media/buttons/button2.png";
			button.pressedFilePath = "src/assets/media/buttons/button3.png";


			button.normalTexture = std::dynamic_pointer_cast<renderer::texture>(assets::assetManager::getAsset(assets::assetManager::importAsset(button.normalFilePath.string(),assets::texture)));
			button.hoverTexture = std::dynamic_pointer_cast<renderer::texture>(assets::assetManager::getAsset(assets::assetManager::importAsset(button.hoverFilePath.string(),assets::texture)));
			button.pressedTexture = std::dynamic_pointer_cast<renderer::texture>(assets::assetManager::getAsset(assets::assetManager::importAsset(button.pressedFilePath.string(),assets::texture)));
		}
	}
}
