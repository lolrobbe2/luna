using Luna;


namespace sharpSandbox
{
    internal class Test : Node
    {
        public override void ChildEnteredTree(Node node)
        {
            Log.Info("test");
        }
        override public void Ready()
        {
            string version = Os.GetVersion();
            Log.Info("current version = {0}",version);
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
            Position.X = 10;
        }

        override public void Process(ulong delta)
        {

        }
    }
}
