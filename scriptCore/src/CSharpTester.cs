using System;

namespace Luna
{
    public class Main
    {
        public float MyPublicFloatVar = 5.0f;
        public Main()
        {
            PrintFloatVar();
        }
        public void PrintFloatVar()
        {
            Console.WriteLine("MyPublicFloatVar = {0:F}", MyPublicFloatVar);
        }

        private void IncrementFloatVar(float value)
        {
            MyPublicFloatVar += value;
        }

    }
}
