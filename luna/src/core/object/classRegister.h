#pragma once
#include <nodes/controlNodes/buttonNode.h>
#include <nodes/controlNodes/buttons/imageButtonNode.h>
#include <nodes/controlNodes/lineEditNode.h>
#include <nodes/controlNodes/labelNode.h>
#include <nodes/controlNodes/spriteNode.h>
#include <nodes/controlNodes/itemListNode.h>
#include <nodes/controlNodes/colorRectNode.h>
#include <nodes/controlNodes/range/range.h>
#include <nodes/controlNodes/range/scrollBar.h>
namespace luna
{
	namespace nodes
	{
		class classRegister
		{
		public:
			_ALWAYS_INLINE_ static void registerClasses()
			{
				LN_REGISTER_CLASS(canvasItem, Node);
				LN_REGISTER_CLASS(controlNode, canvasItem);
				LN_REGISTER_CLASS(labelNode, controlNode);
				LN_REGISTER_CLASS(spriteNode, controlNode);
				LN_REGISTER_CLASS(buttonNode, controlNode);
				LN_REGISTER_CLASS(itemListNode, controlNode);
				LN_REGISTER_CLASS(colorRectNode, controlNode);
				LN_REGISTER_CLASS(lineEditNode, controlNode);
				LN_REGISTER_CLASS(imageButtonNode, buttonNode);

				LN_REGISTER_CLASS(range, controlNode);
				LN_REGISTER_CLASS(scrollBar,range);
				LN_REGISTER_CLASS(HScrollBar, scrollBar);
				LN_REGISTER_CLASS(VScrollBar, scrollBar);

			}
		};
	}
}


