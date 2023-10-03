#include "socket.h"
namespace luna
{
	namespace networking 
	{
		void netSocket::init(luna::scene* scene)
		{
			object::init(scene);
			addComponent<socketComponent>();
		}
		void netSocket::bindMethods()
		{
		}
#ifdef LN_PLATFORM_WINDOWS

		static bool winSockStarted = false;
		static WSADATA wsaData;

		socketError netSocket::bind(int port, const std::string& host, const protocol proto)
		{
			if (!winSockStarted) {
				int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
				LN_ERR_FAIL_COND_V_MSG(iResult != 0,socketError::INIT_FAILED, "WSAStartup failed with error: " + std::to_string(iResult));
				winSockStarted = true;
			}

			struct addrinfo* result = NULL;
			struct addrinfo hints;

			socketComponent& socketData = getComponent<socketComponent>();

			ipAddress address = Ip::resolveHostname(host);

			ZeroMemory(&hints, sizeof(hints));
			if(address.isIpv4()) hints.ai_family = AF_INET;
			else hints.ai_family = AF_INET6;
			

			switch (proto)
			{
			case luna::networking::TCP:
				hints.ai_protocol = IPPROTO_TCP;
				break;
			case luna::networking::UDP:
				break;
			default:
				break;
			}
			hints.ai_socktype = SOCK_STREAM;

			hints.ai_flags = AI_PASSIVE;
			SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			
			LN_ERR_FAIL_COND_V_MSG(sock == INVALID_SOCKET, socketError::INIT_FAILED, "socket creation failed with error: " + std::to_string(WSAGetLastError()));
			
			LN_ERR_FAIL_COND_V_MSG(bindSocket(sock, address, port, hints.ai_family) == SOCKET_ERROR, socketError::BIND_FAILED, "could not bind socket: " + std::to_string(WSAGetLastError()));

			return socketError::SUCCESS;
		}
		static int bindSocket(SOCKET sock,ipAddress& address,int port,uint16_t family) 
		{

			sockaddr_in* serverAddr = new sockaddr_in();
			serverAddr->sin_family = family;
			serverAddr->sin_port = port;
			if (address.isIpv4()) memcpy(&serverAddr->sin_addr, address.getIpv4(), sizeof(IN_ADDR));
			else memcpy(&serverAddr->sin_addr, address.getIpv6(), sizeof(IN6_ADDR)); ;
			
			return bind(sock, (sockaddr*)serverAddr, sizeof(serverAddr));
		}
#endif // LN_PLATFORM_WINDOWS
	}
}