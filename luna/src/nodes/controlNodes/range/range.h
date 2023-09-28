#pragma once
#include <nodes/controlNodes/controlNode.h>
namespace luna 
{
	namespace nodes 
	{
		class LN_API range : public controlNode
		{
		public:
			range() = default;
			range(entt::entity handle, luna::scene* scene);
			range(luna::scene* scene);
			virtual ~range() = default;
			virtual void init(luna::scene* scene) override;
			virtual void bindMethods() override;

			//GODOT based functions
			void setAllowGreater(bool value);
			bool isGreaterAllowed();

			void setAllowLesser(bool value);
			bool isLesserAllowed();

			void setExpRatio(bool value);
			bool isRatioExp();

			void setMax(float value);
			float getMax();

			void setMin(float value);
			float getMin();

			void setPage(float value);
			float getPage();

			void setAsRatio(float value);
			float getAsRatio();

			void setUseRoundedValues(bool value);
			bool isUsingRoundedvalues();

			void setStep(float value);
			float getStep();

			void setValue(float value);
			float getValue();

			void setValueNoSignal(float value);
		private:

		};

	}
}