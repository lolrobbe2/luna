#pragma once
#include <core/scene/scene.h>
namespace luna 
{
	namespace nodes
	{
		/**
		 * @brief implementation of spriteNode.
		 */
		class LN_API spriteNode : public Node
		{
		public:
			spriteNode() = default;
			/**
			 * @brief construct node from entityHandle and corresponding scene.
			 * 
			 * \param handle
			 * \param scene
			 */
			spriteNode(entt::entity handle, luna::scene* scene);
			/**
			 * @brief construct node in scene.
			 * 
			 * \param scene
			 */
			spriteNode(luna::scene* scene);
			virtual ~spriteNode() = default;
		};
	}
}


