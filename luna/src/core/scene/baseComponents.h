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
	struct idComponent
	{
		uuid id;
		std::string typeName;
		operator uint64_t() { return id; }
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
		std::string className;
		int currentItem = -1;
		utils::scriptInstance* scritpInstance;
		scriptComponent() = default;
		scriptComponent(const scriptComponent&) = default;
	};

	struct signalComponent 
	{
		std::unordered_map<std::string, std::vector<connectedSignal>> connectedSignals;
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

		glm::mat4 getTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), scale);
		}
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
	
	struct item {
		struct rectangle
		{
			//origin is in center
			glm::vec2 start = glm::vec2(0.0f); //left top corner
			glm::vec2 end = glm::vec2(0.0f); //right bottom corner

			glm::vec2 position;
			_ALWAYS_INLINE_ bool hasPoint(const glm::vec2& point) { return (start.x == point.x || end.x == point.x) && (start.y == point.y || end.y == point.y); };
			_ALWAYS_INLINE_ float width() { return end.x - start.x; };
			_ALWAYS_INLINE_ float height() { return end.y - start.y; };

			_ALWAYS_INLINE_ void setWidth(float width) { end.x -= width / 2; start.x += width / 2; };
			_ALWAYS_INLINE_ float distanceTo(const glm::vec2& pos) { return glm::length(position - pos); };
		};
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
