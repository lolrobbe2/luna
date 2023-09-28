using Luna;
using System;
using System.Runtime.CompilerServices;

namespace Luna
{
    public class Range : ControlNode
    {
        #region public methods
        protected void SetAllowGreater(bool value) { RangeSetAllowGreater(ObjectId, value); }
        protected bool IsGreaterAllowed() { return RangeIsGreaterAllowed(ObjectId); }

        protected void SetAllowLesser(bool value) { RangeSetAllowLesser(ObjectId, value); }
        protected bool IsLesserAllowed() { return RangeIsLesserAllowed(ObjectId); }

        protected void SetExpRatio(bool value) { RangeSetExpRatio(ObjectId, value); }
        protected bool GetExpRatio() { return RangeIsRatioExp(ObjectId); }

        protected void SetMaxValue(float value) { RangeSetMax(ObjectId, value); }
        protected float GetMaxValue() { return RangeGetMax(ObjectId); }

        protected void SetMinValue(float value) { RangeSetMin(ObjectId, value); }
        protected float GetMinValue() { return RangeGetMin(ObjectId); }

        protected void SetPage(float value) { RangeSetPage(ObjectId, value); }
        protected float GetPage() { return RangeGetPage(ObjectId); }


        #endregion

        #region Signals
        /// <summary>
        /// Emitted when MinValue, MaxValue, Page, or Step change.
        /// </summary>
        [Signal]
        public virtual void Changed() { }
        /// <summary>
        /// Emitted when value changes. When used on a Slider, this is called continuously while dragging (potentially every frame). If you are performing an expensive operation in a function connected to value_changed, consider using a debouncing Timer to call the function less often.
        /// </summary>
        /// <remarks>
        /// Unlike signals such as LineEdit.text_changed, value_changed is also emitted when value is set directly via code.
        /// </remarks>
        /// <param name="value"></param>
        [Signal]
        public virtual void ValuecChanged(float value) { }
        #endregion

        #region Dummy Fields
        /// <summary>
        /// If true, value may be greater than MaxValue.
        /// </summary>
        protected bool AllowGreater { get => RangeIsGreaterAllowed(ObjectId); set => RangeSetAllowGreater(ObjectId, value); }
        /// <summary>
        /// If true, value may be less than MinValue.
        /// </summary>
        protected bool AllowLesser { get => RangeIsLesserAllowed(ObjectId); set => RangeSetAllowLesser(ObjectId, value); }
        /// <summary>
        /// If true, and minValue is greater than 0, value will be represented exponentially rather than linearly.
        /// </summary>
        protected bool ExpEdit { get => RangeIsRatioExp(ObjectId); set => RangeSetExpRatio(ObjectId, value); }
        /// <summary>
        /// Maximum value. Range is clamped if value is greater than MaxValue.
        /// </summary>
        protected float MaxValue { get => RangeGetMax(ObjectId); set => RangeSetMax(ObjectId, value); }
        /// <summary>
        /// Minimum value. Range is clamped if value is less than MinVlue.
        /// </summary>
        protected float MinValue { get => RangeGetMin(ObjectId); set => RangeSetMin(ObjectId, value); }
        /// <summary>
        /// Page size. Used mainly for ScrollBar. ScrollBar's length is its size multiplied by page over the difference between MinValue and MaxValue.
        /// </summary>
        protected float Page { get => RangeGetPage(ObjectId); set => RangeSetPage(ObjectId, value); }
        /// <summary>
        /// The value mapped between 0 and 1.
        /// </summary>
        protected float Ratio { get => RangeGetAsRatio(ObjectId); set => RangeSetAsRatio(ObjectId, value); }
        /// <summary>
        /// If greater than 0, value will always be rounded to a multiple of this property's value. If rounded is also true, value will first be rounded to a multiple of this property's value, then rounded to the nearest integer.
        /// </summary>
        protected float Step { get => RangeGetStep(ObjectId); set => RangeSetStep(ObjectId, value); }
        /// <summary>
        /// Range's current value. Changing this property (even via code) will trigger <see cref="Changed"/> signal. Use SetValueNoSignal if you want to avoid it.
        /// </summary>
        protected float Value { get => RangeGetValue(ObjectId);set => RangeSetValue(ObjectId, value); }
        #endregion
        #region internal calls
        [MethodImplAttribute(MethodImplOptions.InternalCall)]

        static extern void RangeSetAllowGreater(ulong ObjectId, bool value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool RangeIsGreaterAllowed(ulong ObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetAllowLesser(ulong ObjectId,bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool RangeIsLesserAllowed(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetExpRatio(ulong ObjectId,bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool RangeIsRatioExp(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetMax(ulong ObjectId,float value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float RangeGetMax(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetMin(ulong ObjectId, float value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float RangeGetMin(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetPage(ulong ObjectId, float value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float RangeGetPage(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetAsRatio(ulong ObjectId, float value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float RangeGetAsRatio(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetUseRoundedValues(ulong ObjectId, bool value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool RangeIsUsingRoundedvalues(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetStep(ulong ObjectId, float value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float RangeGetStep(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetValue(ulong ObjectId, float value);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern float RangeGetValue(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void RangeSetValueNoSignal(ulong ObjectId, float value);
        #endregion
        #region dummy fields

        #endregion
        protected Range() { }
        internal Range(ulong ObjectId) : base(ObjectId) { }
    }
}
