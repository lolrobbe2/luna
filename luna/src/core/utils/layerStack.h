#pragma once
#include <core/utils/layer.h>

#include <vector>

namespace luna 
{
	namespace utils
	{

		class layerStack
		{
		public:
			layerStack() = default;
			~layerStack();

			void pushLayer(layer* layer);
			void pushOverlay(layer* overlay);
			void popLayer(layer* layer);
			void popOverlay(layer* overlay);

			std::vector<layer*>::iterator begin() { return m_Layers.begin(); }
			std::vector<layer*>::iterator end() { return m_Layers.end(); }
			std::vector<layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
			std::vector<layer*>::reverse_iterator rend() { return m_Layers.rend(); }

			std::vector<layer*>::const_iterator begin() const { return m_Layers.begin(); }
			std::vector<layer*>::const_iterator end()	const { return m_Layers.end(); }
			std::vector<layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
			std::vector<layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
		private:
			std::vector<layer*> m_Layers;
			unsigned int m_LayerInsertIndex = 0;
		};
	}
}
