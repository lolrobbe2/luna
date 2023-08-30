﻿using System.Reflection;
using System.Runtime.CompilerServices;

namespace Luna
{
    //shamefully copied from godot docs and moddified!
    /// <summary>
    /// <para>
    /// <br>Nodes are Luna's building blocks. They can be assigned as the child of another node,</br> 
    /// <br>resulting in a tree arrangement. A given node can contain any number of nodes as children </br> 
    /// <br>with the requirement that all siblings (direct children of a node) should have unique id's.</br> 
    /// </para>
    /// 
    /// <para>
    /// <br>A tree of nodes is called a scene. Scenes can be saved to the disk and then instantiated into other scenes.</br> 
    /// <br> This allows for very high flexibility in the architecture and data model of Luna projects.</br> 
    /// </para>
    /// </summary>
    public class Node : LunaObject
    {
        #region public Methods
        /// <summary>
        /// Adds a sibling node to current's node parent, at the same level as that node, right below it.
        /// </summary>
        /// <remarks>Use add_child instead of this method if you don't need the child node to be added below a specific node in the list of children.</remarks>
        /// <param name="node"></param>
        public void AddSibling(Node node) { NodeAddSibling(ObjectId, node.ObjectId); }
        /// <summary>
        /// Adds a child node. Nodes can have any number of children. 
        /// Child nodes are automatically deleted when the parent node is deleted, so an entire scene can be removed by deleting its topmost node.
        /// </summary>
        /// <param name="node"></param>
        public void AddChild(Node node) { NodeAddChild(ObjectId, node.ObjectId); }

        /// <summary>
        /// sets the node name
        /// </summary>
        /// <param name="name">string name</param>
        public void SetName(string name) { NodeSetName(ObjectId, name); }
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
        public Node[] GetChildren() { return NodeGetChildren(ObjectId);}
        /// <summary>
        /// returns the parent Node or Null when the node is a root node.
        /// </summary>
        /// <returns>Node parent</returns>
        public Node GetParent() { return NodeGetParent(ObjectId); }
        #endregion

        #region signals
        /// <summary>
        /// Emitted when a child node enters the scene tree, either because it entered on its own or because this node entered with it.
        /// </summary>
        /// <param name="node"></param>
        [Signal]
        public virtual void ChildEnteredTree(Node node) { }
        /// <summary>
        /// Emitted when a child node is about to exit the scene tree, either because it is being removed or freed directly, or because this node is exiting the tree.
        /// </summary>
        /// <param name="node"></param>
        [Signal]
        public virtual void ChildExitingTree(Node node) { }
        /// <summary>
        /// emitted when the node is ready. Comes after ready callback and follows the same rules.
        /// </summary>
        [Signal]
        public virtual void Ready() { }
        /// <summary>
        /// Emitted when the node enters the tree.
        /// </summary>
        [Signal]
        public virtual void treeEntered() { }
        /// <summary>
        /// Emitted after the node exits the tree and is no longer active.
        /// </summary>
        [Signal]
        public virtual void treeExited() { }
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
        static extern void NodeSetName(ulong ObjectId,string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeGetName(ulong ObjectId,out string name);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeAddSibling(ulong ObjectId, ulong SiblingNodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern void NodeAddChild(ulong ObjectId, ulong SiblingNodeId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Node[] NodeGetChildren(ulong ObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern Node NodeGetParent(ulong ObjectId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        static extern ulong NodeCreateNew();
        #endregion

        #region Constructor
        /// <summary>
        /// creates a new node at runtime
        /// </summary>
        protected Node() : base(0) { }
        internal Node(ulong id) : base(id) { }
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
        protected string Name { get { NodeGetName(ObjectId, out string Name); return Name; } set => NodeSetName(ObjectId,value); }

        #endregion
    }

}
