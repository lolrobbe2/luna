#pragma once
#include <nodes/controlNodes/controlNode.h>
namespace luna 
{
	namespace nodes 
	{
		class lineEditNode : public controlNode
		{
		public:

			lineEditNode() = default;
			/**
			 * @brief construct node from entityHandle and corresponding scene.
			 *
			 * \param handle
			 * \param scene
			 */
			lineEditNode(entt::entity handle, luna::scene* scene);
			/**
			 * @brief construct node in scene.
			 *
			 * \param scene
			 */
			lineEditNode(luna::scene* scene);
			virtual void init(luna::scene* scene) override;
			virtual ~lineEditNode() = default;
		private:
		};
	}
}


