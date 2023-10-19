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
            Ip.ResolveHostname("facebook.com");
        }

        override public void Process(ulong delta)
        {
            
        }
    }
}
