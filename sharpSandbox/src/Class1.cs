using Luna;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            Log.Info("current version = {0}", Os.GetVersion());
            Node[] children = GetChildren();
            //children[0].SetName("meep meep");
            Log.Info("children size");
        }

        override public void Process(ulong delta)
        {

        }
    }
}
