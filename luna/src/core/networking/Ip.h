#pragma once

#include <core/debug/typedefs.h>
#include <core/utils/semaphore.h>
#include <core/networking/ipAddress.h>
namespace luna 
{
	namespace networking 
	{

		class Ip 
		{

		public:
			enum ResolverStatus {
				RESOLVER_STATUS_NONE,
				RESOLVER_STATUS_WAITING,
				RESOLVER_STATUS_DONE,
				RESOLVER_STATUS_ERROR,
			};

			enum Type {
				TYPE_NONE = 0,
				TYPE_IPV4 = 1,
				TYPE_IPV6 = 2,
				TYPE_ANY = 3,
			};

			enum {
				RESOLVER_MAX_QUERIES = 256,
				RESOLVER_INVALID_ID = -1
			};

			typedef int ResolverID;

		private:
		

		protected:
			static std::vector<std::string> getLocalAddresses();

		public:
			static void RegisterMethods();

			struct Interface_Info {
				std::string name;
				std::string name_friendly;
				std::string index;
				std::list<ipAddress> ip_addresses;
			};

			static std::string resolveHostname(const std::string& p_hostname, Type p_type = TYPE_ANY);
			static std::vector<std::string> resolveHostnameAddresses(const std::string& p_hostname, Type p_type = TYPE_ANY);
			// async resolver hostname
			static ResolverID resolveHostnameQueueItem(const std::string& p_hostname, Type p_type = TYPE_ANY);
			static ResolverStatus getResolveItemStatus(ResolverID p_id);
			static ipAddress getResolveItemAddress(ResolverID p_id);
			static void getLocalAddresses(std::vector<ipAddress>* r_addresses);

			static void _resolve_hostname(std::vector<ipAddress>& r_addresses, const std::string& p_hostname, Type p_type = TYPE_ANY);
			static std::vector<std::string> getResolveItemAddresses(ResolverID p_id) ;
			static std::string getHostnameFromIP(const ipAddress& ip);
			static void getLocalInterfaces(std::unordered_map<std::string, Interface_Info>* r_interfaces);
			static void eraseResolveItem(ResolverID p_id);

			void clearCache(const std::string& p_hostname = "");

			static void init();
			static void shutdown();
		};
	}
}


