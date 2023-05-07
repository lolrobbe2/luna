using System.Runtime.CompilerServices;

namespace Luna
{
    public class ControlNode : Node
    {
        protected Vector2 GetScale() { return Scale;}
        protected void SetScale(Vector2 scale) { Scale = scale; }
        protected Vector2 GetPosition() { return Position; }
        protected void SetPosition(Vector2 position) { ControlNodeSetPosition(NodeId, position); }
        protected Vector2 GetSize() { return Size; }
        protected void SetSize(Vector2 size) {  Size = size; }
        protected void WarpMouse(Vector2 position) { ControlNodeWarpMouse(NodeId, position); }
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
        static extern void ControlNodeGetSize(ulong NodeId,out Vector2 vector);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeSetSize(ulong NodeId,ref Vector2 Size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeWarpMouse(ulong NodeId, Vector2 position);
        #endregion

        #region dummy fields 
        protected Vector2 Position { get { ControlNodeGetPosition(NodeId,out Vector2 Translation); return Translation; } set => SetPosition(value); }
        protected Vector2 Scale { get { ControlNodeGetScale(NodeId, out Vector2 Scale);return Scale; } set => ControlNodeSetScale(NodeId, ref value); }
        protected Vector2 Size { get { ControlNodeGetSize(NodeId, out Vector2 Size); return Size; } set => ControlNodeSetSize(NodeId, ref value); }
        #endregion
        protected ControlNode() : base() { }
        internal ControlNode(ulong NodeId) : base(NodeId) { }
    }
}
