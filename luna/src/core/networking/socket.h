#include <core/object/objectDB.h>
#include <core/scene/scene.h>
#include <core/networking/Ip.h>

#if defined(UNIX_ENABLED) || defined(LN_PLATFORM_WINDOWS)
	#ifdef LN_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <ws2def.h>
	#include <iphlpapi.h>
	#pragma comment(lib, "Ws2_32.lib") //ws2tcip
	#pragma comment(lib, "IPHLPAPI.lib") //iphlpapi
	using socketHandle = SOCKET;
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
#endif
namespace luna 
{
	namespace networking 
	{
		enum socketError {
			SUCCESS,
			INIT_FAILED,
			BIND_FAILED,
			CONNECT_FAILED,
			INVALID_IP_ADDRESS,
			ALREADY_INIT
		};

		enum protocol {
			TCP, // Transmission Control Protocol
			UDP // Unicersal Datagram Protocol
		};

	
		struct socketComponent {
			ipAddress address;
			socketHandle netSocket = INVALID_SOCKET;
			std::string host;
			protocol proto;
			uint16_t getFamily();
		};
		class netSocket : public object
		{
		public:
			netSocket() = default;
			netSocket(entt::entity handle, luna::scene* scene) : object(handle, scene) {};
			netSocket(uint64_t id, luna::scene* scene) { LN_CORE_WARN("DEPRECATED"); };
			virtual void init(luna::scene* scene);
			virtual	void bindMethods();
			virtual void destroy();
			virtual socketError bind(int port, const std::string& host,const protocol proto);
			virtual socketError connectToHost(int port, const std::string& host, const protocol proto);
			int getLocalPort();
			int getConnectedPort();
		private:
			
		};

		
	}
}