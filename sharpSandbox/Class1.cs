using Luna;
using System;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            Console.WriteLine("helloNode");
            //SetName("testing");
        }

        override public void Process(ulong delta)
        {

        }
    }
}
