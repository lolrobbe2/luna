namespace Luna
{
    /// <summary>
    /// base class for all engine types except storage types like vector types.
    /// </summary>
    public class Object
    {
        #region Constructor
        protected Object() { ObjectId = 0; }
        internal Object(ulong id) { ObjectId = id; }

        public readonly ulong ObjectId = 0;
        #endregion

        #region public Methods

        /// <summary>
        /// emits the all the signals with the corresponding name and signature connected to this node.
        /// </summary>
        /// <param name="signalName">the name of the signal to be called (name of the declared delagate function)</param>
        /// <param name="args">arguments to be passed to all the called signals</param>
        public void EmitSignal(string signalName, params dynamic[] args) { Signal.EmitSignal(this, signalName,args); }

        /// <summary>
        /// calls all the signals with the corresponding name and signature regardless of with node the signal is connected to.
        /// 
        /// this is usefull for when you want a bunch of objects to do something at the sam time.
        /// </summary>
        /// <param name="signalName">signal name to emit</param>
        /// <param name="args">arguments to pass to the signal</param>
        static public void EmitGlobalSignal(string signalName, params dynamic[] args) { Signal.EmitGlobalSignal(signalName, args); }
        #endregion

        #region QOL variables;

        /// <summary>
        /// this constant variable represents a invalid object,
        /// and ivalid object will have a 0 as ObjectId.
        /// </summary>
        protected const int Invalid = 0;
        #endregion
    }
}
