#pragma once
#include <nodes/controlNodes/controlNode.h>
namespace luna
{
	namespace nodes
	{
		class LN_API colorRectNode : public controlNode
		{
		public:
			colorRectNode() = default;
			colorRectNode(entt::entity handle, luna::scene* scene);
			colorRectNode(luna::scene* scene);
			virtual ~colorRectNode() = default;
			virtual void init(luna::scene* scene) override;
			//virtual void guiEvent(Event& event) override;

			void setColor(const glm::vec4& color);
			glm::vec4 getColor();
		};
	}
}


