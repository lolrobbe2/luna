#include "buttonNode.h"
#include <core/assets/assetManager.h>
#include <core/events/mouseEvent.h>
#include <core/rendering/renderer.h>
#include <core/scripting/scriptingEngine.h>
#include <core/object/methodDB.h>
namespace luna
{
	namespace nodes
	{
		static bool ButtonNodeGetHovered(entt::entity handle)
		{
			return buttonNode(handle, scripting::scriptingEngine::getContext()).isHovered();
		}
		static void ButtonNodeSetToggleMode(entt::entity handle,bool mode)
		{
			buttonNode(handle, scripting::scriptingEngine::getContext()).setToggleMode(mode);
		}
		static bool ButtonNodeGetToggleMode(entt::entity handle) 
		{
			return buttonNode(handle, scripting::scriptingEngine::getContext()).getToggleMode();
		}
		static void ButtonNodeSetActionMode(entt::entity handle , actionMode mode)
		{
			buttonNode(handle, scripting::scriptingEngine::getContext()).setActionMode(mode);
		}
		static actionMode ButtonNodeGetActionMode(entt::entity handle)
		{
			return buttonNode(handle, scripting::scriptingEngine::getContext()).getActionMode();
		}
		void buttonNode::bindMethods()
		{
			LN_ADD_INTERNAL_CALL(buttonNode, ButtonNodeGetHovered);
			LN_ADD_INTERNAL_CALL(buttonNode, ButtonNodeSetToggleMode);
			LN_ADD_INTERNAL_CALL(buttonNode, ButtonNodeGetToggleMode);
			LN_ADD_INTERNAL_CALL(buttonNode, ButtonNodeSetActionMode);
			LN_ADD_INTERNAL_CALL(buttonNode, ButtonNodeGetActionMode);
		}

		buttonNode::buttonNode(entt::entity handle, luna::scene* scene) : spriteNode(handle, scene)
		{

		}
		buttonNode::buttonNode(luna::scene* scene) : spriteNode(scene)
		{
			getComponent<spriteRendererComponent>().showInEditor = false;
			addComponent<buttonComponent>();

		}
		bool buttonNode::isHovered()
		{
			return getComponent<buttonComponent>().hover;
		}
		void buttonNode::setToggleMode(bool mode)
		{
			getComponent<buttonComponent>().toggleMode = mode;
		}
		bool buttonNode::getToggleMode()
		{
			return getComponent<buttonComponent>().toggleMode;
		}
		void buttonNode::setActionMode(actionMode mode)
		{
			getComponent<buttonComponent>().actionMode = mode;
		}
		actionMode buttonNode::getActionMode()
		{
			return getComponent<buttonComponent>().actionMode;
		}
		void buttonNode::init(luna::scene* scene) 
		{
			this->scene = scene;
			entityHandle = scene->create();
			addComponent<idComponent>().typeName = LN_CLASS_STRINGIFY(buttonNode);
			addComponent<scriptComponent>();
			addComponent<signalComponent>();
			LN_CORE_INFO("node uuid = {0}", getUUID().getId());
			/*sprite Node Components*/
			addComponent<transformComponent>();
			auto& sprite = addComponent<spriteRendererComponent>();
			sprite.showInEditor = false;
		
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
			
			sprite.texture = button.normalTexture;
		}
		void buttonNode::guiEvent(Event& event)
		{
			glm::vec2 normailizedMousePos = renderer::renderer::getSceneMousePos() / renderer::renderer::getSceneDimensions();
			normailizedMousePos.x -= 0.5f;
			normailizedMousePos.y -= 0.5f;
			spriteRendererComponent& sprite = getComponent<spriteRendererComponent>();
			buttonComponent& button = getComponent<buttonComponent>();
			if (event.getEventType() == eventType::MouseMoved)
			{
				mouseMovedEvent* mouseMoveEvent = (mouseMovedEvent*)&event;
				if (!getComponent<spriteRendererComponent>().outOfBounds) {

					transformComponent& transform = getComponent<transformComponent>();
					glm::vec2 leftCorner = { transform.translation.x - transform.scale.x / 2.0f,transform.translation.y - transform.scale.y / 2.0f };
					glm::vec2 rightCorner = { transform.translation.x + transform.scale.x / 2.0f,transform.translation.y + transform.scale.y / 2.0f };
					leftCorner /= 2.0f; //origin coordinates are in center!
					rightCorner /= 2.0f;//origin coordinates are in center!
					buttonComponent& button = getComponent<buttonComponent>();
					bool previousHover = button.hover;
					button.hover = (leftCorner.x < normailizedMousePos.x && leftCorner.y < normailizedMousePos.y && rightCorner.x > normailizedMousePos.x && rightCorner.y > normailizedMousePos.y);
					if (button.hover) sprite.texture = button.hoverTexture;
					else if(button.hover != previousHover) sprite.texture = button.normalTexture;
				}
			}
			if (event.getEventType() == eventType::MouseButtonPressed) 
			{
				mouseButtonPressedEvent* mouseEvent = (mouseButtonPressedEvent*)&event;
				if (mouseEvent->getMouseButton() == Mouse::ButtonLeft)
				{
					if (button.hover) 
					{
						
						LN_EMIT_SIGNAL("ButtonDown"); 
						if (getActionMode() == ACTION_MODE_BUTTON_PRESS) 
						{ 
							LN_EMIT_SIGNAL("Pressed"); 
							if(getToggleMode()) 
							{
								button.pressed != button.pressed;
								if (button.pressed) sprite.texture = button.pressedTexture;
								else sprite.texture = button.normalTexture;
								LN_EMIT_SIGNAL("Toggled", button.pressed);
							}
							else 
							{
								sprite.texture = button.pressedTexture;
							}
						} 
						else 
						{
							sprite.texture = button.pressedTexture;
						}
					}
				}
			}
			else if (event.getEventType() == eventType::MouseButtonReleased)
			{
				mouseButtonPressedEvent* mouseEvent = (mouseButtonPressedEvent*)&event;
				if (mouseEvent->getMouseButton() == Mouse::ButtonLeft)
				{
					
					if (button.hover)
					{

						LN_EMIT_SIGNAL("ButtonUp");
						if (getActionMode() == ACTION_MODE_BUTTON_RELEASE)
						{
							LN_EMIT_SIGNAL("Pressed");
							if (getToggleMode())
							{
								button.pressed != button.pressed;
								if (button.pressed) sprite.texture = button.pressedTexture;
								else sprite.texture = button.normalTexture;
								LN_EMIT_SIGNAL("Toggled", button.pressed);
							}
							else
							{
								sprite.texture = button.normalTexture;
							}
						} 
						else 
						{
							sprite.texture = button.normalTexture;
						}
					}
				}
			}
		}
	}
}
