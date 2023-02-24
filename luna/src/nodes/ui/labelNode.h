#pragma once
#include <core/scene/scene.h>
namespace luna
{
	namespace nodes
	{
		/**
		 * @brief label node implementation.
		 */
		class LN_API labelNode : public Node
		{
		public:
			
			labelNode() = default;
			/**
			 * @brief construct node from entityHandle and corresponding scene.
			 *
			 * \param handle
			 * \param scene
			 */
			labelNode(entt::entity handle, luna::scene* scene);
			/**
			 * @brief construct node in scene.
			 *
			 * \param scene
			 */
			labelNode(luna::scene* scene);
			virtual ~labelNode() = default;
		private:

		};
	}
}