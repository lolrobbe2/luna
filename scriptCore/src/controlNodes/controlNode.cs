using System.Runtime.CompilerServices;

namespace Luna
{
    public class ControlNode : Node
    {
        #region public methods
        protected Vector2 GetScale() { return Scale;}
        protected void SetScale(Vector2 scale) { Scale = scale; }
        protected Vector2 GetPosition() { return Position; }
        protected void SetPosition(Vector2 position) { ControlNodeSetPosition(NodeId, position); }
        protected void WarpMouse(Vector2 position) { ControlNodeWarpMouse(NodeId, position); }
        #endregion

        #region internal calls
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Vector2 ControlNodeGetPosition(ulong NodeId,out Vector2 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeSetPosition(ulong NodeId, Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeGetScale(ulong NodeId,out Vector2 vector);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeSetScale(ulong NodeId,ref Vector2 Offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeWarpMouse(ulong NodeId, Vector2 position);
        #endregion

        #region dummy fields 
        protected Vector2 Position { get { ControlNodeGetPosition(NodeId,out Vector2 Translation); return Translation; } set => SetPosition(value); }
        protected Vector2 Scale { get { ControlNodeGetScale(NodeId, out Vector2 Scale);return Scale; } set => ControlNodeSetScale(NodeId, ref value); }
        #endregion
        protected ControlNode() { }
        internal ControlNode(ulong NodeId) : base(NodeId) { }
    }
}
