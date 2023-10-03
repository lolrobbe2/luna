using Luna;
using System;
using System.Collections.Generic;

namespace Luna
{
    public class IpAddress
    {
        public char[] field8 = new char[16];
        public UInt16[] field16 = new UInt16[8];
        public UInt32[] field32 = new UInt32[4];

        bool valid;
        bool wildcard;

        IpAddress() { /*clear();*/ }
        IpAddress(UInt32 p_a, UInt32 p_b, UInt32 p_c, UInt32 p_d, bool is_v6 = false)
        {
            IpAddress address = new IpAddress();
            ConstructIpAddress(out address,p_a,p_b,p_c,p_d,is_v6);

            field8 = address.field8;
            field16 = address.field16;
            field32 = address.field32;

            valid = address.valid;
            wildcard = address.wildcard;

        }

        extern static void ConstructIpAddress(out IpAddress address, UInt32 p_a, UInt32 p_b, UInt32 p_c, UInt32 p_d, bool is_v6 = false);
        extern static void ConstructIpAddress(out IpAddress address,)
    }
}
