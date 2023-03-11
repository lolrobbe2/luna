#include "sceneSerializer.h"
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


	
	/*deserialize*/
	static void deSerializeTag(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["tagComponent"]) return node.setName("Node");
		auto tagComponent = serializedNode["tagComponent"];
		node.addComponent<luna::tagComponent>().tag = tagComponent["tag"].as<std::string>();
	
	}
	static void deSerializeParent(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["parentComponent"]) return;
		auto& parentIdComponent = serializedNode["parentComponent"]["parentId"];
		if (!parentIdComponent) return;
		node.addComponent<luna::parentComponent>().parentId = parentIdComponent.as<uint64_t>();
		LN_CORE_INFO("parent: node name: {0}", node.getComponent<tagComponent>().tag);

	}

	static void deSerializeChildren(luna::Node& node, YAML::Node& serializedNode)
	{
		if (!serializedNode["childComponent"]) return;
		node.addComponent<luna::childComponent>();
		node.addComponent<luna::childUintComponent>().childs = serializedNode["childComponent"]["childId's"].as<std::vector<uint64_t>>();
		LN_CORE_INFO("child: node name: {0}", node.getComponent<tagComponent>().tag);
	}
	static void deSerializeNode(luna::Node& node,YAML::Node& serializedNode)
	{
		auto& id = node.addComponent<idComponent>();
		id.id = serializedNode["id"].as<uint64_t>();
		id.typeName = serializedNode["node type"].as<std::string>();
		deSerializeTag(node, serializedNode);
		deSerializeParent(node, serializedNode);
		deSerializeChildren(node, serializedNode);
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
		auto parent = node.getComponent<parentComponent>();
		out << YAML::Key << "parentComponent";
		out << YAML::BeginMap;
		out << YAML::Key << "parentId" << YAML::Value << parent.parentId.getId();
		out << YAML::EndMap;
	}
	static void serializeChildComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<childComponent>()) return;
		auto children = node.getComponent<childComponent>();
		std::vector<uint64_t> id = transformEntity(node, children.childs);
		out << YAML::Key << "childComponent";
		out << YAML::BeginMap;
		out << YAML::Key << "childId's" << YAML::Value << id;
		out << YAML::EndMap;
	}
	static void serializeTag(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<tagComponent>()) return;
		auto tag = node.getComponent<tagComponent>();
		out << YAML::Key << "tagComponent";
		out << YAML::BeginMap; //begin tag map
		out << YAML::Key << "tag" << YAML::Value << tag.tag;
		out << YAML::EndMap; //endTagMap
	}
	static void serializeTransform(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<transformComponent>()) return;
		auto transform = node.getComponent<transformComponent>();
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
		auto spriteRenderer = node.getComponent<spriteRendererComponent>();
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
		auto labelRenderer = node.getComponent<labelRendererComponent>();
		out << YAML::Key << "labelRendererComponent";
		out << YAML::BeginMap; //begin sprite map
		out << YAML::Key << "color" << YAML::Value << labelRenderer.color;
		out << YAML::Key << "filePath" << YAML::Value << labelRenderer.filePath;
		out << YAML::Key << "text" << YAML::Value << labelRenderer.text;
		out << YAML::Key << "tilingFactor" << YAML::Value << labelRenderer.TilingFactor;
		out << YAML::EndMap; //end sprite map
	}

	static void serializeButtonComponent(YAML::Emitter& out, luna::Node& node)
	{
		if (!node.hasComponent<buttonComponent>()) return;
		auto button = node.getComponent<buttonComponent>();
		out << YAML::Key << "buttonComponent";
		out << YAML::BeginMap; //begin button map
		out << YAML::Key << "normalFilePath" << YAML::Value << button.normalFilePath;
		out << YAML::Key << "hoverFilePath" << YAML::Value << button.hoverFilePath;
		out << YAML::Key << "pressedFilePath" << YAML::Value << button.pressedFilePath;
		out << YAML::Key << "shownInEditor" << YAML::Value << button.showInEditor;
		out << YAML::EndMap; // end button map

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
		out << YAML::EndMap;//node id and typename end
		out << YAML::EndMap;//node components end

	}



	luna::scene* sceneSerializer::deSerialize(const std::string& filePath)
	{

		std::ifstream stream(filePath);
		std::stringstream strstream;
		strstream << stream.rdbuf();
		stream.close();
		YAML::Node data = YAML::Load(strstream.str());
		strstream.clear();
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
		}
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
