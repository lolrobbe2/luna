#include "lineEditNode.h"
#include <core/events/mouseEvent.h>
#include <core/events/keyEvent.h>

#include <core/application.h>
#include <core/debug/typedefs.h>
#include <core/platform/platformUtils.h>

#define UNSELECTED_COLOR {204.0f,136.935f, 28.05f, 255.0f}
#define SELECTED_COLOR {255, 172.89, 37.995, 255.0f} 
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
			LN_NOTIFICATION_FUNC();
			addComponent<lineEditComponent>();
		}
		void lineEditNode::draw()
		{
			auto& lineEdit = getComponent<lineEditComponent>();
			auto transform = getComponent<transformComponent>();
			auto transform2 = getComponent<transformComponent>();
			//transform2.translation.x -= transform.scale.x / 2.0f - transform2.scale.x * 0.03f;

			transform.setScale(transform.scale + NORMALIZED_BORDER_SIZE);
			glm::vec4 rectColor;
			lineEdit.selected ? rectColor = SELECTED_COLOR : rectColor = UNSELECTED_COLOR;
			RENDERER->drawQuad(transform.getTransform(), RENDERER->normalizeColor(rectColor));
			RENDERER->drawQuad(getComponent<transformComponent>().getTransform(),RENDERER->normalizeColor({ 36.0f,37.0f,38.0f,255.0f }));


			//drawString(lineEdit.charTransforms, color(), lineEdit.font);
			if (lineEdit.font) drawString(lineEdit.font, transform.translation, lineEdit.text, lineEdit.points, color(), lineEdit.bounds);
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
					if (prevHovered)
					{
						platform::os::setCursorShape(platform::ARROW);
					}
					else
					{
						platform::os::setCursorShape(platform::IBEAM);
					}
				}
			}
			if (event.getEventType() == eventType::MouseButtonPressed)
			{
				mouseButtonPressedEvent* mouseEvent = (mouseButtonPressedEvent*)&event;
				if (mouseEvent->getMouseButton() == Mouse::ButtonLeft)
				{
					if (lineEdit.hovered)
					{
						lineEdit.selected = !lineEdit.selected;
					}
					else { lineEdit.selected = false; }
				}
			}
			if (lineEdit.selected && event.getEventType() == eventType::KeyTyped)
			{

				keyTypedEvent* keyBoardEvent = (keyTypedEvent*)&event;
				lineEdit.text += keyBoardEvent->getkeyCode();
				LN_EMIT_SIGNAL("TextChanged", utils::scriptUtils::createMonoString(lineEdit.text));
				calculateTransforms();
			}
			if (lineEdit.selected && event.getEventType() == eventType::KeyPressed)
			{
				keyPressedEvent* keyPressed = (keyPressedEvent*)&event;
				if (keyPressed->getkeyCode() == input::Backspace && lineEdit.text.size())
				{
					lineEdit.text.pop_back();
					LN_EMIT_SIGNAL("TextChanged", utils::scriptUtils::createMonoString(lineEdit.text));
					calculateTransforms();
				}
				if (keyPressed->getkeyCode() == input::Enter)
				{
					LN_EMIT_SIGNAL("TextSubmitted", utils::scriptUtils::createMonoString(lineEdit.text));
				}
			}
			if (lineEdit.selected && event.getEventType() == eventType::MouseScrolled)
			{
				mouseScrolledEvent* scrolledEvent = (mouseScrolledEvent*)&event;
				if (scrolledEvent->getYOffset() < 0) //MOUSE_DOWN
				{
					lineEdit.scrollPosition = CLAMP(lineEdit.scrollPosition + 1, 0, lineEdit.text.size() - 1);
					calculateTransforms();
				}

				if (scrolledEvent->getYOffset() > 0) //MOUSE_UP
				{
					lineEdit.scrollPosition = CLAMP(lineEdit.scrollPosition - 1, 0, lineEdit.text.size() - 1);
					calculateTransforms();
				}
			}
		}

		void lineEditNode::notification(const notificationType type)
		{
			switch (type)
			{
			case luna::TRANSFORM_UPDATED:
				calculateTransforms();
				break;
			default:
				break;
			}
		}


		bool lineEditNode::isHovered()
		{
			auto transform = getComponent<transformComponent>();
			transform.scale += NORMALIZED_BORDER_SIZE;
			glm::vec2 normailizedMousePos = RENDERER->getSceneMousePos() / RENDERER->getSceneDimensions();
			normailizedMousePos.x -= 0.5f;
			normailizedMousePos.y -= 0.5f;

			glm::vec2 leftCorner = { transform.translation.x - transform.scale.x / 2.0f,transform.translation.y - transform.scale.y / 2.0f };
			glm::vec2 rightCorner = { transform.translation.x + transform.scale.x / 2.0f,transform.translation.y + transform.scale.y / 2.0f };
			leftCorner /= 2.0f; //origin coordinates are in center!
			rightCorner /= 2.0f;//origin coordinates are in center!
			bool hover = (leftCorner.x < normailizedMousePos.x && leftCorner.y < normailizedMousePos.y && rightCorner.x > normailizedMousePos.x && rightCorner.y > normailizedMousePos.y);
			return hover;
		}

		void lineEditNode::calculateTransforms()
		{
			auto& lineEdit = getComponent<lineEditComponent>();
			auto transform = getComponent<transformComponent>();
			//transform.translation.x -= transform.scale.x / 2.0f - transform.scale.x * 0.03f;

			float xAdvance = 0.0f;
			if (!lineEdit.font) return;
			const ref<assets::image> spaceGlyph = lineEdit.font->getGlyph('_');
			float pxNorm = lineEdit.points * 1.333; //why the 1.333 (pt to px ,1pt => pw: 1pt * 1.333)
			pxNorm /= RENDERER->getSceneDimensions().y;
			lineEdit.bounds = { transform.translation.x - transform.scale.x / 2.0f + NORMALIZED_BORDER_SIZE,transform.translation.y - transform.scale.y / 2.0f + NORMALIZED_BORDER_SIZE, transform.translation.x + transform.scale.x / 2.0f - NORMALIZED_BORDER_SIZE,transform.translation.y + transform.scale.y / 2.0f - NORMALIZED_BORDER_SIZE };
		}
	}
}