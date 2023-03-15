#pragma once
#include <nodes/controlNodes/buttonNode.h>
#include <nodes/controlNodes/buttons/imageButtonNode.h>
#include <nodes/controlNodes/controlNode.h>
#include <nodes/controlNodes/labelNode.h>
#include <nodes/controlNodes/spriteNode.h>
#include <nodes/controlNodes/itemListNode.h>
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
				LN_CLASS(imageButtonNode, buttonNode);

			}
		};
	}
}


