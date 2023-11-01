#pragma once
#include "lnpch.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <core/debug/uuid.h>
#include <core/rendering/texture.h>
#include <core/scripting/scriptUtils.h>
namespace luna
{
	enum notificationType
	{
		TRANSFORM_UPDATED
	};
	struct idComponent
	{
		uuid id;
		std::string typeName;
		operator uint64_t() { return id; }
		std::function<void(notificationType)> notificationFunc = [](notificationType type) {};
		idComponent() = default;
		idComponent(const idComponent&) = default;
	};

	struct tagComponent
	{
		std::string tag = "Node";

		tagComponent() = default;
		tagComponent(const tagComponent&) = default;
		tagComponent(const std::string& tag)
			: tag(tag) {}
	};

	struct scriptComponent
	{
		std::string className = "noot noot";
		int currentItem = -1;
		utils::scriptInstance* scritpInstance = nullptr;
		scriptComponent() = default;
		scriptComponent(const scriptComponent&) = default;
	};

	struct signalComponent 
	{
		std::unordered_map<std::string, std::vector<connectedSignal>> connectedSignals;
	};

	struct eventComponent
	{
		std::function<void(Event&)> guiEvent;
	};
	struct transformComponent
	{
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

		transformComponent() = default;
		transformComponent(const transformComponent&) = default;
		transformComponent(const glm::vec3& translation)
			: translation(translation) {}

		void setTranslation(const glm::vec3& newTranslation)
		{
			translation = newTranslation;
			recalculateTransform();
		}

		void setRotation(const glm::vec3& newRotation)
		{
			rotation = newRotation;
			recalculateTransform();
		}

		void setScale(const glm::vec3& newScale)
		{
			scale = newScale;
			recalculateTransform();
		}

		// Private method to recalculate the transform matrix
		void recalculateTransform()
		{
			transformMatrix = glm::mat4(1.0f); // Initialize as identity matrix
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

			transformMatrix = translationMatrix * glm::toMat4(glm::quat(rotation)) * scaleMatrix;
		}

		// Getter method for the transform matrix
		glm::mat4 getTransform() const
		{
			return transformMatrix;
		}

	private:
		// Store the transform matrix internally
		glm::mat4 transformMatrix = glm::mat4(1.0f); // Initialize as identity matrix
	};

	struct spriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		ref<renderer::texture> texture;
		std::filesystem::path filePath;
		float TilingFactor = 1.0f;
		bool showInEditor = true;
		bool outOfBounds = false;

		spriteRendererComponent() = default;
		spriteRendererComponent(const spriteRendererComponent&) = default;
		spriteRendererComponent(const glm::vec4& color)
			: color(color) {}
	};
	namespace nodes{class canvasItem;}
	struct canvasComponent
	{
		std::function<void()> drawFunction;
		glm::vec4 modulate { 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec4 selfModulate { 1.0f, 1.0f, 1.0f, 1.0f };
		bool visible = true;
	};

	struct rectComponent
	{
		glm::vec4 color{ 1.0f,1.0f,1.0f,1.0f };
	};
	struct labelRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		ref<renderer::font> font;
		uint64_t handle;
		std::filesystem::path filePath;
		std::string text;
		float TilingFactor = 1.0f;
		glm::vec2 pos;
		int fontSize;
		labelRendererComponent() = default;
		labelRendererComponent(const labelRendererComponent&) = default;
		labelRendererComponent(const std::string& text)
			: text(text) {}
	};
	enum actionMode : uint16_t
	{
		/// <summary>
		/// Require just a press to consider the button clicked.
		/// </summary>
		ACTION_MODE_BUTTON_PRESS = 0,
		/// <summary>
		/// Require a press and a subsequent release before considering the button clicked.
		/// </summary>
		ACTION_MODE_BUTTON_RELEASE = 1
	};
	struct buttonComponent
	{
		ref<renderer::texture> normalTexture; 
		ref<renderer::texture> hoverTexture;
		ref<renderer::texture> pressedTexture;
		std::filesystem::path normalFilePath;
		std::filesystem::path hoverFilePath;
		std::filesystem::path pressedFilePath;

		bool hover = 0;//release/no hover = 0, hover 1;  
		bool pressed = 0;
		bool showInEditor = true;
		bool toggleMode = false;
		actionMode actionMode = ACTION_MODE_BUTTON_RELEASE;
		buttonComponent() = default;
		buttonComponent(const buttonComponent&) = default;
	};
	
	struct scrollComponent 
	{
		bool orientation; //vertical = true,horizontal = false.

		bool hover = false;
		bool pressed = false;
		bool active = false;
		bool scrolling = false;
		double target_scroll = 0.0;
		bool smooth_scroll_enabled = false;
	};
	struct rectangle
	{
		//origin is in center
		glm::vec2 start = glm::vec2(0.0f); //left top corner
		glm::vec2 end = glm::vec2(0.0f); //right bottom corner

		glm::vec2 position = glm::vec2(0.0f);
		_ALWAYS_INLINE_ bool hasPoint(const glm::vec2& point) { return (start.x == point.x || end.x == point.x) && (start.y == point.y || end.y == point.y); };
		_ALWAYS_INLINE_ float width() { return end.x - start.x; };
		_ALWAYS_INLINE_ float height() { return end.y - start.y; };

		_ALWAYS_INLINE_ void setWidth(float width) { end.x -= width / 2; start.x += width / 2; };
		_ALWAYS_INLINE_ float distanceTo(const glm::vec2& pos) { return glm::length(position - pos); };
		_ALWAYS_INLINE_ glm::mat4 getTransform()
		{
			// Calculate translation matrix
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));

			// Calculate scaling matrix
			glm::mat4 scaling = glm::scale(glm::mat4(1.0f), glm::vec3(width(), height(), 1.0f));

			// Combine translation and scaling to get the transformation matrix
			glm::mat4 transform = translation * scaling;

			return transform;
		}
	};

	struct item {
		
		ref<renderer::texture> icon;
		bool iconTransposed = false;
		glm::vec2 iconRegion;
		glm::vec4 iconModulate = { 1, 1, 1, 1 };
		ref<renderer::texture> tagIcon;
		std::string text;
		std::string language;

		bool selectable = true;
		bool selected = false;
		bool disabled = false;
		bool tooltipEnabled = true;
		
		std::string tooltip;
		glm::vec4 customFg;
		glm::vec4 customBg = { 0.0, 0.0, 0.0, 0.0 };

		bool hover = 0;

		rectangle rectCache;
		rectangle minRectCache;

		glm::vec2 getIconSize() const;

		bool operator<(const item& p_another) const { return text < p_another.text; }

		item() {
			//meep meep
		}
	};
	
	struct itemList 
	{
		enum iconMode {
			ICON_MODE_TOP,
			ICON_MODE_LEFT
		};

		enum selectMode {
			SELECT_SINGLE,
			SELECT_MULTI
		};
		std::vector<item> items;
		std::vector<int> separators;
		std::unordered_set<int> selected;
		std::string searchString;

		selectMode selectMode = SELECT_SINGLE;
		iconMode iconMode = ICON_MODE_LEFT;

		int current = -1;
		int ensureSelectedVisible = 0;
		
		int deferSelectSingle = -1;

		bool shapeChanged = false;
		bool allowRmbSelect = false;
		bool allowReselect = false;

		int currentColumns = 1;

		ref<renderer::font> font;
		std::filesystem::path filePath;

		itemList() = default;
		itemList(const itemList&) = default;
	};
	struct rangeComponent {
		double val = 0.0;
		double min = 0.0;
		double max = 100.0;
		double step = 0.01;
		double page = 0.0;
		bool exp_ratio = false;
		bool allow_greater = false;
		bool allow_lesser = false;
		bool rounded = false;
		std::unordered_set<entt::entity> owners;
	};

	struct lineEditComponent
	{
		struct character
		{
			ref<renderer::texture> glyph;

			character() = default;
			character(const character&) = default;
			character(glm::mat4 transform,ref<renderer::texture> glyph)
				: transformMatrix(transform), glyph(glyph) {
			}

			
			// Getter method for the transform matrix
			glm::mat4 getTransform() const
			{
				return transformMatrix;
			}

		private:
			// Store the transform matrix internally
			glm::mat4 transformMatrix = glm::mat4(1.0f); // Initialize as identity matrix
		};
		uint8_t caretPosition;
		uint8_t scrollPosition = 0;
		std::string text;
		std::string drawText;
		std::filesystem::path filePath;
		bool selected = false;
		bool hovered = false;
		bool outOfBounds = false;
		uint8_t points = 16;
		glm::vec4 bounds;
		glm::mat4 outerBorderTransform;
		std::vector<character> charTransforms;
		ref<renderer::font> font;
	};
	/*
		node tree components:
		these components represent the node tree
	*/

	/**
	 * @brief represents a node that has a parent node.
	 */
	struct parentComponent
	{
		uuid parentId;
	};
	/**
	 * represents a node that has a root component.
	 */
	struct childComponent
	{
		std::vector<entt::entity> childs;
	};
	struct childUintComponent
	{
		std::vector<uint64_t> childs;
	};
}
