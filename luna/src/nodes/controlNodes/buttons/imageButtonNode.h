#pragma once
#include <nodes/controlNodes/buttonNode.h>
namespace luna
{
	namespace nodes
	{
		/**
		 * @brief imageButtonNode implementation.
		 */
		class LN_API imageButtonNode : public buttonNode
		{
		public:
			~imageButtonNode() = default;
			imageButtonNode() = default;
			/**
			 * @brief construct node from entityHandle and corresponding scene.
			 *
			 * \param handle
			 * \param scene
			 */
			imageButtonNode(entt::entity handle, luna::scene* scene);
			/**
			 * @brief construct node in scene.
			 *
			 * \param scene
			 */
			imageButtonNode(luna::scene* scene);
			virtual void init(luna::scene* scene) override;
		};
	}
}

