using System;
using System.Diagnostics.Contracts;
using System.Runtime.CompilerServices;

namespace Luna
{
    #region publicAttributeMethods
    /// <summary>
    /// this is the signal attribute/ class
    /// </summary>
    public class Signal : System.Attribute
    {   
        public Signal() {}

        /// <summary>
        /// emits the all the signals with the corresponding name and signature connected to this node.
        /// </summary>
        /// <param name="emitterObject">the object that emits </param>
        /// <param name="signalName"></param>
        /// <param name="args"></param>
        static public void EmitSignal(object emitterObject,string signalName, params dynamic[] args) { SignalEmitSignal(((LunaObject)emitterObject).ObjectId, signalName,args); }
        /// <summary>
        /// emits the all the signals with the corresponding name and signature connected to this node.
        /// </summary>
        /// <param name="emitterObjectId">the objectId of the object that emits </param>
        /// <param name="signalName"></param>
        /// <param name="args"></param>
        static public void EmitSignal(ulong emitterObjectId, string signalName, params dynamic[] args) { SignalEmitSignal(emitterObjectId, signalName, args); }
        /// <summary>
        /// calls all the signals with the corresponding name and signature regardless of with node the signal is connected to.
        /// 
        /// this is usefull for when you want a bunch of objects to do something at the sam time.
        /// </summary>
        /// <param name="signalName">signal name to emit</param>
        /// <param name="args">arguments to pass to the signal</param>
        static public void EmitGlobalSignal(string signalName, params dynamic[] args) { SignalEmitGlobalSignal(signalName,args); }
        #endregion

        #region InternalCalls
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void SignalEmitSignal(ulong emitterObjectId, string signal, params dynamic[] args);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void SignalEmitGlobalSignal(string signalName, params dynamic[] args);
        #endregion
    }

}
