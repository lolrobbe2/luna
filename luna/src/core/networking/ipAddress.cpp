
#include "ipAddress.h"
#include <core/debug/debugMacros.h>
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>

static MonoClassField* ipField;
static MonoClassField* validField;
static MonoClassField* wildCardField;

namespace luna 
{
	namespace networking 
	{
		int getSliceCount(const std::string& input, const std::string& splitter) {
			int count = 0;
			size_t pos = 0;
			size_t found;

			while ((found = input.find(splitter, pos)) != std::string::npos) {
				count++;
				pos = found + splitter.length();
			}

			return count + 1; // Add 1 to count the last slice
		}

	

			static void _parse_hex(const std::string& p_string, int p_start, uint8_t* p_dst) {
				uint16_t ret = 0;
				for (int i = p_start; i < p_start + 4; i++) {
					if (i >= p_string.length()) {
						break;
					}

					int n = 0;
					char32_t c = p_string[i];
					if (isdigit(c)) {
						n = c - '0';
					}
					else if (c >= 'a' && c <= 'f') {
						n = 10 + (c - 'a');
					}
					else if (c >= 'A' && c <= 'F') {
						n = 10 + (c - 'A');
					}
					else if (c == ':') {
						break;
					}
					else {
						//LN_ERR_FAIL_MSG("Invalid character in IPv6 address: " + p_string + ".");
					}
					ret = ret << 4;
					ret += n;
				}

				p_dst[0] = ret >> 8;
				p_dst[1] = ret & 0xff;
			}

		void ipAddress::parseIpv6(const std::string& p_string) {
			static const int parts_total = 8;
			int parts[parts_total] = { 0 };
			int parts_count = 0;
			bool part_found = false;
			bool part_skip = false;
			bool part_ipv4 = false;
			int parts_idx = 0;

			for (int i = 0; i < p_string.length(); i++) {
				char32_t c = p_string[i];
				if (c == ':') {
					if (i == 0) {
						continue; // next must be a ":"
					}
					if (!part_found) {
						part_skip = true;
						parts[parts_idx++] = -1;
					}
					part_found = false;
				}
				else if (c == '.') {
					part_ipv4 = true;

				}
				else if (isxdigit(c)) {
					if (!part_found) {
						parts[parts_idx++] = i;
						part_found = true;
						++parts_count;
					}
				}
				else {
					LN_ERR_FAIL_MSG("Invalid character in IPv6 address: " + p_string + ".");
				}
			}

			int parts_extra = 0;
			if (part_skip) {
				parts_extra = parts_total - parts_count;
			}

			int idx = 0;
			for (int i = 0; i < parts_idx; i++) {
				if (parts[i] == -1) {
					for (int j = 0; j < parts_extra; j++) {
						field16[idx++] = 0;
					}
					continue;
				}

				if (part_ipv4 && i == parts_idx - 1) {
					parseIpv4(p_string, parts[i], (uint8_t*)&field16[idx]); // should be the last one
				}
				else {
					_parse_hex(p_string, parts[i], (uint8_t*)&(field16[idx++]));
				}
			}
		}

		void ipAddress::parseIpv4(const std::string& p_string, int p_start, uint8_t* p_ret) {
			std::string ip;
			if (p_start != 0) {
				ip = p_string.substr(p_start, p_string.length() - p_start);
			}
			else {
				ip = p_string;
			}
			
			
			std::vector<std::string> slices;
			std::string slice;
			size_t start = 0;
			size_t end = ip.find('.');
			while (end != std::string::npos) {
				slice = ip.substr(start, end - start);
				slices.push_back(slice);
				start = end + 1;
				end = ip.find('.', start);
			}


			// Add the last slice
			slice = ip.substr(start);
			slices.push_back(slice);

			LN_ERR_FAIL_COND_MSG(slices.size() != 4, "Invalid IP address string: " + ip + ".");


			for (int i = 0; i < 4; i++) {
				p_ret[i] = std::stoi(slices[i]); // Convert each slice to an integer
			}
		}

		void ipAddress::clear() {
			memset(&field8[0], 0, sizeof(field8));
			valid = false;
			wildcard = false;
		}

		bool ipAddress::isIpv4() const {
			return (field32[0] == 0 && field32[1] == 0 && field16[4] == 0 && field16[5] == 0xffff);
		}

		const uint8_t* ipAddress::getIpv4() {
			LN_ERR_FAIL_COND_V_MSG(!isIpv4(), &(field8[12]), "IPv4 requested, but current IP is IPv6."); // Not the correct IPv4 (it's an IPv6), but we don't want to return a null pointer risking an engine crash.
			return &(field8[12]);
		}

		void ipAddress::setIpv4(const uint8_t* p_ip) {
			clear();
			valid = true;
			field16[5] = 0xffff;
			field32[3] = *((const uint32_t*)p_ip);
		}

		const uint8_t* ipAddress::getIpv6() {
			return field8;
		}

		void ipAddress::setIpv6(const uint8_t* p_buf) {
			clear();
			valid = true;
			for (int i = 0; i < 16; i++) {
				field8[i] = p_buf[i];
			}
		}

		ipAddress::ipAddress(const std::string& p_string) {
			clear();

			if (p_string == "*") {
				// Wildcard (not a valid IP)
				wildcard = true;

			}
			else if (p_string.find(":") != std::string::npos) {
				// IPv6
				parseIpv6(p_string);
				valid = true;

			}
			else if (getSliceCount(p_string,".") == 4) {
				// IPv4 (mapped to IPv6 internally)
				field16[5] = 0xffff;
				parseIpv4(p_string, 0, &field8[12]);
				valid = true;

			}
			else {
				LN_CORE_ERROR("Invalid IP address.");
			}
		}

		_ALWAYS_INLINE_ static void _32_to_buf(uint8_t* p_dst, uint32_t p_n) {
			p_dst[0] = (p_n >> 24) & 0xff;
			p_dst[1] = (p_n >> 16) & 0xff;
			p_dst[2] = (p_n >> 8) & 0xff;
			p_dst[3] = (p_n >> 0) & 0xff;
		}

		ipAddress::ipAddress(uint32_t p_a, uint32_t p_b, uint32_t p_c, uint32_t p_d, bool is_v6)
		{
			clear();
			valid = true;
			if (!is_v6) {
				// Mapped to IPv6
				field16[5] = 0xffff;
				field8[12] = p_a;
				field8[13] = p_b;
				field8[14] = p_c;
				field8[15] = p_d;
			}
			else {
				_32_to_buf(&field8[0], p_a);
				_32_to_buf(&field8[4], p_b);
				_32_to_buf(&field8[8], p_c);
				_32_to_buf(&field8[12], p_d);
			}
		}

		static bool IpAddressIsIpv4(ipAddress* ipAddress)
		{
			return ipAddress->isIpv4();
		}

		
		void ipAddress::registerMethods()
		{
		
		}
	}
}
