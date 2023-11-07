#pragma once
#include <core/networking/streamPeerTCP.h>
#include <core/networking/web/JSON.h>
namespace luna 
{
	namespace networking
	{
		class HTTPClient : streamPeerTCP
		{
		public:
			
			enum status : int
			{
				STATUS_NONE,
				STATUS_CONNECTING,
				STATUS_CONNECTED,
				STATUS_ERROR,
				STATUS_REQUESTING,
				STATUS_RECEIVING
			};

			enum method
			{
				GET,
				POST,
				PUT,
				HEAD,
				_DELETE, //DELETE ALREADY DEFINED!
				_PATCH, //PATCH ALREADY DEFINED!
				OPTIONS,
				CONNECT,
				TRACE,

			};

			OBJ_ROOT(streamPeerTCP);

			HTTPClient() = default;
			HTTPClient(entt::entity handle, luna::scene* scene) : streamPeerTCP(handle, scene) {};

			virtual void bindMethods() override;
			virtual void init(luna::scene* scene);
			/**
			* @brief connectToHost connects the htpp client to a corresponding host for example: example.com. Or 127.0.0.1.
			* @param const std::string& hostName can be a hostname (dns) or a ipAddress.
			* @param int port = 80 should not be touched unless HTTPS is used (443).
			*/
			socketError connectToHost(const std::string& hostName, int port = 80);
			/**
			* @brief sends a request to the connected host
			*/
			void request(const method requestMethod,const std::string& destination,utils::json headers, std::string body);
			//TODO json 
			bool hasResponse();
			virtual socketError poll() override;
			virtual status getStatus();
			operator entt::entity() { return *(streamPeerTCP*)this; } //needs to be done because streamPeerTCP functions cannot be externally accesible only internally
			friend class utils::json;
		private:
			std::string generateRequest(const method requestMethod, const std::string& destination, utils::json headers,std::string body);
			void parseHeaders(const std::string& headersString);
		};
	}
}


