#include "lineEditNode.h"
#include <core/rendering/renderer2D.h>
#include <core/events/mouseEvent.h>
#include <core/events/keyEvent.h>
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
			transform.scale += NORMALIZED_BORDER_SIZE;
			glm::vec4 rectColor;
			getComponent<lineEditComponent>().selected ? rectColor = SELECTED_COLOR : rectColor = UNSELECTED_COLOR;
			renderer::renderer2D::drawQuad(transform.getTransform(), rectColor);
			renderer::renderer2D::drawQuad(getComponent<transformComponent>().getTransform(), {36.0f,37.0f,38.0f,1.0f});
			if (getComponent<lineEditComponent>().font) drawString(getComponent<lineEditComponent>().font, transform.translation, getComponent<lineEditComponent>().drawText,16,color(), lineEdit.bounds, lineEdit.indexOutOfBounds);
		}

		void lineEditNode::guiEvent(Event& event)
		{
			if(event.getEventType() == eventType::MouseButtonPressed)
			{
				mouseButtonPressedEvent* mouseEvent = (mouseButtonPressedEvent*)&event;
				if(mouseEvent->getMouseButton() == Mouse::ButtonLeft)
				{
					if (isHovered())
					{
						getComponent<lineEditComponent>().selected = !getComponent<lineEditComponent>().selected;
					}
					else { getComponent<lineEditComponent>().selected = false; }
				}
			}
			if (event.getEventType() == eventType::KeyTyped)
			{
				
				keyTypedEvent* keyBoardEvent = (keyTypedEvent*)&event;
				getComponent<lineEditComponent>().text += keyBoardEvent->getkeyCode();
				getComponent<lineEditComponent>().drawText = getComponent<lineEditComponent>().text;
				updateMetrics();
			}
			if (event.getEventType() == eventType::KeyPressed)
			{
				keyPressedEvent* keyPressed = (keyPressedEvent*)&event;
				if (keyPressed->getkeyCode() == input::Backspace) getComponent<lineEditComponent>().text.pop_back();
				getComponent<lineEditComponent>().drawText = getComponent<lineEditComponent>().text;
				updateMetrics();
			}
		}

		void lineEditNode::updateMetrics()
		{
			auto& lineEdit = getComponent<lineEditComponent>();
			auto transform = getComponent<transformComponent>();
			float pxNorm = (lineEdit.points * 1.333);
			pxNorm /= renderer::renderer::getSceneDimensions().y;
			float xAdvance = 0.0f;
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