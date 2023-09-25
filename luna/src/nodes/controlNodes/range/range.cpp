#include "range.h"
#include <core/scripting/scriptingEngine.h>
#include <core/debug/typedefs.h>
#include <core/object/methodDB.h>
namespace luna 
{
	namespace nodes
	{
		

		static void RangeSetAllowGreater(entt::entity ObjectId, bool value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setAllowGreater(value);
		}

		
		static bool RangeIsGreaterAllowed(entt::entity ObjectId) 
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).isGreaterAllowed();
		}

		
		static void RangeSetAllowLesser(entt::entity ObjectId, bool value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setAllowLesser(value);
		}
		
		static bool RangeIsLesserAllowed(entt::entity ObjectId)
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).isLesserAllowed();
		}
		
		static void RangeSetExpRatio(entt::entity ObjectId, bool value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setExpRatio(value);
		}
		
		static bool RangeIsRatioExp(entt::entity ObjectId)
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).isRatioExp();
		}
		
		static void RangeSetMax(entt::entity ObjectId, float value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setAllowGreater(value);
		}
		
		static float RangeGetMax(entt::entity ObjectId) 
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).getMax();
		}
		
		static void RangeSetMin(entt::entity ObjectId, float value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setAllowGreater(value);
		}
		
		static float RangeGetMin(entt::entity ObjectId)
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).getMin();
		}
		
		static void RangeSetPage(entt::entity ObjectId, float value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setPage(value);

		}
		
		static float RangeGetPage(entt::entity ObjectId)
		{
			return 	range(ObjectId, scripting::scriptingEngine::getContext()).getPage();

		}
		
		static void RangeSetAsRatio(entt::entity ObjectId, float value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setAllowGreater(value);
		}
		
		static float RangeGetAsRatio(entt::entity ObjectId)
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).getAsRatio();
		}
		
		static void RangeSetUseRoundedValues(entt::entity ObjectId, bool value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setAllowGreater(value);

		}
		
		static bool RangeIsUsingRoundedvalues(entt::entity ObjectId)
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).isUsingRoundedvalues();

		}
		
		static void RangeSetStep(entt::entity ObjectId, float value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setStep(value);

		}
		
		static float RangeGetStep(entt::entity ObjectId)
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).getStep();
		}
		
		static void RangeSetValue(entt::entity ObjectId, float value) 
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setValue(value);

		}
		
		static float RangeGetValue(entt::entity ObjectId)
		{
			return range(ObjectId, scripting::scriptingEngine::getContext()).getValue();
		}
		
		static void RangeSetValueNoSignal(entt::entity ObjectId, float value)
		{
			range(ObjectId, scripting::scriptingEngine::getContext()).setValueNoSignal(value);
		}



		range::range(entt::entity handle, luna::scene* scene) : controlNode(handle,scene)
		{
		}
		void range::init(luna::scene* scene)
		{
			controlNode::init(scene);
			addComponent<rangeComponent>();
			LN_CLASS_TYPE_NAME(range);
		}
		void range::bindMethods()
		{
			LN_ADD_INTERNAL_CALL(range, RangeSetAllowGreater);
			LN_ADD_INTERNAL_CALL(range, RangeIsGreaterAllowed);

			LN_ADD_INTERNAL_CALL(range, RangeSetAllowLesser);
			LN_ADD_INTERNAL_CALL(range, RangeIsLesserAllowed);

			LN_ADD_INTERNAL_CALL(range, RangeSetExpRatio);
			LN_ADD_INTERNAL_CALL(range, RangeIsRatioExp);

			LN_ADD_INTERNAL_CALL(range, RangeSetMax);
			LN_ADD_INTERNAL_CALL(range, RangeGetMax);

			LN_ADD_INTERNAL_CALL(range, RangeGetMin);
			LN_ADD_INTERNAL_CALL(range, RangeSetMin);

			LN_ADD_INTERNAL_CALL(range, RangeSetPage);
			LN_ADD_INTERNAL_CALL(range, RangeGetPage);

			LN_ADD_INTERNAL_CALL(range, RangeGetAsRatio);
			LN_ADD_INTERNAL_CALL(range, RangeSetAsRatio);

			LN_ADD_INTERNAL_CALL(range, RangeIsUsingRoundedvalues);
			LN_ADD_INTERNAL_CALL(range, RangeSetUseRoundedValues);

			LN_ADD_INTERNAL_CALL(range, RangeGetStep);
			LN_ADD_INTERNAL_CALL(range, RangeSetStep);

			LN_ADD_INTERNAL_CALL(range, RangeGetValue);
			LN_ADD_INTERNAL_CALL(range, RangeSetValue);
			LN_ADD_INTERNAL_CALL(range, RangeSetValueNoSignal);
		}
		void range::setAllowGreater(bool value)
		{
			getComponent<rangeComponent>().allow_greater = value;
		}
		bool range::isGreaterAllowed()
		{
			return getComponent<rangeComponent>().allow_greater;
		}
		void range::setAllowLesser(bool value)
		{
			getComponent<rangeComponent>().allow_lesser = value;
		}
		bool range::isLesserAllowed()
		{
			return getComponent<rangeComponent>().allow_lesser;
		}
		void range::setExpRatio(bool value)
		{
			getComponent<rangeComponent>().exp_ratio = value;
		}
		bool range::isRatioExp()
		{
			return getComponent<rangeComponent>().exp_ratio;
		}
		void range::setMax(float value)
		{
			getComponent<rangeComponent>().max = value;
			LN_EMIT_SIGNAL("Changed");
		}
		float range::getMax()
		{
			return getComponent<rangeComponent>().max;
		}
		void range::setMin(float value)
		{
			if (getComponent<rangeComponent>().min == value) {
				return;
			}

			getComponent<rangeComponent>().min = value;
			getComponent<rangeComponent>().max = MAX(getComponent<rangeComponent>().max, getComponent<rangeComponent>().min);
			getComponent<rangeComponent>().page = CLAMP(getComponent<rangeComponent>().page, 0, getComponent<rangeComponent>().max - getComponent<rangeComponent>().max); 
			LN_EMIT_SIGNAL("Changed");
		}
		float range::getMin()
		{
			return getComponent<rangeComponent>().min;
		}
		void range::setPage(float value)
		{
			double pageValidated = CLAMP(value, 0, getMax() - getMin());
			if (getPage() == pageValidated) {
				return;
			}

			getComponent<rangeComponent>().page = pageValidated;
			setValue(getValue());
		
			LN_EMIT_SIGNAL("Changed");
		}
		float range::getPage()
		{
			return getComponent<rangeComponent>().page;
		}

		void range::setAsRatio(float value)
		{
			double v;

			if (isRatioExp() && getMin() >= 0) {
				double exp_min = getMin() == 0 ? 0.0 : glm::log(getMin()) / glm::log((double)2);
				double exp_max = glm::log(getMax()) / glm::log((double)2);
				v = glm::pow(2, exp_min + (exp_max - exp_min) * value);
			}
			else {
				double percent = (getMax() - getMin()) * value;
				if (getStep() > 0) {
					double steps = round(percent / getStep());
					v = steps * getStep() + getMin();
				}
				else {
					v = percent + getMin();
				}
			}
			v = CLAMP(v, getMin(), getMax());
			setValue(v);
		}

		float range::getAsRatio()
		{
			if (isEqualApprox(getMax(), getMin(),0.5f)) {
				// Avoid division by zero.
				return 1.0;
			}

			if (isRatioExp() && getMin() >= 0) {
				double exp_min = getMin() == 0 ? 0.0 : glm::log(getMin()) / glm::log((double)2);
				double exp_max = glm::log(getMax()) / glm::log((double)2);
				float value = CLAMP(getValue(), getMin(), getMax());
				double v = glm::log(value) / glm::log((double)2);

				return CLAMP((v - exp_min) / (exp_max - exp_min), 0, 1);
			}
			else {
				float value = CLAMP(getValue(), getMin(), getMax());
				return CLAMP((value - getMin()) / (getMax() - getMin()), 0, 1);
			}
		
		}

		void range::setUseRoundedValues(bool value)
		{
			getComponent<rangeComponent>().rounded = value;
		}

		bool range::isUsingRoundedvalues()
		{
			return getComponent<rangeComponent>().rounded;
		}

		void range::setStep(float value)
		{
			if (getStep() == value) return;
			getComponent<rangeComponent>().step = value;
			LN_EMIT_SIGNAL("Changed");
		}

		float range::getStep()
		{
			return getComponent<rangeComponent>().step;
		}

		void range::setValue(float value)
		{
			double prev_val = getValue();
			setValueNoSignal(value);

			if (getValue() != prev_val) {
				LN_EMIT_SIGNAL("ValueChanged", value);
			}
		}

		float range::getValue()
		{
			return getComponent<rangeComponent>().val;
		}

		void range::setValueNoSignal(float value)
		{
			if (!isfinite(value)) {
				return;
			}

			if (getStep() > 0) {
				value = glm::round((value - getMin()) / getStep()) * getStep() + getMin();
			}

			if (isUsingRoundedvalues()) {
				value = glm::round(value);
			}

			if (!isGreaterAllowed() && value > getMax() - getPage()) {
				value = getMax() - getPage();
			}

			if (!isLesserAllowed() && value < getMin()) {
				value = getMin();
			}

			if (getValue() == value) {
				return;
			}

			getComponent<rangeComponent>().val = value;

		}



	}
}
