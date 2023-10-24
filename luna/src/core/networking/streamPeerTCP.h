#pragma once
/**
* disclaimer this class is inspired by the godot equivelant.
*/
#include <core/networking/socket.h>
namespace luna 
{
	namespace networking
	{
		using status = int;

		enum status 
		{
			STATUS_NONE,
			STATUS_CONNECTING,
			STATUS_CONNECTED,
			STATUS_ERROR,
		};

		class streamPeerTCP : netSocket //no public(keyword) is used to restrict externall usage of the netSocket functions
		{
		public:
			streamPeerTCP() = default;
			streamPeerTCP(entt::entity handle, luna::scene* scene) : netSocket(handle, scene) {};
			streamPeerTCP(uint64_t id, luna::scene* scene) { LN_CORE_WARN("DEPRECATED"); };
			virtual void bindMethods() override;
			virtual void init(luna::scene* scene);
			virtual socketError poll();
			socketError write(const uint8_t* data, int bytes, int& sent, bool block);
			socketError read(uint8_t* buffer, int bytes, int& received, bool block);
			void setNoDelay(bool enabled);
			status getStatus();
			socketError wait(pollType type, int timeout = 0);

			// Read/Write from StreamPeer
			socketError putData(const uint8_t* data, int bytes);
			socketError putPartialData(const uint8_t* data, int p_bytes, int& sent);
			socketError getData(uint8_t* buffer, int bytes);
			socketError getPartialData(uint8_t* buffer, int bytes, int& received);

			//bind /connect
			socketError bind(int port, const ipAddress& host);
			virtual void disconnectFromHost();
			virtual int getConnectedPort();
			virtual socketError connectToHost(const ipAddress& host, int port);

			virtual int getLocalPort();
			//get info
			int getAvailableBytes();
			ipAddress getConnectedhost();
			operator entt::entity() { return entityHandle; } //needs to be done because netsocket functions cannot be externally accesible only internally

		protected:
			friend class TCPServer;
			void setSocket(ref<netSocket> socket);
		};
	}
}