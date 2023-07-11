using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Luna
{
    public class Node
    {
        #region public Methods
        /// <summary>
        /// Adds a sibling node to current's node parent, at the same level as that node, right below it.
        /// </summary>
        /// <remarks>Use add_child instead of this method if you don't need the child node to be added below a specific node in the list of children.</remarks>
        /// <param name="node"></param>
        public void AddSibling(Node node) { NodeAddSibling(NodeId, node.NodeId); }
        /// <summary>
        /// Adds a child node. Nodes can have any number of children. 
        /// Child nodes are automatically deleted when the parent node is deleted, so an entire scene can be removed by deleting its topmost node.
        /// </summary>
        /// <param name="node"></param>
        public void AddChild(Node node) { NodeAddChild(NodeId, node.NodeId); }

        /// <summary>
        /// sets the node name
        /// </summary>
        /// <param name="name">string name</param>
        public void SetName(string name) { NodeSetName(NodeId, name); }
        /// <summary>
        /// <para>Returns a child node by its index (see get_child_count). This method is often used for iterating all children of a node.</para>
        /// Negative indices access the children from the last one.
        /// </summary>
        /// <see cref="GetNode()"/>
        /// <param name="idx"></param>
        /// <returns></returns>
        public Node GetChild(int idx) 
        {
            if (idx < 0) idx = GetChildren().Length - 1;
            return GetChildren()[idx]; 
        }

        /// <summary>
        /// returns the amount of children the node has.
        /// </summary>
        /// <returns>int ChildCount</returns>
        public int GetChildCount() { return Children.Length; }
        /// <summary>
        /// returns an array of the nodes children.
        /// </summary>
        /// <returns></returns>
        public Node[] GetChildren() { return NodeGetChildren(NodeId);}
        /// <summary>
        /// returns the parent Node or Null when the node is a root node.
        /// </summary>
        /// <returns>Node parent</returns>
        public Node GetParent() { return NodeGetParent(NodeId); }
        #endregion

        #region engineHooks
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
        #endregion

        #region InternalCalls

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeSetName(ulong NodeId,string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeGetName(ulong NodeId,out string name);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeAddSibling(ulong NodeId, ulong SiblingNodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeAddChild(ulong NodeId, ulong SiblingNodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Node[] NodeGetChildren(ulong NodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Node NodeGetParent(ulong NodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern ulong NodeCreateNew();
        #endregion

        #region Constructor
        /// <summary>
        /// creates a new node at runtime
        /// </summary>
        protected Node() { NodeId = 0; }
        internal Node(ulong id) { NodeId = id; }

        public readonly ulong NodeId = 0;
        #endregion

        #region QOL variables;


        /// <summary>
        /// Child nodes of the current node.
        /// </summary>
        protected Node[] Children => GetChildren();
        /// <summary>
        /// parent Node of the current node.
        /// </summary>
        protected Node Parent => GetParent();
        /// <summary>
        /// node name
        /// </summary>
        protected string Name { get { NodeGetName(NodeId, out string Name); return Name; } set => NodeSetName(NodeId,value); }

        #endregion
    }

}
