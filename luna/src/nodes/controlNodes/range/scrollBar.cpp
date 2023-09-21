#include "scrollBar.h"
#include <core/events/mouseEvent.h>
#include <core/events/keyEvent.h>
#include <core/debug/typedefs.h>
#include <core/rendering/renderer2D.h>
namespace luna 
{
	namespace nodes 
	{
		scrollBar::scrollBar(entt::entity handle, luna::scene* scene)
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
				if (getComponent<scrollComponent>().active) LN_EMIT_SIGNAL("Scrolling");

			}

			if(event.getEventType() == eventType::MouseScrolled) 
			{
				mouseScrolledEvent* scrolledEvent = (mouseScrolledEvent*)&event;
				if(scrolledEvent < 0 && getComponent<scrollComponent>().active) //MOUSE_DOWN
				{
					double change = getPage() != 0.0 ? getPage() / 4.0 : (getMax() - getMin()) / 16.0;
					setValue(getValue() + MAX(change, getStep()));
				}

				if(scrolledEvent > 0 && getComponent<scrollComponent>().active) //MOUSE_UP
				{
					double change = getPage() != 0.0 ? getPage() / 4.0 : (getMax() - getMin()) / 16.0;
					setValue(getValue() - MAX(change, getStep()));
				}
			}
			if (event.getEventType() == eventType::MouseButtonPressed)
			{
				mouseButtonPressedEvent* mousePressed = (mouseButtonPressedEvent*)&event;
				if (mousePressed->getMouseButton() == Mouse::ButtonLeft) 
				{
					
				}
			}


		}
		void scrollBar::draw()
		{
			LN_ERR_FAIL_COND_MSG(entityHandle == (entt::entity)-1, "invalid entity");
			renderer::renderer2D::drawQuad(getComponent<transformComponent>().getTransform(), {36.0f,37.0f,38.0f,0.8f});
			renderer::renderer2D::drawQuad(getThumbTrackTransform(), { 228.0f,223.0f,235.0f,1.0f });
		}
		glm::mat4 scrollBar::getThumbTrackTransform()
		{
			transformComponent component = getComponent<transformComponent>();
			getComponent<scrollComponent>().orientation ? component.scale.x *= 0.2f : component.scale.x *= 0.9f;
			getComponent<scrollComponent>().orientation ? component.scale.y *= 0.9f : component.scale.y *= 0.2f;

			return component.getTransform();
		}
	}
}

