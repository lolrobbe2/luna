using Luna;
using System.Security.Cryptography.X509Certificates;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            string version = Os.GetVersion();
            Log.Info("current version = {0}",version);
            Children[0].SetName("meep meep");
            Log.Info("children size");
        }

        override public void Process(ulong delta)
        {

        }
    }

    internal class Empty : ControlNode
    {
        override public void Ready()
        {
            Parent.SetName("welp");
            Position.X = 0;
        }

        override public void Process(ulong delta)
        {

        }
    }
}
