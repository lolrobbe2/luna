
using System.Runtime.CompilerServices;

namespace Luna
{
    public class ButtonNode : ControlNode
    {
        #region enums
        public enum ActionMode : ushort
        {
            /// <summary>
            /// Require just a press to consider the button clicked.
            /// </summary>
            ACTION_MODE_BUTTON_PRESS = 0,
            /// <summary>
            /// Require a press and a subsequent release before considering the button clicked.
            /// </summary>
            ACTION_MODE_BUTTON_RELEASE = 1 
        }
        #endregion

        #region public methods
        bool IsHovered() { return ButtonNodeGetHovered(ObjectId); }
        #endregion

        #region signals
        /// <summary>
        /// Emitted when the button starts being held down.
        /// </summary>
        [Signal]
        public virtual void ButtonDown() { }
        /// <summary>
        /// Emitted when the button stops being held down.
        /// </summary>
        [Signal]
        public virtual void ButtonUp() { }
        /// <summary>
        /// <para>Emitted when the button is toggled or pressed. This is on <see cref="ButtonDown"></see> if <see cref="ActionMode"/> is
        /// <see cref="ActionMode.ACTION_MODE_BUTTON_PRESS"/> and on <see cref="ButtonUp"/> otherwise.</para>
        /// <para>If you need to know the button's pressed state (and toggle_mode is active), use <see cref="Toggled(bool)"/> instead.</para>
        /// </summary>
        [Signal]
        public virtual void Pressed() { }
        /// <summary>
        /// Emitted when the button was just toggled between pressed and normal states (only if <see cref="ToggleMode"/> is active). The new state is contained in the ButtonPressed argument.
        /// </summary>
        /// <param name="ButtonPressed"></param>
        [Signal]
        public virtual void Toggled(bool ButtonPressed) { }
        #endregion
        #region internal calls
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ButtonNodeSetToggleMode(ulong ObjectId, bool mode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool ButtonNodeGetToggleMode(ulong ObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ButtonNodeSetActionMode(ulong ObjectId, ushort mode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern ushort ButtonNodeGetActionMode(ulong ObjectId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern bool ButtonNodeGetHovered(ulong ObjectId);
        #endregion
        #region dummy fields
        /// <summary>
        /// <para>If true, the button is in toggle mode. Makes the button flip state between pressed and unpressed each time <br/> its area is clicked.</para>   
        /// </summary>
        protected bool ToggleMode { get => ButtonNodeGetToggleMode(ObjectId); set => ButtonNodeSetToggleMode(ObjectId,value); }
        /// <summary>
        /// Determines when the button is considered clicked, one of the <see cref="ActionMode"/> constants.
        /// </summary>
        protected ActionMode Mode { get => (ActionMode)ButtonNodeGetActionMode(ObjectId); set => ButtonNodeSetActionMode(ObjectId, (ushort)value); }
        #endregion
        protected ButtonNode() : base(0) { }
        internal ButtonNode(ulong ObjectId) : base(ObjectId) { }
    }
}
