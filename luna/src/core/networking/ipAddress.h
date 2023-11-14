#pragma once

#include <core/core.h>
namespace luna
{
	namespace networking
	{
		struct ipAddress
		{
		private:
			union {
				uint8_t field8[16];
				uint16_t field16[8];
				uint32_t field32[4];
			};

			bool valid;
			bool wildcard;

		protected:
			void parseIpv6(const std::string& string);
			void parseIpv4(const std::string& string, int start, uint8_t* p_ret);

		public:
			operator std::string() const {
				if (wildcard) {
					return "*";
				}

				if (!valid) {
					return "";
				}

				if (isIpv4()) {
					// IPv4 address mapped to IPv6
					return std::to_string(field8[12]) + "." + std::to_string(field8[13]) + "." + std::to_string(field8[14]) + "." + std::to_string(field8[15]);
				}
				std::string ret;
				for (int i = 0; i < 8; i++) {
					if (i > 0) {
						ret = ret + ":";
					}
					uint16_t num = (field8[i * 2] << 8) + field8[i * 2 + 1];
					std::stringstream sstream;
					sstream << std::hex << num;
					std::string result = sstream.str();
					ret += result;
				}

				return ret;
			}
			

			bool operator==(const ipAddress& p_ip) const {
				if (p_ip.valid != valid) {
					return false;
				}
				if (!valid) {
					return false;
				}
				for (int i = 0; i < 4; i++) {
					if (field32[i] != p_ip.field32[i]) {
						return false;
					}
				}
				return true;
			}

			bool operator!=(const ipAddress& p_ip) const {
				if (p_ip.valid != valid) {
					return true;
				}
				if (!valid) {
					return true;
				}
				for (int i = 0; i < 4; i++) {
					if (field32[i] != p_ip.field32[i]) {
						return true;
					}
				}
				return false;
			}

			void clear();
			bool isWildcard() const { return wildcard; }
			bool isValid() const { return valid; }
			bool isIpv4() const;
			const uint8_t* getIpv4();
			void setIpv4(const uint8_t* p_ip);

			const uint8_t* getIpv6();
			void setIpv6(const uint8_t* p_buf);

			static void registerMethods();

			ipAddress(const std::string& p_string);
			ipAddress(uint32_t p_a, uint32_t p_b, uint32_t p_c, uint32_t p_d, bool is_v6 = false);
			ipAddress() { clear(); }
		};

	
	}
}

