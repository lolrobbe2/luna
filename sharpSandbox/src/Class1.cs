using Luna;


namespace sharpSandbox
{
    internal class Test : Range
    {
        StreamPeerTCP tcpClient = new StreamPeerTCP();
        override public void Ready()
        {
            tcpClient.ConnectToHost(Ip.ResolveHostnameAddress("example.com"), 80); 
        }

        override public void Process(ulong delta)
        {
            tcpClient.Poll();
            if(tcpClient.GetStatus() == Status.STATUS_CONNECTED && tcpClient.GetAvailableBytes() > 0)
            {
                Log.Info("bytes available");
            }
        }
    }
}
