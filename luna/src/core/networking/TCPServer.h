#pragma once
#include <core/networking/streamPeerTCP.h>
namespace luna 
{
	namespace networking
	{
		class TCPServer : netSocket
		{
			TCPServer() = default;
			TCPServer(entt::entity handle, luna::scene* scene) : netSocket(handle, scene) {};
			TCPServer(uint64_t id, luna::scene* scene) { LN_CORE_WARN("DEPRECATED"); };
			virtual void init(luna::scene* scene);
			virtual	void bindMethods();
			virtual socketError listen(int port, std::string bindingAddress = "*");
			bool isConnectionAvailable();
			ref<streamPeerTCP> takeIncommingConnection();
		};
	}
}


