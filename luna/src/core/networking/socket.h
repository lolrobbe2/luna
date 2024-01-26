#include <core/object/objectDB.h>
#include <core/scene/scene.h>
#include <core/networking/Ip.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#if defined(UNIX_ENABLED) || defined(LN_PLATFORM_WINDOWS)
	#ifdef LN_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#include <ws2tcpip.h>
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
#ifdef LN_PLATFORM_WINDOWS
	static std::string getWinsockError()
	{
		int eno = WSAGetLastError();
		char erbuf[40];
		LN_CORE_INFO("socket error code: {0}", eno);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
			NULL, eno, 0, erbuf, sizeof(erbuf), NULL); OutputDebugStringA(erbuf);
		return std::string(erbuf);
	}
	static std::string getWinsockError(int eno)
	{
		char erbuf[40];
		LN_CORE_INFO("socket error code: {0}", eno);
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
			NULL, eno, 0, erbuf, sizeof(erbuf), NULL); OutputDebugStringA(erbuf);
		return std::string(erbuf);
	}
#endif
	namespace networking 
	{
		enum socketError {
			SUCCESS,
			FAILED,
			INIT_FAILED,
			BIND_FAILED,
			CONNECT_FAILED,
			INVALID_IP_ADDRESS,
			ALREADY_INIT,
			CONNECTION_CLOSED,
			OUT_OF_BUFFER_MEMORY,
			BUSY,
			SOCKET_INVALID,
			FILE_EOF
		};

		enum protocol {
			NONE,
			TCP, // Transmission Control Protocol
			UDP // Unicersal Datagram Protocol
		};

		enum pollType {
			POLL_TYPE_IN,
			POLL_TYPE_OUT,
			POLL_TYPE_IN_OUT
		};

	
		struct socketComponent {
			struct sockaddr_storage addr;
			bool open;
			bool blocking = true;
			ipAddress address;
			uint16_t port;
			socketHandle netSocket = INVALID_SOCKET;
			protocol proto;
			uint16_t getFamily();
			Ip::Type getType();
		};

		struct tcpComponent
		{
			ipAddress peerHost;
			uint16_t peerPort;
			int status;
			int timeout;
		};
		class netSocket : public object
		{
		public:
			netSocket() = default;
			netSocket(entt::entity handle, luna::scene* scene) : object(handle, scene) {};
			netSocket(uint64_t id, luna::scene* scene) { LN_CORE_WARN("DEPRECATED"); };
			static void terminate();
			virtual void init(luna::scene* scene);
			virtual	void bindMethods();
			virtual void destroy();
			virtual socketError open(const protocol proto);
			virtual socketError bind(int port, const std::string& host,const protocol proto);
			virtual socketError connectToHost(int port, const std::string& host);
			virtual socketError receive(uint8_t* p_buffer, int p_len, int& r_read);
			virtual socketError receiveFrom(uint8_t* p_buffer, int len, int& r_read, ipAddress& r_ip, uint16_t& r_port, bool p_peek = false);
			virtual socketError send(const uint8_t* p_buffer, int len, int& r_sent);
			virtual socketError sendto(const uint8_t* p_buffer, int len, int& r_sent, ipAddress p_ip, uint16_t p_port);
			virtual ref<netSocket> accept(ipAddress& r_ip, uint16_t& r_port);
			virtual socketError listen(int maxPendding);
			virtual bool isValid();
			virtual bool isOpen();
			virtual bool isBlocking();
			virtual int getAvailableBytes();
			virtual socketError poll(pollType p_type, int timeout);	
			void setSocket(socketHandle handle, ipAddress address);
			int getLocalPort();
			int getConnectedPort();
			void setReuseAddressEnabled(bool p_enabled);
			void setBlockingEnabled(bool enabled);
			void setTcpNoDelayEnabled(bool enabled);

		private:
			
		};

		
	}
}