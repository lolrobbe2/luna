using System;
using System.Runtime.CompilerServices;

namespace Luna
{
    class node
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void setName(string name);
    }

}
