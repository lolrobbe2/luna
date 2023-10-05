using System;
using System.Runtime.InteropServices;

namespace Luna
{
    [StructLayout(LayoutKind.Explicit)]
    public struct ipField
    {
        [FieldOffset(0)]
        public byte[] field8;

        [FieldOffset(0)]
        public ushort[] field16;

        [FieldOffset(0)]
        public uint[] field32;
    }

    public class IpAddress : object
    {
        ipField ipField = new ipField();
        bool valid = new bool();
        bool wildcard = new bool();

        public string ToString()
        {
            if (wildcard)
            {
                return "*";
            }

            if (!valid)
            {
                return "";
            }

            if (IsIpv4())
            {
                // IPv4 address mapped to IPv6
                return ipField.field8[12].ToString() + "." + ipField.field8[13].ToString() + "." + ipField.field8[14].ToString() + "." + ipField.field8[15].ToString();
            }
            string ret = "";
            for (int i = 0; i < 8; i++)
            {
                if (i > 0)
                {
                    ret += ":";
                }
                UInt16 num = (ushort)((ipField.field8[i * 2] << 8) + ipField.field8[i * 2 + 1]);
                
               
               
                ret += num.ToString("X4");
            }

            return ret;
        }

        public IpAddress() {
            ipField.field8 = new byte[16];
         
            Clear(); }
        public IpAddress(UInt32 p_a, UInt32 p_b, UInt32 p_c, UInt32 p_d, bool is_v6 = false)
        {
            ipField.field8 = new byte[16];

            Clear();
            valid = true;
            if (!is_v6)
            {
                // Mapped to IPv6
                ipField.field16[5] = 0xffff;
                ipField.field8[12] = (byte)p_a;
                ipField.field8[13] = (byte)p_b;
                ipField.field8[14] = (byte)p_c;
                ipField.field8[15] = (byte)p_d;
            }
            else
            {
                _32_to_buf(ipField.field8, p_a, 0);
                _32_to_buf(ipField.field8, p_b, 4);
                _32_to_buf(ipField.field8, p_c, 8);
                _32_to_buf(ipField.field8, p_d, 12);
            }
            void _32_to_buf(byte[] p_dst, UInt32 p_n, uint offset)
            {
                p_dst[offset + 0] = (byte)((p_n >> 24) & 0xff);
                p_dst[offset + 1] = (byte)((p_n >> 16) & 0xff);
                p_dst[offset + 2] = (byte)((p_n >> 8) & 0xff);
                p_dst[offset + 3] = (byte)((p_n >> 0) & 0xff);
            }
        }

        public IpAddress(string ipAddress)
        {
            ipField.field8 = new byte[16];
            
            Clear();

            if (ipAddress == "*")
            {
                // Wildcard (not a valid IP)
                wildcard = true;

            }
            else if (ipAddress.IndexOf(":") >= 0)
            {
                // IPv6
                parseIpv6(ipAddress);
                valid = true;

            }
            else if (ipAddress.Split('.').Length == 4)
            {
                // IPv4 (mapped to IPv6 internally)
                ipField.field16[5] = 0xffff;
                parseIpv4(ipAddress, 0, ipField.field8, 12); // field offset = 12.
                valid = true;

            }
            else
            {
                Log.Error("Invalid IP address.");
            }
        }

        private void parseIpv6(string ipAddress)
        {
            const int parts_total = 8;
            int[] parts = new int[parts_total];
            int parts_count = 0;
            bool part_found = false;
            bool part_skip = false;
            bool part_ipv4 = false;
            int parts_idx = 0;

            for (int i = 0; i < ipAddress.Length; i++)
            {
                char c = ipAddress[i];
                if (c == ':')
                {
                    if (i == 0)
                    {
                        continue; // next must be a ":"
                    }
                    if (!part_found)
                    {
                        part_skip = true;
                        parts[parts_idx++] = -1;
                    }
                    part_found = false;
                }
                else if (c == '.')
                {
                    part_ipv4 = true;

                }
                else if (char.IsDigit(c))
                {
                    if (!part_found)
                    {
                        parts[parts_idx++] = i;
                        part_found = true;
                        ++parts_count;
                    }
                }
                else
                {
                    Log.Error("Invalid character in IPv6 address: {0} .", ipAddress);
                }
            }

            int parts_extra = 0;
            if (part_skip)
            {
                parts_extra = parts_total - parts_count;
            }

            int idx = 0;
            for (int i = 0; i < parts_idx; i++)
            {
                if (parts[i] == -1)
                {
                    for (int j = 0; j < parts_extra; j++)
                    {
                        ipField.field16[idx++] = 0;
                    }
                    continue;
                }

                if (part_ipv4 && i == parts_idx - 1)
                {
                    byte[] tempField = new byte[ipField.field16.Length * 2];
                    Buffer.BlockCopy(ipField.field16, 0, tempField, 0, tempField.Length);
                    parseIpv4(ipAddress, parts[i], tempField, idx); // should be the last one
                    Buffer.BlockCopy(tempField, 0, ipField.field16, 0, tempField.Length);
                }
                else
                {
                    byte[] tempField = new byte[ipField.field16.Length * 2];
                    Buffer.BlockCopy(ipField.field16, 0, tempField, 0, tempField.Length);
                    _parse_hex(ipAddress, parts[i], tempField, idx++);
                    Buffer.BlockCopy(tempField, 0, ipField.field16, 0, tempField.Length);

                }
            }
        }


        public void Clear()
        {

            valid = false;
            wildcard = false;
        }
        public bool IsWildcard() { return wildcard; }
        public bool IsValid() { return valid; }
        public bool IsIpv4()
        {
            return (ipField.field32[0] == 0 && ipField.field32[1] == 0 && ipField.field16[4] == 0 && ipField.field16[5] == 0xffff); ;
        }

        private void parseIpv4(String IpString, int Start, byte[] p_ret, int StartIndex)
        {
            string Ip;
            if (Start != 0)
            {
                Ip = IpString.Substring(Start, IpString.Length - Start);
            }
            else
            {
                Ip = IpString;
            }

            string[] SlicedIpString = IpString.Split('.');
            if (SlicedIpString.Length != 4) { Log.Error("Invalid IP address string: {0} .", IpString); return; }
            for (int i = 0; i < 4; i++)
            {
                p_ret[StartIndex + i] = Byte.Parse(SlicedIpString[i]);
            }
        }
        public void _parse_hex(string p_string, int p_start, byte[] p_dst,int offset) {
			UInt16 ret = 0;
			for (int i = p_start; i<p_start + 4; i++) {
				if (i >= p_string.Length) {
					break;
				}

				int n = 0;
                char c = p_string[i];
				if (char.IsDigit(c)) {
					n = c - '0';
				}
				else if (c >= 'a' && c <= 'f') {
					n = 10 + (c - 'a');
				}

                else if (c >= 'A' && c <= 'F')
                {
                    n = 10 + (c - 'A');
                }
                else if (c == ':')
                {
                    break;
                }
                else
                {
                    //LN_ERR_FAIL_MSG("Invalid character in IPv6 address: " + p_string + ".");
                }
                ret = (ushort)(ret << 4);
                ret += (ushort)n;
			}

			p_dst[offset] = (byte)(ret >> 8);
            p_dst[offset+1] = (byte)(ret & 0xff);
		}
    }
}
   

   
