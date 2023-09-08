#pragma once
#include <nodes/controlNodes/buttonNode.h>
#include <nodes/controlNodes/buttons/imageButtonNode.h>
#include <nodes/controlNodes/lineEditNode.h>
#include <nodes/controlNodes/labelNode.h>
#include <nodes/controlNodes/spriteNode.h>
#include <nodes/controlNodes/itemListNode.h>
#include <nodes/controlNodes/colorRectNode.h>
namespace luna
{
	namespace nodes
	{
		class classRegister
		{
		public:
			_ALWAYS_INLINE_ static void registerClasses()
			{
				LN_CLASS(controlNode, Node);
				LN_CLASS(labelNode, controlNode);
				LN_CLASS(spriteNode, controlNode);
				LN_CLASS(buttonNode, controlNode);
				LN_CLASS(itemListNode, controlNode);
				LN_CLASS(colorRectNode, controlNode);
				LN_CLASS(lineEditNode, controlNode);
				LN_CLASS(imageButtonNode, buttonNode);


			}
		};
	}
}


