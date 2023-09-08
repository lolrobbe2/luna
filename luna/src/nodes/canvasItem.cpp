#include "canvasItem.h"
#include <core/scene/scene.h>
#include <core/rendering/renderer2D.h>
namespace luna 
{
	namespace nodes 
	{
		canvasItem::canvasItem(entt::entity handle, luna::scene* scene) : controlNode(handle,scene)
		{
		
		}
		canvasItem::canvasItem(uint64_t id, luna::scene* scene)
		{
		}
		canvasItem::canvasItem(luna::scene* scene) : controlNode(scene)
		{
			addComponent<canvasComponent>().drawFunction = [&]() { canvasItem(entityHandle, scene).draw(); };

		}

		void canvasItem::drawChar(ref<renderer::font> font, glm::vec2 pos, char chr, int font_size, color modulate) 
		{
			const ref<renderer::texture> charGlyph = font->getGlyph(chr);
			transformComponent transform = getComponent<transformComponent>();
			int pxNorm = PT_TO_PX(font_size) / renderer::renderer::getSceneGuiDimensions().y;
			glm::vec3 position{ pos.x,pos.y,1.0f };
			renderer::renderer2D::drawCharQuad(position, { pxNorm,pxNorm }, charGlyph, renderer::renderer2D::checkHandle(charGlyph->handle()), modulate);
		}

		void canvasItem::drawString(ref<renderer::font> font, glm::vec2 pos, std::string string, int font_size, color modulate)
		{
			transformComponent transform = getComponent<transformComponent>();
			int pxNorm = PT_TO_PX(font_size) / renderer::renderer::getSceneGuiDimensions().y;
			glm::vec3 position{ pos.x,pos.y,1.0f };
			renderer::renderer2D::drawLabel(position, { pxNorm,pxNorm }, font,string);
		}
		void canvasItem::drawTexture(ref<renderer::texture> texture, glm::vec2 position, color modulate = color(1, 1, 1, 1))
		{
			glm::vec3 position3 = { position.x,position.y,0.0f };
			glm::vec2 normSize = { texture->getWidth() / renderer::renderer::getSceneGuiDimensions().x,texture->getHeight() / renderer::renderer::getSceneGuiDimensions().y };
			renderer::renderer2D::drawQuad(position3, normSize, texture); 
		}
		void canvasItem::executeDraw() {
			
		}
	}
}