#include "scrollBar.h"
#include <core/events/mouseEvent.h>
#include <core/events/keyEvent.h>
#include <core/debug/typedefs.h>
#include <core/rendering/renderer2D.h>
namespace luna 
{
	namespace nodes 
	{
		scrollBar::scrollBar(entt::entity handle, luna::scene* scene) : range(handle,scene)
		{
		}
		scrollBar::scrollBar(luna::scene* scene)
		{
		}
		void scrollBar::init(luna::scene* scene)
		{
			range::init(scene);
			LN_CLASS_TYPE_NAME(scrollBar);
			LN_CANVAS_COMPONENT(scrollBar);
			addComponent<scrollComponent>();
			LN_CORE_INFO("node uuid = {0}", (uint32_t)entityHandle);

		}
		void scrollBar::bindMethods()
		{
		}
		void scrollBar::guiEvent(Event& event)
		{
			if (event.getEventType() == eventType::MouseMoved)
			{
				mouseMovedEvent* mouseMoveEvent = (mouseMovedEvent*)&event;
				if (getComponent<scrollComponent>().active) {
					LN_EMIT_SIGNAL("Scrolling");
					static glm::vec2 prevMousePos = renderer::renderer::getSceneMousePos();
					static glm::vec2 normailizedMousePos = (prevMousePos - renderer::renderer::getSceneMousePos()) / renderer::renderer::getSceneDimensions();
					prevMousePos = renderer::renderer::getSceneMousePos();
					normailizedMousePos.x -= 0.5f;
					normailizedMousePos.y -= 0.5f;
					float diff = 0.0f;
					if(getComponent<scrollComponent>().orientation)
					{
						diff = normailizedMousePos.x * 1.25f; //ex: normalizedmouse pos = 0.2f = 1/5, 0.8 = 1/1.25,   (1/5) / (1/1.25f) = (1/5) * 1.25f  (0.8) is the slider distance. 
					} 
					else 
					{
						diff = normailizedMousePos.y * 1.25f;
					}
				}

			}

			if(event.getEventType() == eventType::MouseScrolled) 
			{
				mouseScrolledEvent* scrolledEvent = (mouseScrolledEvent*)&event;
				if(scrolledEvent->getYOffset() < 0 && getComponent<scrollComponent>().active) //MOUSE_DOWN
				{
					
					double change = getPage() != 0.0 ? getPage() / 4.0 : (getMax() - getMin()) / 16.0;
					setValue(getValue() + MAX(change, getStep()));
					
				}

				if(scrolledEvent->getYOffset() > 0 && getComponent<scrollComponent>().active) //MOUSE_UP
				{
					double change = getPage() != 0.0 ? getPage() / 4.0 : (getMax() - getMin()) / 16.0;
					setValue(getValue() - MAX(change, getStep()));
				}
			}
			if (event.getEventType() == eventType::MouseButtonPressed)
			{
				mouseButtonPressedEvent* mousePressed = (mouseButtonPressedEvent*)&event;
				if (mousePressed->getMouseButton() == Mouse::ButtonLeft && isThumbTrackHovered(getThumbTrackTransform()))
				{
					getComponent<scrollComponent>().active = true;
				}
			}
			if(event.getEventType() == eventType::MouseButtonReleased) 
			{
				mouseButtonReleasedEvent* mousePressed = (mouseButtonReleasedEvent*)&event;
				if (mousePressed->getMouseButton() == Mouse::ButtonLeft) { getComponent<scrollComponent>().active = true; }
			}


		}
		void scrollBar::draw()
		{
			LN_ERR_FAIL_COND_MSG(entityHandle == (entt::entity)-1, "invalid entity");
			renderer::renderer2D::drawQuad(getComponent<transformComponent>().getTransform(), {36.0f,37.0f,38.0f,0.8f});
			renderer::renderer2D::drawQuad(getThumbTrackTransform().getTransform(), {228.0f,223.0f,235.0f,1.0f});
		}
		transformComponent scrollBar::getThumbTrackTransform()
		{
			transformComponent component = getComponent<transformComponent>();
			getComponent<scrollComponent>().orientation ? component.scale.x *= 0.1f : component.scale.x *= 0.9f;
			getComponent<scrollComponent>().orientation ? component.scale.y *= 0.9f : component.scale.y *= 0.1f;
			float offset = (getAsRatio() * 0.8f);
			getComponent<scrollComponent>().orientation ? component.translation.x -= 0.4f - offset : component.translation.y -= 0.4f - offset;
			return component;
		}
		bool scrollBar::isThumbTrackHovered(transformComponent transform)
		{
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

