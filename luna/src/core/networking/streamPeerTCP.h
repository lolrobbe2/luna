#pragma once
#include <core/networking/socket.h>
namespace luna 
{
	namespace networking
	{
		enum status 
		{
			STATUS_NONE,
			STATUS_CONNECTING,
			STATUS_CONNECTED,
			STATUS_ERROR,
		};

		class streamPeerTCP : netSocket
		{
		public:
			streamPeerTCP() = default;
			streamPeerTCP(entt::entity handle, luna::scene* scene) : netSocket(handle, scene) {};
			streamPeerTCP(uint64_t id, luna::scene* scene) { LN_CORE_WARN("DEPRECATED"); };
			virtual socketError poll();
			virtual void disconnectFromHost();
			virtual socketError connectToHost(const ipAddress& host, int port);
		protected:
			friend class TCPServer;
			void setSocket(ref<netSocket> socket);
		};
	}
}