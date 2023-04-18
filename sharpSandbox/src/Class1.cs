using Luna;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            string name = Os.GetName();
            Log.Warn("os name:{0}",name);
        }

        override public void Process(ulong delta)
        {

        }
    }
}
