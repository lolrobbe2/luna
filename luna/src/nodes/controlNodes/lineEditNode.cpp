#include "lineEditNode.h"
#include <core/rendering/renderer2D.h>
#include <core/events/mouseEvent.h>
#include <core/events/keyEvent.h>

#include <core/application.h>
#include <core/debug/typedefs.h>
#include <core/platform/platformUtils.h>

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
			LN_NOTIFICATION_FUNC();
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
			
			drawString(lineEdit.charTransforms, color(), lineEdit.font);
			//if (lineEdit.font) drawString(lineEdit.font, transform2.translation, lineEdit.drawText,16,color(), lineEdit.bounds, lineEdit.indexOutOfBounds);
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
						platform::os::setCursorShape(platform::ARROW);
					}
					else 
					{
						platform::os::setCursorShape(platform::IBEAM);
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
				LN_EMIT_SIGNAL("TextChanged", utils::scriptUtils::createMonoString(lineEdit.text));
				calculateTransforms();
			}
			if (event.getEventType() == eventType::KeyPressed)
			{
				keyPressedEvent* keyPressed = (keyPressedEvent*)&event;
				if (keyPressed->getkeyCode() == input::Backspace && lineEdit.text.size()) 
				{
					lineEdit.text.pop_back();
					LN_EMIT_SIGNAL("TextChanged", utils::scriptUtils::createMonoString(lineEdit.text));
					calculateTransforms();
				}
				if(keyPressed->getkeyCode() == input::Enter)
				{
					LN_EMIT_SIGNAL("TextSubmitted", utils::scriptUtils::createMonoString(lineEdit.text));
				}
			}
			if(event.getEventType() == eventType::MouseScrolled) 
			{
				mouseScrolledEvent* scrolledEvent = (mouseScrolledEvent*)&event;
				if (scrolledEvent->getYOffset() < 0) //MOUSE_DOWN
				{
					lineEdit.scrollPosition = CLAMP(lineEdit.scrollPosition + 1,0,lineEdit.text.size() - 1);
					calculateTransforms();
				}

				if (scrolledEvent->getYOffset() > 0) //MOUSE_UP
				{
					lineEdit.scrollPosition = CLAMP(lineEdit.scrollPosition - 1, 0, lineEdit.text.size() - 1);
					calculateTransforms();
				}	
			}
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

		void lineEditNode::calculateTransforms()
		{
			auto& lineEdit = getComponent<lineEditComponent>();
			auto transform = getComponent<transformComponent>();
			transform.translation.x -= transform.scale.x / 2.0f - transform.scale.x * 0.03f;

			float xAdvance = 0.0f;
			if (!lineEdit.font) return;
			const ref<renderer::texture> spaceGlyph = lineEdit.font->getGlyph('_');
			float pxNorm = lineEdit.points * 1.333;
			pxNorm /= renderer::renderer::getSceneDimensions().y;
			lineEdit.bounds = { transform.translation.x - transform.scale.x - NORMALIZED_BORDER_SIZE,transform.translation.x + transform.scale.x - NORMALIZED_BORDER_SIZE, transform.translation.y - transform.scale.y / 2.0f,transform.translation.y + transform.scale.y / 2.0f };
			const glm::vec2 normalizedDimensions = glm::vec2(pxNorm,pxNorm) / renderer::renderer::getSceneDimensions(); // Calculate normalized dimensions based on size relative to scene dimensions.
			std::vector<lineEditComponent::character> characters; // Vector to store character transforms.
			
			for (size_t i = lineEdit.scrollPosition; i < lineEdit.text.size(); i++)
			{
				const ref<renderer::texture> glyph = lineEdit.font->getGlyph(lineEdit.text[i]);
				const glm::vec2 dimensions = { glyph->getWidth(),glyph->getHeight() };
				const glm::vec2 normalizedCharDimensions = dimensions / renderer::renderer::getSceneDimensions();
				xAdvance += lineEdit.font->getAdvance(lineEdit.text[i]).x * normalizedDimensions.x;

				// Calculate the transformation matrix for the character using the provided quad vertices.
				glm::mat4 transformMat = glm::translate(glm::mat4(1.0f), { xAdvance + transform.translation.x, transform.translation.y + (pxNorm / 2) + lineEdit.font->getAdvance(lineEdit.text[i]).y * normalizedDimensions.y, transform.translation.z })
					* glm::scale(glm::mat4(1.0f), { pxNorm, pxNorm, 1.0f })
					* glm::scale(glm::mat4(1.0f), { normalizedCharDimensions.x, normalizedCharDimensions.y, 1.0f });

				// Check if the character is out of bounds.
				if (isOutOfBounds(transformMat, lineEdit.bounds))
				{
					lineEdit.charTransforms = characters;
					lineEdit.outOfBounds = true;
					return;
				}
				// Store the character's transformation matrix and glyph in the vector.

				characters.emplace_back(transformMat, glyph);

				// Update xAdvance based on character width.
				if (lineEdit.text[i] == ' ')
				{
					xAdvance += spaceGlyph->getWidth() * normalizedDimensions.x;
				}
				else
				{
					xAdvance += glyph->getWidth() * normalizedDimensions.x;
				}

	
			}
			lineEdit.charTransforms = characters;
			lineEdit.outOfBounds = false;
		}
		bool lineEditNode::isOutOfBounds(const glm::mat4 transform,const glm::vec4& bounds)
		{
			glm::vec4 vert[] = {
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
				glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
			};

			for (size_t i = 0; i < 4; i++)
			{
				glm::vec4 transVert = transform * vert[i];
				if((transVert.x < bounds.x || transVert.x > bounds.y) || (transVert.y < bounds.z || transVert.y > bounds.w)) return true;
			}
			return false;
		}
		
	}
}