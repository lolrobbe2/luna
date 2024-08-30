#include "canvasItem.h"
#include <core/artemis/renderer.h>
#include <core/application.h>

namespace luna 
{

	namespace nodes 
	{
		canvasItem::canvasItem(entt::entity handle, luna::scene* scene) : Node(handle,scene)
		{
		
		}
		canvasItem::canvasItem(uint64_t id, luna::scene* scene)
		{
		}
		canvasItem::canvasItem(luna::scene* scene) : Node(scene)
		{
			LN_CANVAS_COMPONENT(canvasItem);
		}

		void canvasItem::init(luna::scene* scene)
		{

			Node::init(scene);
			LN_CLASS_TYPE_NAME(canvasItem);
			LN_CANVAS_COMPONENT(canvasItem);
		}

		void canvasItem::drawChar(ref<assets::font> font, glm::vec2 pos, char chr, int font_size, color modulate) 
		{
			
			const ref<assets::image> charGlyph = font->getGlyph(chr);
			transformComponent transform = getComponent<transformComponent>();
			//int pxNorm = PT_TO_PX(font_size) / renderer::renderer::getSceneGuiDimensions().y;
			glm::vec3 position{ pos.x,pos.y,1.0f };
			//renderer::renderer2D::drawCharQuad(position, { pxNorm,pxNorm }, charGlyph, renderer::renderer2D::checkHandle(charGlyph->handle()), modulate);
		}

		void canvasItem::drawChar(const lineEditComponent::character character, ref<assets::font> font, color modulate)
		{
			//renderer::renderer2D::drawCharQuad(character.getTransform(), character.glyph, modulate,font->handle());
		}

		void canvasItem::drawString(ref<assets::font> font, glm::vec2 pos, std::string string, int font_size, color modulate, const glm::vec4& bounds, size_t scrollPosition , bool drawCaret , size_t caretPosition )
		{
			float pxNorm = (font_size * 1.333);
			pxNorm /= application::application::get().getRenderer()->getSceneDimensions().y;
			glm::vec3 position{ pos.x,pos.y,1.0f };
			application::application::get().getRenderer()->drawLabel(position, { pxNorm,pxNorm }, font,string,modulate,bounds,scrollPosition,drawCaret,caretPosition);
		}
		void canvasItem::drawTexture(ref<assets::image> image, glm::vec2 position, color modulate = color(1, 1, 1, 1))
		{
			glm::vec3 position3 = { position.x,position.y,0.0f };
			glm::vec2 normSize = image->getExtent() / application::application::get().getRenderer()->getSceneDimensions();
			application::application::get().getRenderer()->drawQuad(position3, normSize, image);
		}
	}
}