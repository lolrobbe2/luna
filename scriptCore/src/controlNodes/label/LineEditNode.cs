﻿using Luna;

namespace Luna
{
    public class LineEditNode : ControlNode
    {
        /// <summary>
        /// Emitted when the text changes.
        /// </summary>
        /// <param name="NewText"></param>
        [Signal]
        public virtual void TextChanged(string NewText) { }

        [Signal]
        public virtual void TextSubmitted(string NewText) { }
        protected LineEditNode() { }
        internal LineEditNode(ulong ObjectId) : base(ObjectId) { }
    }
}
