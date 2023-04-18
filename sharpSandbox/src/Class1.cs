using Luna;

namespace sharpSandbox
{
    internal class Test : Node
    {
        override public void Ready()
        {
            string filePath = Luna.Os.OpenFileDialog("image\0*.png;*.jpeg;*.jpg\0");
            Luna.Log.Warn("open file:{0}",filePath);
        }

        override public void Process(ulong delta)
        {

        }
    }
}
