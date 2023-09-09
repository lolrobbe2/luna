#pragma once
#include <nodes/canvasItem.h>
#ifndef LN_DRAW_LAMBDA
	#define LN_DRAW_LAMBDA(nodeClass) [&]() { nodeClass(entityHandle,scene).draw(); }
#endif
namespace luna 
{
	namespace nodes
	{
		/**
		 * @brief implementation of spriteNode.
		 */
		class LN_API spriteNode : public canvasItem
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
			virtual void init(luna::scene* scene) override;
			virtual ~spriteNode() = default;
			virtual void draw() override;
			void executeDraw();
		};
	}
}


