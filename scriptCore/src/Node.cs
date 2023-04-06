using System;
using System.Runtime.CompilerServices;

namespace Luna
{
    public class Node
    {
        /// <summary>
        /// sets the node name
        /// </summary>
        /// <param name="name">string name</param>
        public void SetName(string name) { NodeSetName(NodeId, name); }
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
        /// <summary>
        /// runs every frame.

        /// <param name="delta">time since previuous frame.</param>
        /// </summary>
        public virtual void Process(ulong delta) { }
        /*internal calls per node*/
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NodeSetName(ulong NodeId,string name);

        protected Node() { NodeId = 0; }
        internal Node(ulong id) { NodeId = id; }

        public readonly ulong NodeId;
    }

}
