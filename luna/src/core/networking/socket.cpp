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

		void netSocket::destroy()
		{
			closesocket(getComponent<socketComponent>().netSocket);
		}

#ifdef LN_PLATFORM_WINDOWS

		static bool winSockStarted = false;
		static WSADATA wsaData;

		static socketError createSocket(socketComponent& socketData, const std::string& host, const protocol proto)
		{

			LN_ERR_FAIL_COND_V_MSG(socketData.netSocket == INVALID_SOCKET, socketError::ALREADY_INIT, "you cannot initalize a socket twice!");

			if (!winSockStarted) {
				int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
				LN_ERR_FAIL_COND_V_MSG(iResult != 0, socketError::INIT_FAILED, "WSAStartup failed with error: " + std::to_string(iResult));
				winSockStarted = true;
			}

			struct addrinfo* result = NULL;
			struct addrinfo hints;

			ipAddress address = Ip::resolveHostname(host);

			LN_ERR_FAIL_COND_V_MSG(address.isValid(), socketError::INVALID_IP_ADDRESS, "ipAddress/ hostname was invalid!");

			ZeroMemory(&hints, sizeof(hints));



			switch (proto)
			{
			case luna::networking::TCP:
				hints.ai_protocol = IPPROTO_TCP;
				break;
			case luna::networking::UDP:
				hints.ai_protocol = IPPROTO_UDP;
				break;
			default:
				LN_CORE_ERROR("invalid protocol selected!");
				return socketError::INIT_FAILED;
			}
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;
			SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			LN_ERR_FAIL_COND_V_MSG(sock == INVALID_SOCKET, socketError::INIT_FAILED, "socket creation failed with error: " + std::to_string(WSAGetLastError()));
			socketData.netSocket = sock;
			return socketError::SUCCESS;
		}


		static int bindSocket(SOCKET sock, ipAddress& address, int port, uint16_t family)
		{

			sockaddr_in* serverAddr = new sockaddr_in();
			serverAddr->sin_family = family;
			serverAddr->sin_port = port;
			if (address.isIpv4()) memcpy(&serverAddr->sin_addr, address.getIpv4(), sizeof(IN_ADDR));
			else memcpy(&serverAddr->sin_addr, address.getIpv6(), sizeof(IN6_ADDR)); ;

			return bind(sock, (sockaddr*)serverAddr, sizeof(serverAddr));
		}

		socketError netSocket::bind(int port, const std::string& host, const protocol proto)
		{
			auto& socketData = getComponent<socketComponent>();
			socketError result = createSocket(socketData, host, proto);

			LN_ERR_FAIL_COND_V_MSG(result != socketError::SUCCESS || result != socketError::ALREADY_INIT, result, "something went wrong during socket creation!");
			
			LN_ERR_FAIL_COND_V_MSG(bindSocket(socketData.netSocket, socketData.address, port, socketData.getFamily()) == SOCKET_ERROR, socketError::BIND_FAILED, "could not bind socket: " + std::to_string(WSAGetLastError()));

			return socketError::SUCCESS;
		}

		static int connect(SOCKET sock, ipAddress& address, int port, uint16_t family)
		{
			sockaddr_in* serverAddr = new sockaddr_in();
			serverAddr->sin_family = family;
			serverAddr->sin_port = port;
			if (address.isIpv4()) memcpy(&serverAddr->sin_addr, address.getIpv4(), sizeof(IN_ADDR));
			else memcpy(&serverAddr->sin_addr, address.getIpv6(), sizeof(IN6_ADDR));

			return connect(sock, (sockaddr*)serverAddr, sizeof(serverAddr));
		}

		socketError netSocket::connectToHost(int port, const std::string& host, const protocol proto)
		{
			auto& socketData = getComponent<socketComponent>();
			socketError result = createSocket(socketData, host, proto);

			LN_ERR_FAIL_COND_V_MSG(result != socketError::SUCCESS || result != socketError::ALREADY_INIT, result, "socket creation failed with error: " + std::to_string(WSAGetLastError()));

			LN_ERR_FAIL_COND_V_MSG(connect(socketData.netSocket, socketData.address, port, socketData.getFamily()) == SOCKET_ERROR, socketError::CONNECT_FAILED, "could not connect socket to host: " + std::to_string(WSAGetLastError()));

			return socketError();
		}

		int netSocket::getLocalPort()
		{
			struct sockaddr_in sin;
			int addrlen = sizeof(sin);
			if (getsockname(getComponent<socketComponent>().netSocket, (struct sockaddr*)&sin, &addrlen) == 0 && sin.sin_family == getComponent<socketComponent>().getFamily() && addrlen == sizeof(sin))
			{
				return ntohs(sin.sin_port);
			}
			else 
			{
				LN_CORE_ERROR("shit happend!");
			}
			return 0;
		}

	

		
#endif 
		uint16_t socketComponent::getFamily()
		{
			if (address.isIpv4())return AF_INET;
			else return AF_INET6;
		}
		// LN_PLATFORM_WINDOWS
	}
}