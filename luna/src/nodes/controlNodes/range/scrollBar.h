#pragma once
#include <nodes/controlNodes/range/range.h>
namespace luna 
{
	namespace nodes 
	{
		class scrollBar : public range
		{
		public:
			scrollBar() = default;
			scrollBar(entt::entity handle, luna::scene* scene);
			scrollBar(luna::scene* scene);
			virtual ~scrollBar() = default;
			virtual void init(luna::scene* scene) override;
			virtual void bindMethods() override;
			virtual void guiEvent(Event& event) override;
			virtual void draw() override;
			
		private:
			transformComponent getThumbTrackTransform();
			bool isThumbTrackHovered(transformComponent transform);
		};
	}
}

