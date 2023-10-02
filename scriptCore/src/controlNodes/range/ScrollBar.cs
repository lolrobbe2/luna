namespace Luna
{
    /// <summary>
    /// Abstract base class for scrollbars, typically used to navigate through content that extends beyond the visible area of a control. Scrollbars are Range-based controls.
    /// </summary>
    public class ScrollBar : Range
    {
        /// <summary>
        /// Emitted when the scrollbar is being scrolled.
        /// </summary>
        [Signal]
        public virtual void Srolling() { }
        protected ScrollBar() { }
        internal ScrollBar(ulong ObjectId) : base(ObjectId) { }
    }
}
