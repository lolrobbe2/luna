using Luna;


namespace sharpSandbox
{
    internal class Test : Range
    {
        public override void ChildEnteredTree(Node node)
        {
            Log.Info("test");
        }
        override public void Ready()
        {
            string version = Os.GetVersion();
            Log.Info("current version = {0}",version);
        }

        override public void Process(ulong delta)
        {
            
        }
    }
}
