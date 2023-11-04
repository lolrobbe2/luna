#include "HTTPClient.h"
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>
namespace luna 
{
	namespace networking 
	{
		struct HTTPResponseComponent 
		{

		};
#pragma region glue 
		static entt::entity HTTPClientCreate()
		{
			HTTPClient httpClient;
			httpClient.init(scripting::scriptingEngine::getContext());
			return httpClient;
		}
		static socketError HTTPClientConnectToHost(entt::entity objectId,MonoString* hostName,int port)
		{
			return OBJ_GET(HTTPClient).connectToHost(mono_string_to_utf8(hostName), port);
		}
		static void HTTPClientRequest(entt::entity objectId, const HTTPClient::method method, MonoString* destination, MonoString* headersJsonString, MonoString* body)
		{
			OBJ_GET(HTTPClient).request(method, mono_string_to_utf8(destination), std::string(mono_string_to_utf8(headersJsonString)), mono_string_to_utf8(body));
		}

		static bool HTTPClientHasResponse(entt::entity objectId) 
		{
			return OBJ_GET(HTTPClient).hasResponse();
		}
#pragma endregion

#pragma region native implmentation
		enum class value_t
		{
			null,             ///< null value
			object,           ///< object (unordered set of name/value pairs)
			array,            ///< array (ordered collection of values)
			string,           ///< string value
			boolean,          ///< boolean value
			number_integer,   ///< number value (signed integer)
			number_unsigned,  ///< number value (unsigned integer)
			number_float,     ///< number value (floating-point)
			binary,           ///< binary array (ordered collection of bytes)
			discarded         ///< discarded by the parser callback function
		};

		void HTTPClient::bindMethods()
		{
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientCreate);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientConnectToHost);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientRequest);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientHasResponse);
	
		}

		void HTTPClient::init(luna::scene* scene)
		{
			streamPeerTCP::init(scene);
		}

		socketError HTTPClient::connectToHost(const std::string& hostName, int port)
		{
			ipAddress address(hostName);
			ipAddress finalizedAddress = address.isValid() || address.isWildcard() ? address : Ip::resolveHostname(hostName);
			return streamPeerTCP::connectToHost(finalizedAddress, port);
		}
		void HTTPClient::request(const method requestMethod,const std::string& destination, utils::json headers, std::string body)
		{
			while(streamPeerTCP::getStatus() != STATUS_CONNECTED)
			{
				if (streamPeerTCP::getStatus() == STATUS_ERROR) return LN_CORE_ERROR("[HTTP]error host was not connected!");
				streamPeerTCP::poll();
			}
			std::string requestString = generateRequest(requestMethod, destination, headers, body);
			LN_CORE_INFO("\r\n" + requestString);
			socketError error = streamPeerTCP::putData((const uint8_t*)requestString.data(), requestString.size());
			LN_ERR_FAIL_COND_MSG(error != SUCCESS, "could not send reqeust, reason: " + std::to_string(error));
		}

		socketError HTTPClient::getHttpData(uint8_t* p_buffer, int p_bytes, int& r_received) {
			if (streamPeerTCP::isBlocking()) {
				// We can't use StreamPeer.get_data, since when reaching EOF we will get an
				// error without knowing how many bytes we received.
				socketError err = socketError::OUT_OF_BUFFER_MEMORY;
				int read = 0;
				int left = p_bytes;
				r_received = 0;
				while (left > 0) {
					err = streamPeerTCP::getPartialData(p_buffer + r_received, left, read);
					if (err == SUCCESS) {
						r_received += read;
					}
					else if (err == OUT_OF_BUFFER_MEMORY) {
						r_received += read;
						return err;
					}
					else {
						return err;
					}
					left -= read;
				}
				return err;
			}
			else {
				return streamPeerTCP::getPartialData(p_buffer, p_bytes, r_received);
			}
		}

		bool HTTPClient::hasResponse()
		{
			
			if (streamPeerTCP::getStatus() == STATUS_CONNECTED)
			{
				std::vector<std::uint8_t> responseData;
				std::array<std::uint8_t, 4096> tempBuffer{ 0 };
				int received;
				for(;;)
				{
					//NOTE http responses are delivered in chunks so read them in chunks!
					socketError error = streamPeerTCP::getPartialData(tempBuffer.data(), tempBuffer.size(), received);
					if (error == FAILED) return false;
					responseData.insert(responseData.end(), tempBuffer.begin(), tempBuffer.begin() + received);
					if (received == 0 && responseData.size() != 0) { // disconnected
						LN_CORE_INFO(std::string((char*)responseData.data()));
						return true;
					}
				}
					
			} 
			else if(streamPeerTCP::getStatus() == STATUS_ERROR) LN_CORE_ERROR("[HTTP] an error occured while polling the http client!");
			else if(streamPeerTCP::getStatus() == STATUS_CONNECTING) 
			{
				streamPeerTCP::poll();
			}
			return false;
		}
		std::string HTTPClient::generateRequest(const method requestMethod, const std::string& destination, utils::json headers, std::string body)
		{
			std::string requestBody;

			// add method
			switch (requestMethod)
			{
			case luna::networking::HTTPClient::GET:
				requestBody += "GET";
				break;
			case luna::networking::HTTPClient::POST:
				requestBody += "POST";
				break;
			case luna::networking::HTTPClient::PUT:
				requestBody += "PUT";
				break;
			case luna::networking::HTTPClient::HEAD:
				requestBody += "HEAD";
				break;
			case luna::networking::HTTPClient::_DELETE:
				requestBody += "DELETE";
				break;
			case luna::networking::HTTPClient::_PATCH:
				requestBody += "PATCH";
				break;
			case luna::networking::HTTPClient::OPTIONS:
				requestBody += "OPTIONS";
				break;
			case luna::networking::HTTPClient::CONNECT:
				requestBody += "CONNECT";
				break;
			case luna::networking::HTTPClient::TRACE:
				requestBody += "TRACE";
				break;
			default:
				LN_CORE_ERROR("[HTTP] request method not found {0}", requestMethod);
				break;
			}
			//destination
			if (destination.size()) 
			{
				requestBody += " ";
				requestBody += destination;
			}

			//HTTP version
			requestBody += " HTTP/1.1\r\n";
			
			//MANDATORY HEADERS
			//HTTP host
			requestBody += "Host: " + Ip::getHostnameFromIP(streamPeerTCP::getConnectedhost()) + "\r\n";

			///content lenght
			if(body.size()) requestBody += "Content-Length: " + std::to_string(body.size()) + "\r\n";
			nlohmann::json nativeJsonHeaders = headers.jsonData;
			for (auto jsonVal : nativeJsonHeaders.items())
			{
				//add header key
				std::string key = jsonVal.key();
				requestBody += key + ": ";

				//add header key value
				switch ((value_t)jsonVal.value().type())
				{
				case value_t::null:
					break;
				case value_t::object:
					LN_CORE_ERROR("[HTTP] json value type object not supported as header value type");
					break;
				case value_t::array:
					LN_CORE_ERROR("[HTTP] json value type array not supported as header value type");
					break;
				case value_t::string:
					requestBody += jsonVal.value().get<std::string>();
					break;
				case value_t::boolean:
					requestBody += jsonVal.value().get<bool>();
					break;
				case value_t::number_integer:
					requestBody += jsonVal.value().get<int>();
					break;
				case value_t::number_unsigned:
					requestBody += jsonVal.value().get<unsigned int>();
					break;
				case value_t::number_float:
					requestBody += jsonVal.value().get<float>();
					break;
				case value_t::binary:
					LN_CORE_ERROR("[HTTP] json value type binary not supported as header value type");
					break;
				case value_t::discarded:
					LN_CORE_ERROR("[HTTP] how even did this happen!");
					break;
				default:
					LN_CORE_ERROR("[HTTP] json value type default not supported as header value type");
					break;
				}
				requestBody += "\r\n";
			}

			if(requestMethod != method::GET) //GET cannot have a body
			{
				requestBody += body;
			}
			requestBody += "\r\n";
			return requestBody;
		}
	}
#pragma endregion
}

