using Luna;
using System;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            //Console.WriteLine("helloNode");
            string name = "John Doe";
            int age = 26;
            Log.Trace("Hi, my name is {0}. I am {1} years old", name, age);
            Log.Info("Hi, my name is {0}. I am {1} years old", name, age);
            Log.Warn("Hi, my name is {0}. I am {1} years old", name, age);
            Log.Error("Hi, my name is {0}. I am {1} years old", name, age);
            Log.Critical("Hi, my name is {0}. I am {1} years old", name, age);
        }

        override public void Process(ulong delta)
        {

        }
    }
}
