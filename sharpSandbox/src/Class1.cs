using Luna;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            Log.Info("current version = {0}", Os.GetVersion());
        }

        override public void Process(ulong delta)
        {

        }
    }
}
