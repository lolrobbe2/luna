#pragma once
#include <nodes/controlNodes/spriteNode.h>
namespace luna 
{
	namespace nodes 
	{
		/**
		 * @brief buttonNode implementation.
		 */
		class LN_API buttonNode : public spriteNode
		{
		public:
			~buttonNode() = default;
			buttonNode() = default;
			/**
			 * @brief construct node from entityHandle and corresponding scene.
			 *
			 * \param handle
			 * \param scene
			 */
			buttonNode(entt::entity handle, luna::scene* scene);
			/**
			 * @brief construct node in scene.
			 *
			 * \param scene
			 */
			buttonNode(luna::scene* scene);
			bool isHovered();
			void setToggleMode(bool mode);
			bool getToggleMode();
			void setActionMode(actionMode mode);
			actionMode getActionMode();
			virtual void init(luna::scene* scene) override;
			virtual void guiEvent(Event& event) override;
			virtual void bindMethods() override;
		};
	}
}

