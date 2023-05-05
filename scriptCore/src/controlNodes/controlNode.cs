using System.Runtime.CompilerServices;

namespace Luna
{
    public class ControlNode : Node
    {
        protected Vector2 GetScale() { return ControlNodeGetScale(NodeId);}
        protected void SetScale(Vector2 Scale) { ControlNodeSetScale(NodeId, Scale); }
        protected Vector2 GetPosition() { return ControlNodeGetPosition(NodeId); }
        protected void SetPosition(Vector2 position) { ControlNodeSetPosition(NodeId, position); }
        protected Vector2 GetSize() { return ControlNodeGetSize(NodeId); }
        protected void SetSize(Vector2 size) {  ControlNodeSetSize(NodeId, size); }
        #region internal calls
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Vector2 ControlNodeGetPosition(ulong NodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeSetPosition(ulong NodeId, Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Vector2 ControlNodeGetScale(ulong NodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeSetScale(ulong NodeId, Vector2 Offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Vector2 ControlNodeGetSize(ulong NodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void ControlNodeSetSize(ulong NodeId, Vector2 Size);
        #endregion

        #region dummy fields 
        protected Vector2 Position { get => GetPosition(); set => SetPosition(value); }
        protected Vector2 Scale { get => GetPosition(); set => SetPosition(value); }
        protected Vector2 Size { get => GetSize(); set => SetSize(value); }
        #endregion
    }
}
