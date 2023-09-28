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
			transformComponent getBaseThumbTrackTransform();
			bool isThumbTrackHovered(transformComponent transform);
			bool isScrollBarHovered();
		};

		class HScrollBar : public scrollBar 
		{
		public:
			HScrollBar() = default;
			HScrollBar(entt::entity handle, luna::scene* scene);
			HScrollBar(luna::scene* scene) {};
			virtual ~HScrollBar() = default;
			virtual void init(luna::scene* scene) override;
		};
		class VScrollBar : public scrollBar 
		{
		public:
			VScrollBar() = default;
			VScrollBar(entt::entity handle, luna::scene* scene);
			VScrollBar(luna::scene* scene) {};
			virtual ~VScrollBar() = default;
			virtual void init(luna::scene* scene) override;
		};
	}
}

