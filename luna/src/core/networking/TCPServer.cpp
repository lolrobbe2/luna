#include "TCPServer.h"
#define SOCKET_ERROR(result) {socketError res = result;if (res != SUCCESS){ netSocket::destroy();return res;}}
namespace luna 
{
	namespace networking
	{
		socketError TCPServer::listen(int port, std::string bindingAddress)
		{
			netSocket::setReuseAddressEnabled(true);
			SOCKET_ERROR(netSocket::bind(port, bindingAddress, TCP));
			SOCKET_ERROR(netSocket::listen(500));//todo settings for max incoming connections.
			return SUCCESS();
		}
		bool TCPServer::isConnectionAvailable()
		{ 
			return (netSocket::poll(POLL_TYPE_IN, 0) == SUCCESS);
		}

		ref<streamPeerTCP> TCPServer::takeIncommingConnection()
		{
			ref<streamPeerTCP> out = createRef<streamPeerTCP>();

			if (!isConnectionAvailable()) return out;
			ipAddress incommingAddress;
			uint16_t port = 0;

			ref<netSocket> incommingSocket = netSocket::accept(incommingAddress, port);
			out->setSocket(incommingSocket);

			return out;
		}
	}
}