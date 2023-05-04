using Luna;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            string version = Os.GetVersion();
            Log.Info("current version = {0}",version);
            Node[] children = GetChildren();
            //Node[] children = GetChildren();
            children[0].SetName("meep meep");
            Log.Info("children size");
        }

        override public void Process(ulong delta)
        {

        }
    }

    internal class Empty : Node
    {
        override public void Ready()
        {
            string version = Os.GetVersion();
            Log.Info("current version = {0}", version);
            
        }

        override public void Process(ulong delta)
        {

        }
    }
}
