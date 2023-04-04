using System;
using System.Runtime.CompilerServices;

namespace Luna
{
    class Node
    {
        /// <summary>
        /// sets the node name
        /// </summary>
        /// <param name="name">string name</param>
        public void SetName(string name) { NodeSetName(NodeId, name); }

        /*internal calls per node*/
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NodeSetName(ulong NodeId,string name);

        protected Node() { NodeId = 0; }
        internal Node(ulong id) { NodeId = id; }

        public readonly ulong NodeId;
    }

}
