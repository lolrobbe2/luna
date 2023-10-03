#include "Ip.h"
#include <core/debug/debugMacros.h>
namespace luna 
{

	template <typename T>
	class safeNumeric {
	public:
		safeNumeric() : value((T)0) {}

		safeNumeric(T val) : value(val) {}

		safeNumeric<T>& operator=(T val) {
			value = val;
			return *this;
		}

		safeNumeric<T>& operator+=(T val) {
			checkOverflow(val);
			value += val;
			return *this;
		}

		safeNumeric<T>& operator-=(T val) {
			checkOverflow(-val);
			value -= val;
			return *this;
		}

		safeNumeric<T>& operator*=(T val) {
			checkMultiplicationOverflow(val);
			value *= val;
			return *this;
		}

		safeNumeric<T>& operator/=(T val) {
			if (val == 0) {
				//throw std::runtime_error("Division by zero");
			}
			value /= val;
			return *this;
		}

		T get() const {
			return value;
		}

		void set(T val) {
			value = val;
		}

	private:
		T value;

		void checkOverflow(T val) {
			if ((val > 0 && value > std::numeric_limits<T>::max() - val) ||
				(val < 0 && value < std::numeric_limits<T>::min() - val)) {
				//throw std::overflow_error("Arithmetic overflow");
			}
		}

		void checkMultiplicationOverflow(T val) {
			if ((val > 0 && (value > std::numeric_limits<T>::max() / val ||
				value < std::numeric_limits<T>::min() / val)) ||
				(val < 0 && (value < std::numeric_limits<T>::max() / val ||
					value > std::numeric_limits<T>::min() / val))) {
				//throw std::overflow_error("Arithmetic overflow");
			}
		}
	};
	namespace networking 
	{
	
		struct _IP_ResolverPrivate 
		{
		public:
			_IP_ResolverPrivate() { };
			struct queueItem {
				safeNumeric<Ip::ResolverStatus> status;

				std::vector<ipAddress> response;

				std::string hostname;
				Ip::Type type;

				void clear() {
					status.set(Ip::RESOLVER_STATUS_NONE);
					response.clear();
					type = Ip::TYPE_NONE;
					hostname = "";
				};

				queueItem() {
					clear();
				}
			};

			queueItem queue[Ip::RESOLVER_MAX_QUERIES];

			Ip::ResolverID find_empty_id() const {
				for (int i = 0; i < Ip::RESOLVER_MAX_QUERIES; i++) {
					if (queue[i].status.get() == Ip::RESOLVER_STATUS_NONE) {
						return i;
					}
				}
				return Ip::RESOLVER_INVALID_ID;
			}

			std::mutex mutex;
			const int max_smph{ 3 };
			semaphore sem;


			std::thread thread;
			bool threadAbort;

			void resolveQueues() {
				for (int i = 0; i < Ip::RESOLVER_MAX_QUERIES; i++) {
					if (queue[i].status.get() != Ip::RESOLVER_STATUS_WAITING) {
						continue;
					}

					mutex.lock();
					std::vector<ipAddress> response;
					std::string hostname = queue[i].hostname;
					Ip::Type type = queue[i].type;
					mutex.unlock();

					// We should not lock while resolving the hostname,
					// only when modifying the queue.
					Ip::_resolve_hostname(response, hostname, type);

					std::lock_guard lock(mutex);
					// Could have been completed by another function, or deleted.
					if (queue[i].status.get() != Ip::RESOLVER_STATUS_WAITING) {
						continue;
					}
					// We might be overriding another result, but we don't care as long as the result is valid.
					if (response.size()) {
						std::string key = getCacheKey(hostname, type);
						cache[key] = response;
					}
					queue[i].response = response;
					queue[i].status.set(response.empty() ? Ip::RESOLVER_STATUS_ERROR : Ip::RESOLVER_STATUS_DONE);
				}
			}

			static void _thread_function(void* self) {
				_IP_ResolverPrivate* ipr = static_cast<_IP_ResolverPrivate*>(self);

				while (!ipr->threadAbort) {
					ipr->sem.wait();
					ipr->resolveQueues();
				}
			}

			std::unordered_map<std::string, std::vector<ipAddress>> cache;

			static std::string getCacheKey(std::string p_hostname, Ip::Type p_type) {
				return std::to_string(p_type) + p_hostname;
			}
		};

		static _IP_ResolverPrivate* resolver;

		std::string Ip::resolveHostname(const std::string& p_hostname, Ip::Type p_type) {
			const std::vector<std::string> addresses = resolveHostnameAddresses(p_hostname, p_type);
			return addresses.size() ? addresses[0] : ipAddress();
		}

		std::vector<std::string> Ip::resolveHostnameAddresses(const std::string& p_hostname, Type p_type) {
			std::vector<ipAddress> res;
			std::string key = _IP_ResolverPrivate::getCacheKey(p_hostname, p_type);

			resolver->mutex.lock();
			if (resolver->cache.find(key) != resolver->cache.end()) {
				res = resolver->cache[key];
			}
			else {
				// This should be run unlocked so the resolver thread can keep resolving
				// other requests.
				resolver->mutex.unlock();
				_resolve_hostname(res, p_hostname, p_type);
				resolver->mutex.lock();
				// We might be overriding another result, but we don't care as long as the result is valid.
				if (res.size()) {
					resolver->cache[key] = res;
				}
			}
			resolver->mutex.unlock();

			std::vector<std::string> result;
			for (int i = 0; i < res.size(); ++i) {
				result.push_back(res[i]);
			}
			return result;
		}

		Ip::ResolverID Ip::resolveHostnameQueueItem(const std::string& p_hostname, Ip::Type p_type) {
			std::lock_guard lock(resolver->mutex);

			ResolverID id = resolver->find_empty_id();

			if (id == RESOLVER_INVALID_ID) {
				LN_CORE_WARN("Out of resolver queries");
				return id;
			}

			std::string key = _IP_ResolverPrivate::getCacheKey(p_hostname, p_type);
			resolver->queue[id].hostname = p_hostname;
			resolver->queue[id].type = p_type;
			if (resolver->cache.find(key) != resolver->cache.end()) {
				resolver->queue[id].response = resolver->cache[key];
				resolver->queue[id].status.set(Ip::RESOLVER_STATUS_DONE);
			}
			else 
			{
				resolver->queue[id].response = std::vector<ipAddress>();
				resolver->queue[id].status.set(Ip::RESOLVER_STATUS_WAITING);
				if (resolver->thread.joinable()) {
					resolver->sem.post();
				}
				else {
					resolver->resolveQueues();
				}
			}

			return id;
		}

		Ip::ResolverStatus Ip::getResolveItemStatus(ResolverID p_id) {
			
			//LN_ERR_FAIL_INDEX_V_MSG(p_id, Ip::RESOLVER_MAX_QUERIES, Ip::RESOLVER_STATUS_NONE, "Too many concurrent DNS resolver queries (" + std::to_string(p_id) + ", but should be" + std::to_string(Ip::RESOLVER_MAX_QUERIES) + " at most).Try performing less network requests at once.");

			Ip::ResolverStatus res = resolver->queue[p_id].status.get();
			if (res == Ip::RESOLVER_STATUS_NONE) {
				//LN_CORE_ERROR("Condition status == Ip::RESOLVER_STATUS_NONE");
				return Ip::RESOLVER_STATUS_NONE;
			}
			return res;
		}

		ipAddress Ip::getResolveItemAddress(ResolverID p_id){
			//LN_ERR_FAIL_INDEX_V_MSG(p_id, Ip::RESOLVER_MAX_QUERIES, ipAddress(), "Too many concurrent DNS resolver queries (" + std::to_string(p_id) + ", but should be" +std::to_string(Ip::RESOLVER_MAX_QUERIES) + " at most).Try performing less network requests at once.");

			std::lock_guard lock(resolver->mutex);

			if (resolver->queue[p_id].status.get() != Ip::RESOLVER_STATUS_DONE) {
				LN_CORE_ERROR("Resolve of '" + resolver->queue[p_id].hostname + "'' didn't complete yet.");
				return ipAddress();
			}

			std::vector<ipAddress> res = resolver->queue[p_id].response;

			for (int i = 0; i < res.size(); ++i) {
				if (res[i].isValid()) {
					return res[i];
				}
			}
			return ipAddress();
		}

		void Ip::getLocalAddresses(std::vector<ipAddress>* r_addresses)
		{
		}

		std::vector<std::string> Ip::getResolveItemAddresses(ResolverID p_id) 
		{
			//LN_ERR_FAIL_INDEX_V_MSG(p_id, Ip::RESOLVER_MAX_QUERIES, std::vector<std::string>(), "Too many concurrent DNS resolver queries (" + std::to_string(p_id) + ", but should be" + std::to_string(Ip::RESOLVER_MAX_QUERIES) + " at most).Try performing less network requests at once.");
			std::lock_guard lock(resolver->mutex);

			if (resolver->queue[p_id].status.get() != Ip::RESOLVER_STATUS_DONE) {
				LN_CORE_ERROR("Resolve of '" + resolver->queue[p_id].hostname + "'' didn't complete yet.");
				return std::vector<std::string>();
			}

			std::vector<ipAddress> res = resolver->queue[p_id].response;

			std::vector<std::string> result;
			for (int i = 0; i < res.size(); ++i) {
				if (res[i].isValid()) {
					result.push_back(std::string(res[i]));
				}
			}
			return result;
		}

		void Ip::eraseResolveItem(ResolverID p_id) {
			//LN_ERR_FAIL_INDEX_MSG(p_id, Ip::RESOLVER_MAX_QUERIES, "Too many concurrent DNS resolver queries (" + std::to_string(p_id) + ", but should be" + std::to_string(Ip::RESOLVER_MAX_QUERIES) + " at most).Try performing less network requests at once.");

			resolver->queue[p_id].status.set(Ip::RESOLVER_STATUS_NONE);
		}

		void Ip::clearCache(const std::string& p_hostname) {
			std::lock_guard lock(resolver->mutex);

			if (p_hostname.empty()) {
				resolver->cache.clear();
			}
			else {
				resolver->cache.erase(_IP_ResolverPrivate::getCacheKey(p_hostname, Ip::TYPE_NONE));
				resolver->cache.erase(_IP_ResolverPrivate::getCacheKey(p_hostname, Ip::TYPE_IPV4));
				resolver->cache.erase(_IP_ResolverPrivate::getCacheKey(p_hostname, Ip::TYPE_IPV6));
				resolver->cache.erase(_IP_ResolverPrivate::getCacheKey(p_hostname, Ip::TYPE_ANY));
			}
		}

		std::vector<std::string> Ip::getLocalAddresses() {
			std::vector<std::string> addresses;
			std::vector<ipAddress> ip_addresses;
			getLocalAddresses(&ip_addresses);

			for (const ipAddress& E : ip_addresses) addresses.push_back(E);
			return addresses;
		}

		void Ip::RegisterMethods()
		{
		}

		void Ip::init()
		{
			resolver = new _IP_ResolverPrivate();
			resolver->threadAbort = false;
			resolver->thread = std::thread(_IP_ResolverPrivate::_thread_function, resolver);
		}

		void Ip::shutdown()
		{
			resolver->threadAbort = true;
			resolver->sem.post();
			resolver->thread.join();

			delete(resolver);
		}

#pragma region  platform specific implementation
#if defined(UNIX_ENABLED) || defined(LN_PLATFORM_WINDOWS)

#ifdef LN_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2def.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib") //ws2tcip
#pragma comment(lib, "IPHLPAPI.lib") //iphlpapi

#else // UNIX

#include <netdb.h>

#ifdef ANDROID_ENABLED
		// We could drop this file once we up our API level to 24,
		// where the NDK's ifaddrs.h supports to needed getifaddrs.
#include "thirdparty/misc/ifaddrs-android.h"
#else
#ifdef __FreeBSD__
#include <sys/types.h>
#endif
#include <ifaddrs.h>
#endif

#include <arpa/inet.h>
#include <sys/socket.h>

#ifdef __FreeBSD__
#include <netinet/in.h>
#endif

#include <net/if.h> // Order is important on OpenBSD, leave as last.

#endif // UNIX

		static ipAddress _sockaddr2ip(struct sockaddr* p_addr) {
			ipAddress ip;

			if (p_addr->sa_family == AF_INET) {
				struct sockaddr_in* addr = (struct sockaddr_in*)p_addr;
				ip.setIpv4((uint8_t*)&(addr->sin_addr));
			}
			else if (p_addr->sa_family == AF_INET6) {
				struct sockaddr_in6* addr6 = (struct sockaddr_in6*)p_addr;
				ip.setIpv6(addr6->sin6_addr.s6_addr);
			}

			return ip;
		}

		void Ip::_resolve_hostname(std::vector<ipAddress>& r_addresses, const std::string& p_hostname, Type p_type) {
			struct addrinfo hints;
			struct addrinfo* result = nullptr;

			memset(&hints, 0, sizeof(struct addrinfo));
			if (p_type == TYPE_IPV4) {
				hints.ai_family = AF_INET;
			}
			else if (p_type == TYPE_IPV6) {
				hints.ai_family = AF_INET6;
				hints.ai_flags = 0;
			}
			else {
				hints.ai_family = AF_UNSPEC;
				hints.ai_flags = AI_ADDRCONFIG;
			}
			hints.ai_flags &= ~AI_NUMERICHOST;

			int s = getaddrinfo(p_hostname.data(), nullptr, &hints, &result);
			if (s != 0) 
			{
				LN_CORE_WARN("getaddrinfo failed! Cannot resolve hostname.");
				return;
			}

			if (result == nullptr || result->ai_addr == nullptr) 
			{
				LN_CORE_WARN("Invalid response from getaddrinfo");
				if (result) return freeaddrinfo(result);
			}

			struct addrinfo* next = result;

			do 
			{
				if (next->ai_addr == nullptr) 
				{
					next = next->ai_next;
					continue;
				}
				ipAddress ip = _sockaddr2ip(next->ai_addr);
				if (ip.isValid()) {
					r_addresses.push_back(ip);
				}
				next = next->ai_next;
			} while (next);

			freeaddrinfo(result);
		}

#if defined(LN_PLATFORM_WINDOWS)

		void Ip::getLocalInterfaces(std::unordered_map<std::string, Interface_Info>* r_interfaces) {
			ULONG buf_size = 1024;
			IP_ADAPTER_ADDRESSES* addrs;

			while (true) {
				addrs = (IP_ADAPTER_ADDRESSES*)malloc(buf_size);
				int err = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME,
					nullptr, addrs, &buf_size);
				if (err == NO_ERROR) {
					break;
				}
				free(addrs);
				if (err == ERROR_BUFFER_OVERFLOW) {
					continue; // will go back and alloc the right size
				}

				LN_ERR_FAIL_MSG("Call to GetAdaptersAddresses failed with error " + std::to_string(err) + ".");
			}

			IP_ADAPTER_ADDRESSES* adapter = addrs;

			while (adapter != nullptr) {
				Interface_Info info;
				info.name = adapter->AdapterName;
				std::wstring temp = std::wstring(adapter->FriendlyName);
				info.name_friendly = std::string(temp.begin(), temp.end());
				info.index = std::to_string(adapter->IfIndex);

				IP_ADAPTER_UNICAST_ADDRESS* address = adapter->FirstUnicastAddress;
				while (address != nullptr) {
					int family = address->Address.lpSockaddr->sa_family;
					if (family != AF_INET && family != AF_INET6) {
						continue;
					}
					info.ip_addresses.push_front(_sockaddr2ip(address->Address.lpSockaddr));
					address = address->Next;
				}
				adapter = adapter->Next;
				// Only add interface if it has at least one IP
				if (info.ip_addresses.size() > 0) {
					r_interfaces->insert({ info.name, info });
				}
			}

			free(addrs);
		}

#else // UNIX

		void IPUnix::get_local_interfaces(HashMap<String, Interface_Info>* r_interfaces) const {
			struct ifaddrs* ifAddrStruct = nullptr;
			struct ifaddrs* ifa = nullptr;
			int family;

			getifaddrs(&ifAddrStruct);

			for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
				if (!ifa->ifa_addr) {
					continue;
				}

				family = ifa->ifa_addr->sa_family;

				if (family != AF_INET && family != AF_INET6) {
					continue;
				}

				HashMap<String, Interface_Info>::Iterator E = r_interfaces->find(ifa->ifa_name);
				if (!E) {
					Interface_Info info;
					info.name = ifa->ifa_name;
					info.name_friendly = ifa->ifa_name;
					info.index = String::num_uint64(if_nametoindex(ifa->ifa_name));
					E = r_interfaces->insert(ifa->ifa_name, info);
					ERR_CONTINUE(!E);
				}

				Interface_Info& info = E->value;
				info.ip_addresses.push_front(_sockaddr2ip(ifa->ifa_addr));
			}

			if (ifAddrStruct != nullptr) {
				freeifaddrs(ifAddrStruct);
			}
		}

#endif // UNIX
#endif // UNIX_ENABLED || LN_PLATFORM_WINDOWS
#pragma endregion



	}
}
