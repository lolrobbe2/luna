using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Luna
{
    public class Node
    {
        /// <summary>
        /// sets the node name
        /// </summary>
        /// <param name="name">string name</param>
        public void SetName(string name) { NodeSetName(NodeId, name); }
        
        public Node[] GetChildren() {
            IntPtr[] ptr = NodeGetChildren(NodeId);
            Log.Info("array size = {0}",ptr.Length);
            /*
            Node[] children = new Node[ptr.Length];
            for (int i = 0; i < ptr.Length; i++)
            {
  
                Marshal.PtrToStructure(ptr[i], node);
            }
            */
            return new Node[1]; 
        }
        /// <summary>
        /// runs on node creation.
        /// <example>
        /// <code>
        /// override public void Ready()
        /// {
        ///     SetName("testing");
        /// }
        /// </code>
        /// </example>
        /// </summary>
        public virtual void Ready() { }
        /// <summary>control
        /// runs every frame.

        /// <param name="delta">time since previuous frame.</param>
        /// </summary>
        public virtual void Process(ulong delta) { }
        /*internal calls per node*/

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeSetName(ulong NodeId,string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern IntPtr[] NodeGetChildren(ulong NodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern int NodeGetChildrenSize(ulong NodeId);
        protected Node() { NodeId = 0; }
        internal Node(ulong id) { NodeId = id; }

        public readonly ulong NodeId;
    }

}
