#include "socket.h"
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>
namespace luna
{
	namespace networking 
	{
#pragma region Glue
		static entt::entity NetSocketCreate()
		{
			netSocket socket;
			socket.init(scripting::scriptingEngine::getContext());
			return socket;
		}
		static socketError NetSocketBind(entt::entity objectId, int port, MonoString* host, protocol proto)
		{
			return netSocket(objectId, scripting::scriptingEngine::getContext()).bind(port, mono_string_to_utf8(host), proto);
		}

		static socketError NetSocketConnectToHost(entt::entity objectId, int port, MonoString* host, protocol proto)
		{
			return netSocket(objectId, scripting::scriptingEngine::getContext()).connectToHost(port, mono_string_to_utf8(host), proto);
		}

		static void NetSocketDestroy(entt::entity objectId)
		{
			netSocket(objectId, scripting::scriptingEngine::getContext()).destroy();
		}

		static socketError NetSocketReceive(entt::entity objectId, MonoArray* byteArray, int length)
		{
			uint8_t* p_receieveBuffer = (uint8_t*)mono_array_addr_with_size(byteArray, length, 0);

			int read;
			return netSocket(objectId, scripting::scriptingEngine::getContext()).receive(p_receieveBuffer, length, read);
		}
		static socketError NetSocketReceiveFrom(entt::entity objectId, MonoArray* byteArray, MonoArray* addressArray, int length, uint16_t port, bool peek)
		{
			uint8_t* p_receieveBuffer = (uint8_t*)mono_array_addr_with_size(byteArray, length, 0);

			int read;
			ipAddress* address = (ipAddress*)mono_array_addr_with_size(addressArray, mono_array_length(addressArray), 0);
			return netSocket(objectId, scripting::scriptingEngine::getContext()).receiveFrom(p_receieveBuffer, length, read, *address, port, peek); //internall 
		}

		static socketError NetSocketSend(entt::entity objectId, MonoArray* byteArray, int len)
		{
			uint8_t* p_packetData = (uint8_t*)mono_array_addr_with_size(byteArray, len, 0);
			int r_sent = 0;
			return netSocket(objectId, scripting::scriptingEngine::getContext()).send(p_packetData, len, r_sent);
		}
		static socketError NetSocketSendTo(entt::entity objectId, MonoArray* byteArray, int len, MonoArray* addressArray, uint16_t port)
		{
			int r_sent = 0;
			uint8_t* p_packetData = (uint8_t*)mono_array_addr_with_size(byteArray, len, 0);
			ipAddress* p_address = (ipAddress*)mono_array_addr_with_size(addressArray, mono_array_length(addressArray), 0);
			return netSocket(objectId, scripting::scriptingEngine::getContext()).sendto(p_packetData, len, r_sent, *p_address, port);
		}

#pragma endregion
		size_t static _set_addr_storage(struct sockaddr_storage* p_addr,ipAddress& p_ip, uint16_t p_port, Ip::Type p_ip_type) {
			memset(p_addr, 0, sizeof(struct sockaddr_storage));
			if (p_ip_type == Ip::TYPE_IPV6 || p_ip_type == Ip::TYPE_ANY) { // IPv6 socket

				// IPv6 only socket with IPv4 address
				LN_ERR_FAIL_COND_V(!p_ip.isWildcard() && p_ip_type == Ip::TYPE_IPV6 && p_ip.isIpv4(), 0);

				struct sockaddr_in6* addr6 = (struct sockaddr_in6*)p_addr;
				addr6->sin6_family = AF_INET6;
				addr6->sin6_port = htons(p_port);
				if (p_ip.isValid()) {
					memcpy(&addr6->sin6_addr.s6_addr, p_ip.getIpv6(), 16);
				}
				else {
					addr6->sin6_addr = in6addr_any;
				}
				return sizeof(sockaddr_in6);
			}
			else { // IPv4 socket

				// IPv4 socket with IPv6 address
				LN_ERR_FAIL_COND_V(!p_ip.isWildcard() && !p_ip.isIpv4(), 0);

				struct sockaddr_in* addr4 = (struct sockaddr_in*)p_addr;
				addr4->sin_family = AF_INET;
				addr4->sin_port = htons(p_port); // short, network byte order
				if (p_ip.isValid()) {
					memcpy(&addr4->sin_addr.s_addr, p_ip.getIpv4(), 4);
				}
				else {
					addr4->sin_addr.s_addr = INADDR_ANY;
				}

				return sizeof(sockaddr_in);
			}
		}


		void netSocket::terminate() { WSACleanup(); }
		void netSocket::init(luna::scene* scene)
		{
			object::init(scene);
			addComponent<socketComponent>();
		}
		void netSocket::bindMethods()
		{
			LN_ADD_INTERNAL_CALL(netSocket, NetSocketCreate);
			LN_ADD_INTERNAL_CALL(netSocket, NetSocketDestroy);
			LN_ADD_INTERNAL_CALL(netSocket, NetSocketBind);
			LN_ADD_INTERNAL_CALL(netSocket, NetSocketConnectToHost);
			LN_ADD_INTERNAL_CALL(netSocket, NetSocketReceive);
			LN_ADD_INTERNAL_CALL(netSocket, NetSocketReceiveFrom);
		}

		void netSocket::destroy()
		{
			closesocket(getComponent<socketComponent>().netSocket);
			getComponent<socketComponent>().netSocket = INVALID_SOCKET;
		}

#ifdef LN_PLATFORM_WINDOWS

		static bool winSockStarted = false;
		static WSADATA wsaData;

		static socketError createSocket(socketComponent& socketData, const protocol proto)
		{

			LN_ERR_FAIL_COND_V_MSG(socketData.netSocket != INVALID_SOCKET, socketError::ALREADY_INIT, "you cannot initalize a socket twice!");

			if (!winSockStarted) {
				int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
				LN_ERR_FAIL_COND_V_MSG(iResult != 0, socketError::INIT_FAILED, "WSAStartup failed with error: " + std::to_string(iResult));
				winSockStarted = true;
			}

			struct addrinfo hints;
			
			ZeroMemory(&hints, sizeof(hints));

			int family = socketData.getType() == Ip::TYPE_IPV4 ? AF_INET : AF_INET6;
			int protocol = proto == TCP ? IPPROTO_TCP : IPPROTO_UDP;
			int type = proto == TCP ? SOCK_STREAM : SOCK_DGRAM;
			socketData.netSocket = ::socket(family,type, protocol);

			LN_ERR_FAIL_COND_V_MSG(socketData.netSocket == INVALID_SOCKET, socketError::INIT_FAILED, "socket creation failed with error: " + std::to_string(WSAGetLastError()));

			return socketError::SUCCESS;
		}
		_ALWAYS_INLINE_ static sockaddr_in* ipAddressNative(ipAddress& address, int port, uint16_t family)
		{
			sockaddr_in* service = new sockaddr_in();
			service->sin_family = family;
			service->sin_port = port;
			if (address.isWildcard()) service->sin_addr.s_addr = INADDR_ANY;
			else service->sin_addr.s_addr = inet_addr(((std::string)address).c_str());
			return service;
		}

		_ALWAYS_INLINE_ static int bindSocket(SOCKET sock, socketComponent& socketData, int port)
		{
			size_t addrSize = _set_addr_storage(&socketData.addr, socketData.address, port, socketData.getType());
			LN_CORE_INFO("addr = {0}", sizeof(sockaddr));
			return ::bind(sock, (struct sockaddr*)&socketData.addr, addrSize);
		}

		socketError netSocket::bind(int port, const std::string& host, const protocol proto)
		{
			auto& socketData = getComponent<socketComponent>();
			socketError result = createSocket(socketData, proto);

			LN_ERR_FAIL_COND_V_MSG(!(result == socketError::SUCCESS || result == socketError::ALREADY_INIT), result, "something went wrong during socket creation!");
			
			ipAddress address = ipAddress(host);
			if (!address.isWildcard() && !address.isValid()) address = Ip::resolveHostname(host);
			socketData.address = address;

			LN_ERR_FAIL_COND_V_MSG(!address.isValid() && !address.isWildcard(), socketError::INVALID_IP_ADDRESS, "ipAddress/ hostname was invalid!");
			
			LN_ERR_FAIL_COND_V_MSG(bindSocket(socketData.netSocket, socketData, port) != 0, socketError::BIND_FAILED, "could not bind socket: " + std::to_string(WSAGetLastError()));
			
			return socketError::SUCCESS;
		}

		_ALWAYS_INLINE_ static int connect(SOCKET sock, ipAddress& address, int port, uint16_t family,Ip::Type ipType)
		{
			struct sockaddr_storage addr;
			size_t addrSize = _set_addr_storage(&addr, address, port, ipType);

			return ::connect(sock, (sockaddr*)&addr, addrSize);
		}

		static void setIpPort(struct sockaddr_storage* p_addr, ipAddress* r_ip, uint16_t* r_port) {
			if (p_addr->ss_family == AF_INET) {
				struct sockaddr_in* addr4 = (struct sockaddr_in*)p_addr;
				if (r_ip) {
					r_ip->setIpv4((uint8_t*)&(addr4->sin_addr.s_addr));
				}
				if (r_port) {
					*r_port = ntohs(addr4->sin_port);
				}
			}
			else if (p_addr->ss_family == AF_INET6) {
				struct sockaddr_in6* addr6 = (struct sockaddr_in6*)p_addr;
				if (r_ip) {
					r_ip->setIpv6(addr6->sin6_addr.s6_addr);
				}
				if (r_port) {
					*r_port = ntohs(addr6->sin6_port);
				}
			}
		}


		socketError netSocket::connectToHost(int port, const std::string& host, const protocol proto)
		{
			auto& socketData = getComponent<socketComponent>();
			socketError result = createSocket(socketData, proto);

			LN_ERR_FAIL_COND_V_MSG(result != socketError::SUCCESS || result != socketError::ALREADY_INIT, result, "socket creation failed with error: " + std::to_string(WSAGetLastError()));

			LN_ERR_FAIL_COND_V_MSG(connect(socketData.netSocket, socketData.address, port, socketData.getFamily(),socketData.getType()) == SOCKET_ERROR, socketError::CONNECT_FAILED, "could not connect socket to host: " + std::to_string(WSAGetLastError()));

			return socketError();
		}

		socketError netSocket::receive(uint8_t* p_buffer, int len, int& r_read)
		{
			LN_ERR_FAIL_COND_V_MSG(!isOpen(), socketError::INIT_FAILED,"socket was not initialized");
			r_read = recv(getComponent<socketComponent>().netSocket, (char*)p_buffer, len, 0);
			if (r_read > 0) return socketError::SUCCESS;
			if (r_read == 0) return socketError::CONNECTION_CLOSED;
			else if (r_read == WSAEMSGSIZE) return socketError::OUT_OF_BUFFER_MEMORY;
			LN_CORE_ERROR("recv failed, errror:{0}", WSAGetLastError());
		}

		socketError netSocket::receiveFrom(uint8_t* p_buffer, int len, int& r_read, ipAddress& ipAddress, uint16_t& port, bool p_peek)
		{
			LN_ERR_FAIL_COND_V_MSG(!isOpen(), socketError::INIT_FAILED, "socket was not initialized");
			struct sockaddr_storage from;
			socklen_t sockLen = sizeof(struct sockaddr_storage);
			memset(&from, 0, sockLen);
			int result = recvfrom(getComponent<socketComponent>().netSocket, (char*)p_buffer, len, p_peek ? MSG_PEEK : 0, (struct sockaddr*)&from, &sockLen);
			if (result > 0) return socketError::SUCCESS;
			if (result == 0) return socketError::CONNECTION_CLOSED;
			else if (result == WSAEMSGSIZE) return socketError::OUT_OF_BUFFER_MEMORY;

			//get ipAddress
			if (from.ss_family == AF_INET) {
				struct sockaddr_in* sin_from = (struct sockaddr_in*)&from;
				ipAddress.setIpv4((uint8_t*)&sin_from->sin_addr);
				port = ntohs(sin_from->sin_port);
			}
			else if (from.ss_family == AF_INET6) {
				struct sockaddr_in6* s6_from = (struct sockaddr_in6*)&from;
				ipAddress.setIpv6((uint8_t*)&s6_from->sin6_addr);
				port = ntohs(s6_from->sin6_port);
			}
			else {
				// Unsupported socket family, should never happen.
				LN_ERR_FAIL_V_MSG(socketError::FAILED,"Unsupported socket family, should never happen");
			}
			return socketError::SUCCESS;
		}

		socketError netSocket::send(const uint8_t* p_buffer, int len, int& r_sent)
		{
			LN_ERR_FAIL_COND_V_MSG(!isOpen(), socketError::INIT_FAILED, "socket was not initialized");
			int flags = 0;
			#ifdef MSG_NOSIGNAL
						if (_is_stream) {
							flags = MSG_NOSIGNAL;
						}
			#endif
			r_sent = ::send(getComponent<socketComponent>().netSocket, (char*)p_buffer, len, flags);
			if (r_sent > 0) return socketError::SUCCESS;
			if (r_sent == 0) return socketError::CONNECTION_CLOSED;
			else if (r_sent == WSAEMSGSIZE) return socketError::OUT_OF_BUFFER_MEMORY;
			return socketError::SUCCESS;
		}

		_ALWAYS_INLINE_ static size_t setAddrStorage(struct sockaddr_storage* p_addr, ipAddress& p_ip, uint16_t p_port, Ip::Type p_ip_type) {
			memset(p_addr, 0, sizeof(struct sockaddr_storage));
			if (p_ip_type == Ip::TYPE_IPV6 || p_ip_type == Ip::TYPE_ANY) { // IPv6 socket

				// IPv6 only socket with IPv4 address
				LN_ERR_FAIL_COND_V(!p_ip.isWildcard() && p_ip_type == Ip::TYPE_IPV6 && p_ip.isIpv4(), 0);

				struct sockaddr_in6* addr6 = (struct sockaddr_in6*)p_addr;
				addr6->sin6_family = AF_INET6;
				addr6->sin6_port = htons(p_port);

				if (p_ip.isValid()) memcpy(&addr6->sin6_addr.s6_addr, p_ip.getIpv6(), 16);
				else addr6->sin6_addr = in6addr_any;

				return sizeof(sockaddr_in6);
			}
			else { // IPv4 socket

				// IPv4 socket with IPv6 address
				LN_ERR_FAIL_COND_V(!p_ip.isWildcard() && !p_ip.isIpv4(), 0);

				struct sockaddr_in* addr4 = (struct sockaddr_in*)p_addr;
				addr4->sin_family = AF_INET;
				addr4->sin_port = htons(p_port); // short, network byte order

				if (p_ip.isValid()) memcpy(&addr4->sin_addr.s_addr, p_ip.getIpv4(), 4);
				else addr4->sin_addr.s_addr = INADDR_ANY;

				return sizeof(sockaddr_in);
			}
		}
		socketError netSocket::sendto(const uint8_t* p_buffer, int p_len, int& r_sent, ipAddress p_ip, uint16_t p_port)
		{
			LN_ERR_FAIL_COND_V_MSG(!isOpen(), socketError::INIT_FAILED, "socket was not initialized");

			struct sockaddr_storage addr;
			size_t addrSize = setAddrStorage(&addr, p_ip, p_port,getComponent<socketComponent>().getType());
			r_sent = ::sendto(getComponent<socketComponent>().netSocket, (char*)p_buffer, p_len, 0, (sockaddr*)&addr, addrSize);

			if (r_sent > 0) return socketError::SUCCESS;
			if (r_sent == 0) return socketError::CONNECTION_CLOSED;
			else if (r_sent == WSAEMSGSIZE) return socketError::OUT_OF_BUFFER_MEMORY;
			return socketError::SUCCESS;
		}

		ref<netSocket> netSocket::accept(ipAddress& r_ip, uint16_t& r_port)
		{
			ref<netSocket> out;
			LN_ERR_FAIL_COND_V(!isOpen(), out);

			struct sockaddr_storage their_addr;
			socklen_t size = sizeof(their_addr);
			socketHandle fd = ::accept(getComponent<socketComponent>().netSocket, (struct sockaddr*)&their_addr, &size);
			LN_ERR_FAIL_COND_V_MSG(fd == INVALID_SOCKET, out, "something went wrong trting to accept an incomming socket! error code: " + std::to_string(WSAGetLastError())); 

			return ref<netSocket>();
		}

		socketError netSocket::listen(int maxPendding)
		{
			auto& socketData = getComponent<socketComponent>();
			int clampedPending = SOMAXCONN_HINT(maxPendding);
			if (clampedPending != maxPendding)LN_CORE_WARN("maxPendding count should be between 200 and 65535, addjusted to fit!");
			LN_ERR_FAIL_COND_V_MSG(::listen(socketData.netSocket, clampedPending) != SOCKET_ERROR, FAILED, "something went wrong when trying to listen on the socket, error: " + std::to_string(WSAGetLastError()));
			return SUCCESS;
		}

		bool netSocket::isOpen()
		{
			return getComponent<socketComponent>().netSocket != INVALID_SOCKET;
		}

		int netSocket::getAvailableBytes()
		{
			unsigned long len;
			int result = ::ioctlsocket(getComponent<socketComponent>().netSocket, FIONREAD, &len);
			LN_ERR_FAIL_COND_V_MSG(result != NO_ERROR, 0, "an error occured while getting available bytes!");
			return len;
		}

		socketError netSocket::poll(pollType p_type, int p_timeout)
		{
			bool ready = false;
			fd_set rd, wr, ex;
			fd_set* rdp = nullptr;
			fd_set* wrp = nullptr;
			SOCKET _sock = getComponent<socketComponent>().netSocket;
			FD_ZERO(&rd);
			FD_ZERO(&wr);
			FD_ZERO(&ex);
			FD_SET(_sock, &ex);
			struct timeval timeout = { -p_timeout / 1000, (p_timeout % 1000) * 1000 };
			// For blocking operation, pass nullptr timeout pointer to select.
			struct timeval* tp = nullptr;
			if (p_timeout >= 0) {
				//  If timeout is non-negative, we want to specify the timeout instead.
				tp = &timeout;
			}

			switch (p_type) {
			case POLL_TYPE_IN:
				FD_SET(_sock, &rd);
				rdp = &rd;
				break;
			case POLL_TYPE_OUT:
				FD_SET(_sock, &wr);
				wrp = &wr;
				break;
			case POLL_TYPE_IN_OUT:
				FD_SET(_sock, &rd);
				FD_SET(_sock, &wr);
				rdp = &rd;
				wrp = &wr;
			}
			int ret = select(1, rdp, wrp, &ex, tp);

			if (ret == SOCKET_ERROR) {
				return FAILED;
			}

			if (ret == 0) {
				return BUSY;
			}

			if (FD_ISSET(_sock, &ex)) {
				LN_CORE_ERROR("poll failed, errror:{0}", WSAGetLastError());
				return FAILED;
			}

			if (rdp && FD_ISSET(_sock, rdp)) {
				ready = true;
			}
			if (wrp && FD_ISSET(_sock, wrp)) {
				ready = true;
			}

			return ready ? SUCCESS : BUSY;
		}

		void netSocket::setSocket(socketHandle handle, ipAddress address, int port)
		{
			auto& socketData =  getComponent<socketComponent>();
			socketData.address = address;
			socketData.netSocket = handle;

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
				LN_CORE_ERROR("shit happend! goodluck");
			}
			return 0;
		}

#endif 	//LN_PLATFORM_WINDOWS
		uint16_t socketComponent::getFamily()
		{
			if (address.isIpv4()) return AF_INET;
			else return AF_INET6;
		}

		Ip::Type socketComponent::getType()
		{
			
			return address.isIpv4() ? Ip::TYPE_IPV4: Ip::TYPE_IPV6;
		}
	}
}