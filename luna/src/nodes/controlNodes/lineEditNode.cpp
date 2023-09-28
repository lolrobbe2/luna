#include "lineEditNode.h"
#include <core/rendering/renderer2D.h>
#include <core/events/mouseEvent.h>
#include <core/events/keyEvent.h>

#include <core/application.h>

#define UNSELECTED_COLOR {204.0f,136.935f, 28.05f, 1.0f}
#define SELECTED_COLOR {255, 172.89, 37.995, 1.0f} 
#define NORMALIZED_BORDER_SIZE 0.03f

namespace luna
{
	namespace nodes
	{
		lineEditNode::lineEditNode(entt::entity handle, luna::scene* scene) : controlNode(handle, scene)
		{

		}
		lineEditNode::lineEditNode(luna::scene* scene) : controlNode(scene)
		{

		}

		void lineEditNode::init(luna::scene* scene)
		{
			controlNode::init(scene);
			LN_CLASS_TYPE_NAME(controlNode);
			LN_CANVAS_COMPONENT();
			LN_EVENT_COMPONENT();
			addComponent<lineEditComponent>();
		}
		void lineEditNode::draw()
		{
			auto& lineEdit = getComponent<lineEditComponent>();
			auto transform = getComponent<transformComponent>();
			auto transform2 = getComponent<transformComponent>();
			transform2.translation.x -= transform.scale.x / 2.0f - transform2.scale.x * 0.03f;

			transform.setScale(transform.scale + NORMALIZED_BORDER_SIZE);
			glm::vec4 rectColor;
			lineEdit.selected ? rectColor = SELECTED_COLOR : rectColor = UNSELECTED_COLOR;
			renderer::renderer2D::drawQuad(transform.getTransform(), rectColor);
			renderer::renderer2D::drawQuad(getComponent<transformComponent>().getTransform(), {36.0f,37.0f,38.0f,1.0f});

			if (lineEdit.font) drawString(lineEdit.font, transform2.translation, lineEdit.drawText,16,color(), lineEdit.bounds, lineEdit.indexOutOfBounds);
		}

		void lineEditNode::guiEvent(Event& event)
		{
			auto& lineEdit = getComponent<lineEditComponent>();
			
			if (event.getEventType() == eventType::MouseMoved)
			{
				bool prevHovered = lineEdit.hovered;
				lineEdit.hovered = isHovered();
				if (prevHovered != lineEdit.hovered)
				{
					if(prevHovered)
					{
						application::application::get().getWindow().setCursorShape(vulkan::ARROW);
					}
					else 
					{
						application::application::get().getWindow().setCursorShape(vulkan::IBEAM);
					}
				}
			}
			if(event.getEventType() == eventType::MouseButtonPressed)
			{
				mouseButtonPressedEvent* mouseEvent = (mouseButtonPressedEvent*)&event;
				if(mouseEvent->getMouseButton() == Mouse::ButtonLeft)
				{
					if(lineEdit.hovered)
					{
						lineEdit.selected = !lineEdit.selected;
					}
					else { lineEdit.selected = false; }
				}
			}
			if (event.getEventType() == eventType::KeyTyped)
			{
				
				keyTypedEvent* keyBoardEvent = (keyTypedEvent*)&event;
				lineEdit.text += keyBoardEvent->getkeyCode();
				lineEdit.drawText = lineEdit.text;
				updateMetrics();
			}
			if (event.getEventType() == eventType::KeyPressed)
			{
				keyPressedEvent* keyPressed = (keyPressedEvent*)&event;
				if (keyPressed->getkeyCode() == input::Backspace && lineEdit.text.size()) lineEdit.text.pop_back();
				lineEdit.drawText = lineEdit.text;
				updateMetrics();
			}
		}

		void lineEditNode::updateMetrics()
		{
			auto& lineEdit = getComponent<lineEditComponent>();
			auto transform = getComponent<transformComponent>();
			lineEdit.bounds = { transform.translation.x - transform.scale.x / 2.0f,transform.translation.x + transform.scale.x / 2.0f, transform.translation.y - transform.scale.y / 2.0f,transform.translation.y + transform.scale.y / 2.0f };
			if (!lineEdit.font) return;
			lineEdit.drawText = lineEdit.text;
			if(lineEdit.indexOutOfBounds)lineEdit.drawText.resize(lineEdit.indexOutOfBounds);
		}
		bool lineEditNode::isHovered()
		{
			auto transform = getComponent<transformComponent>();
			transform.scale += NORMALIZED_BORDER_SIZE;
			glm::vec2 normailizedMousePos = renderer::renderer::getSceneMousePos() / renderer::renderer::getSceneDimensions();
			normailizedMousePos.x -= 0.5f;
			normailizedMousePos.y -= 0.5f;

			glm::vec2 leftCorner = { transform.translation.x - transform.scale.x / 2.0f,transform.translation.y - transform.scale.y / 2.0f };
			glm::vec2 rightCorner = { transform.translation.x + transform.scale.x / 2.0f,transform.translation.y + transform.scale.y / 2.0f };
			leftCorner /= 2.0f; //origin coordinates are in center!
			rightCorner /= 2.0f;//origin coordinates are in center!
			bool hover = (leftCorner.x < normailizedMousePos.x && leftCorner.y < normailizedMousePos.y && rightCorner.x > normailizedMousePos.x && rightCorner.y > normailizedMousePos.y);
			return hover;
		}
		
	}
}