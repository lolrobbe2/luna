#include "sceneSerializer.h"
#include <core/vulkan/utils/vulkanAllocator.h>
#include <yaml-cpp/yaml.h>


namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<luna::item>
	{
		static Node encode(const luna::item& item)
		{
			Node node;
			node.push_back(item);
		}
		static bool decode(const Node& node,luna::item& item)
		{
			if (!node.IsMap()) return false;
			item.rectCache;
			item.disabled = node["disabled"].as<bool>();
			item.iconRegion = node["iconRegion"].as<glm::vec2>();
			item.language = node["language"].as<std::string>();
			item.selectable = node["selectable"].as<bool>();
			item.text = node["text"].as<std::string>();
			item.tooltip = node["tooltip"].as<std::string>();
			item.tooltipEnabled = node["tooltipEnabled"].as<bool>();

		}
	};

	template<>
	struct convert<std::vector<luna::item>>
	{
		static Node encode(const std::vector<luna::item>& items)
		{
			Node node;
			for (luna::item item : items)
				node.push_back(item);
		}

		static bool decode(const Node& node, std::vector<luna::item>& items) 
		{
			if (!node.IsSequence())
				return false;
			for (Node subNode : node)
			{
				items.push_back(subNode.as<luna::item>());
			}
			
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<std::vector<uint64_t>>
	{
		static Node encode(const std::vector<uint64_t>& rhs)
		{
			Node node;
			for (auto& entity : rhs) node.push_back(entity);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, std::vector<uint64_t>& rhs)
		{
			if (!node.IsSequence()) return false;
			for (size_t i = 0; i < node.size(); i++)
			{
				rhs.push_back(node[i].as<uint64_t>());
			}
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
	
}


namespace luna
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const std::vector<uint64_t>& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;
		for (uint64_t entity : v)
		{
			out << entity;
		}
		out << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const item::rectangle& v)
	{
		out << YAML::BeginMap; //begin item map
		out << YAML::Key << "start" << YAML::Value << v.start;
		out << YAML::Key << "end" << YAML::Value << v.end;
		out << YAML::Key << "position" << YAML::Value << v.position;
		out << YAML::EndMap;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const item& v)
	{
		out << YAML::BeginMap; //begin item map
		out << YAML::Key << "rectangle" << YAML::Value << v.rectCache;
		out << YAML::Key << "disabled" << YAML::Value << v.disabled;
		out << YAML::Key << "iconRegion" << YAML::Value << v.iconRegion;
		out << YAML::Key << "language" << YAML::Value << v.language;
		out << YAML::Key << "selectable" << YAML::Value << v.selectable;
		out << YAML::Key << "text" << YAML::Value << v.text;
		out << YAML::Key << "tooltip" << YAML::Value << v.tooltip;
		out << YAML::Key << "tooltipEnabled" << YAML::Value << v.tooltipEnabled;
		out << YAML::EndMap;
		return out;
	}
	
	/*deserialize*/
	static void deSerializeTag(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["tagComponent"]) return node.setName("Node");
		auto& tagComponent = serializedNode["tagComponent"];
		node.addComponent<luna::tagComponent>().tag = tagComponent["tag"].as<std::string>();
	}
	static void deSerializeParent(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["parentComponent"]) return;
		auto& parentIdComponent = serializedNode["parentComponent"]["parentId"];
		if (!parentIdComponent) return;
		node.addComponent<luna::parentComponent>().parentId = parentIdComponent.as<uint64_t>();
	}

	static void deSerializeChildren(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["childComponent"]) return;
		node.addComponent<luna::childComponent>();
		node.addComponent<luna::childUintComponent>().childs = serializedNode["childComponent"]["childId's"].as<std::vector<uint64_t>>();
	}

	static void deSerializeTransform(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["transformComponent"]) return;
		auto& transformComponent = serializedNode["transformComponent"];
		auto& transform = node.addComponent<luna::transformComponent>();
		transform.translation = transformComponent["translation"].as<glm::vec3>();
		transform.rotation = transformComponent["rotation"].as<glm::vec3>();
		transform.scale = transformComponent["scale"].as<glm::vec3>();

	}

	static void deSerializeSprite(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["spriteRendererComponent"]) return;
		auto& spriteComponent = serializedNode["spriteRendererComponent"];
		auto& sprite = node.addComponent<spriteRendererComponent>();
		sprite.color = spriteComponent["color"].as<glm::vec4>();
		sprite.filePath = spriteComponent["filePath"].as<std::string>();
		sprite.TilingFactor = spriteComponent["tilingFactor"].as<float>();
		sprite.showInEditor = spriteComponent["shownInEditor"].as<bool>();
		
		if (sprite.filePath.size() && sprite.showInEditor) sprite.texture = renderer::texture::create(sprite.filePath);
	}

	static void deSerializeLabelRenderer(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["labelRendererComponent"]) return;
		auto& labelComponent = serializedNode["labelRendererComponent"];
		auto& label = node.addComponent<labelRendererComponent>();
		label.color = labelComponent["color"].as<glm::vec4>();
		label.filePath = labelComponent["filePath"].as<std::string>();
		label.text = labelComponent["text"].as<std::string>();
		label.TilingFactor = labelComponent["tilingFactor"].as<float>();
		
		if(label.filePath.size()) label.font = renderer::font::create(label.filePath);
	}
	static void deserializeButton(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["buttonComponent"]) return;
		auto& buttonComponent = serializedNode["buttonComponent"];
		auto& button = node.addComponent<luna::buttonComponent>();
		button.normalFilePath = buttonComponent["normalFilePath"].as<std::string>();
		button.hoverFilePath = buttonComponent["hoverFilePath"].as<std::string>();
		button.pressedFilePath = buttonComponent["pressedFilePath"].as<std::string>();
		button.showInEditor = buttonComponent["shownInEditor"].as<bool>();
		
		
		if (button.normalFilePath.size()) button.normalTexture = renderer::texture::create(button.normalFilePath);
		if (button.hoverFilePath.size()) button.hoverTexture = renderer::texture::create(button.hoverFilePath);
		if (button.pressedFilePath.size()) button.pressedTexture = renderer::texture::create(button.pressedFilePath);
		
	}
	static void deserializeColorRect(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["colorrectComponent"]) return;
		auto& colorrectComponent = serializedNode["colorrectComponent"];
		auto& colorRect = node.addComponent<rectComponent>();
		colorRect.color = colorrectComponent["color"].as<glm::vec4>();
	}
	static void deserializeItemList(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["itemListComponent"]) return;
		auto& itemListComponent = serializedNode["itemListComponent"];
		auto& itemList = node.addComponent<luna::itemList>();
		itemList.filePath = itemListComponent["filePath"].as<std::string>();
		itemList.allowReselect = itemListComponent["allowReselect"].as<bool>();
		itemList.allowRmbSelect = itemListComponent["allowRmbSelect"].as<bool>();
		itemList.items = itemListComponent["items"].as<std::vector<luna::item>>();
		if (itemList.filePath.size()) itemList.font = renderer::font::create(itemList.filePath);
	}
	static void deSerializeNode(luna::Node& node,YAML::Node& serializedNode)
	{
		auto& id = node.addComponent<idComponent>();
		id.id = serializedNode["id"].as<uint64_t>();
		id.typeName = serializedNode["node type"].as<std::string>();
		deSerializeTag(node, serializedNode);
		deSerializeParent(node, serializedNode);
		deSerializeChildren(node, serializedNode);
		deSerializeTransform(node, serializedNode);
		deSerializeSprite(node, serializedNode);
		deSerializeLabelRenderer(node, serializedNode);
		deserializeButton(node, serializedNode);
		deserializeItemList(node, serializedNode);
		deserializeColorRect(node,serializedNode);
	}

	/*serialize*/
	static std::vector<uint64_t> transformEntity(luna::Node& node, const std::vector<entt::entity>& entitys)
	{
		std::vector<uint64_t> entityId;
		scene* scene = node;
		for (entt::entity entity : entitys)
		{
			Node node{ entity,scene };
			entityId.push_back(node.getUUID());
		}
		return entityId;
	}

	static void serializeParentComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<parentComponent>()) return;
		auto& parent = node.getComponent<parentComponent>();
		out << YAML::Key << "parentComponent";
		out << YAML::BeginMap;
		out << YAML::Key << "parentId" << YAML::Value << parent.parentId.getId();
		out << YAML::EndMap;
	}
	static void serializeChildComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<childComponent>()) return;
		auto& children = node.getComponent<childComponent>();
		std::vector<uint64_t> id = transformEntity(node, children.childs);
		out << YAML::Key << "childComponent";
		out << YAML::BeginMap;
		out << YAML::Key << "childId's" << YAML::Value << id;
		out << YAML::EndMap;
	}
	static void serializeTag(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<tagComponent>()) return;
		auto& tag = node.getComponent<tagComponent>();
		out << YAML::Key << "tagComponent";
		out << YAML::BeginMap; //begin tag map
		out << YAML::Key << "tag" << YAML::Value << tag.tag;
		out << YAML::EndMap; //endTagMap
	}
	static void serializeTransform(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<transformComponent>()) return;
		auto& transform = node.getComponent<transformComponent>();
		out << YAML::Key << "transformComponent";
		out << YAML::BeginMap; //begin tag map
		out << YAML::Key << "translation" << YAML::Value << transform.translation;
		out << YAML::Key << "rotation" << YAML::Value << transform.rotation;
		out << YAML::Key << "scale" << YAML::Value << transform.scale;
		out << YAML::EndMap;
	}
	
	static void serializeSpriteRendererComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<spriteRendererComponent>()) return;
		auto& spriteRenderer = node.getComponent<spriteRendererComponent>();
		out << YAML::Key << "spriteRendererComponent";
		out << YAML::BeginMap; //begin sprite map
		out << YAML::Key << "color" << YAML::Value << spriteRenderer.color;
		out << YAML::Key << "filePath" << YAML::Value << spriteRenderer.filePath;
		out << YAML::Key << "tilingFactor" << YAML::Value << spriteRenderer.TilingFactor;
		out << YAML::Key << "shownInEditor" << YAML::Value << spriteRenderer.showInEditor;
		out << YAML::EndMap; //end sprite map
	}
	static void serializeLabelRendererComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<labelRendererComponent>()) return;
		auto& labelRenderer = node.getComponent<labelRendererComponent>();
		out << YAML::Key << "labelRendererComponent";
		out << YAML::BeginMap; //begin sprite map
		out << YAML::Key << "color" << YAML::Value << labelRenderer.color;
		out << YAML::Key << "filePath" << YAML::Value << labelRenderer.filePath;
		out << YAML::Key << "text" << YAML::Value << labelRenderer.text;
		out << YAML::Key << "tilingFactor" << YAML::Value << labelRenderer.TilingFactor;
		out << YAML::EndMap; //end sprite map
	}
	static void serializeColorRect(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<rectComponent>()) return;
		auto colorrect= node.getComponent<rectComponent>();
		out << YAML::Key << "colorrectComponent";
		out << YAML::BeginMap; //begin colorrect map
		out << YAML::Key << "color" << YAML::Value << colorrect.color;
		out << YAML::EndMap; //end colorrect map
	}
	static void serializeButtonComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<buttonComponent>()) return;
		auto& button = node.getComponent<buttonComponent>();
		out << YAML::Key << "buttonComponent";
		out << YAML::BeginMap; //begin button map
		out << YAML::Key << "normalFilePath" << YAML::Value << button.normalFilePath;
		out << YAML::Key << "hoverFilePath" << YAML::Value << button.hoverFilePath;
		out << YAML::Key << "pressedFilePath" << YAML::Value << button.pressedFilePath;
		out << YAML::Key << "shownInEditor" << YAML::Value << button.showInEditor;
		out << YAML::EndMap; // end button map

	}
	static void serializeItemListComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<itemList>()) return;
		auto& itemList = node.getComponent<luna::itemList>();
		out << YAML::Key << "itemListComponent";
		out << YAML::BeginMap; //begin itemList map
		out << YAML::Key << "filePath" << YAML::Value << itemList.filePath;
		out << YAML::Key << "allowReselect" << YAML::Value << itemList.allowReselect;
		out << YAML::Key << "allowRmbSelect" << YAML::Value << itemList.allowRmbSelect;
		out << YAML::Key << "items" << YAML::Value << YAML::BeginSeq;
		for (item& item : itemList.items)
		{
			out << item;
		}
		out << YAML::EndSeq;
 		out << YAML::EndMap; // end itemList map
	}
	static void serializeNode(YAML::Emitter& out,luna::Node& node)
	{
		
		out << YAML::BeginMap; //node components start
		out << YAML::Key << "Node";
		out << YAML::BeginMap; //node id and typename start
		out << YAML::Key << "id" << YAML::Value << node.getComponent<idComponent>().id.getId();
		out << YAML::Key << "node type" << YAML::Value << node.getComponent<idComponent>().typeName;
		serializeTag(out, node);
		serializeParentComponent(out, node);
		serializeChildComponent(out, node);
		serializeTransform(out, node);
		serializeSpriteRendererComponent(out, node);
		serializeLabelRendererComponent(out, node);
		serializeButtonComponent(out, node);
		serializeColorRect(out, node);
		serializeItemListComponent(out, node);
		out << YAML::EndMap;//node id and typename end
		out << YAML::EndMap;//node components end

	}



	luna::scene* sceneSerializer::deSerialize(const std::string& filePath)
	{
		LN_PROFILE_SCOPE("loading and deserializing");
		YAML::Node data = YAML::LoadFile(filePath);//YAML::Load(strstream.str());

		
		if (!data["scene"]) return nullptr;

		scene* scene = new luna::scene();

		std::string sceneName = data["scene"].as<std::string>();
		LN_CORE_INFO("deSerializing scene {0}", sceneName);
		auto& nodes = data["nodes"];
		for (auto& serializedNode : nodes)
		{
			entt::entity entity = scene->create();
			Node node{ entity,scene };
			deSerializeNode(node, serializedNode["Node"]);
		}
	
		LN_PROFILE_SCOPE("node tree assembly");
	
		auto childGroup = scene->m_Registry.view<childUintComponent>();
		for (auto& entity : childGroup)
		{
			Node parent{ entity,scene };
			auto children = parent.getComponent<childUintComponent>().childs;
			for(uint64_t childId : children)
			{
				Node child{ childId, scene };
				parent.addChild(child);
			}
			parent.removeComponent<childUintComponent>();
			childGroup = scene->m_Registry.view<childUintComponent>();
		}
		LN_CORE_INFO("finished deserializing scene");
		return scene;
	}

	bool sceneSerializer::serialize(scene* scene, const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "scene" << YAML::Value << "name";
		out << YAML::Key << "nodes" << YAML::Value << YAML::BeginSeq;
		scene->m_Registry.each([&](auto entity)
			{
				Node node = { entity,scene };
				if (!node) return;
				serializeNode(out, node);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fileOut(filePath);
		fileOut << out.c_str();
		return false;
	}
}
