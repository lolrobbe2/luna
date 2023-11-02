#include "HTTPClient.h"
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>
namespace luna 
{
	namespace networking 
	{
#pragma region glue 
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
		enum class value_t : std::uint8_t
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
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientConnectToHost);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientRequest);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientHasResponse);
		}

		socketError HTTPClient::connectToHost(const std::string& hostName, int port)
		{
			ipAddress address(hostName);
			ipAddress finalizedAddress = address.isValid() || address.isWildcard() ? address : Ip::resolveHostname(hostName);
			return streamPeerTCP::connectToHost(finalizedAddress, port);
		}
		void HTTPClient::request(const method requestMethod,const std::string& destination, utils::json headers, std::string body)
		{
			std::string requestString = generateRequest(requestMethod, destination, headers, body);
			streamPeerTCP::putData((const uint8_t*)requestString.data(), requestString.size());
		}
		bool HTTPClient::hasResponse()
		{
			streamPeerTCP::poll();
			if (streamPeerTCP::getStatus() == STATUS_CONNECTED)
			{
				return streamPeerTCP::getAvailableBytes();
			} 
			else if(streamPeerTCP::getStatus() == STATUS_ERROR) LN_CORE_ERROR("[HTTP] an error occured while polling the http client!");
			return false;
		}
		const std::string& HTTPClient::generateRequest(const method requestMethod, const std::string& destination, utils::json headers, std::string body)
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
			requestBody += " ";
			requestBody += destination;

			//HTTP version
			requestBody += " HTTP/1.1 \n";
			
			//MANDATORY HEADERS
			//HTTP host
			requestBody += "Host: ";
			requestBody += streamPeerTCP::getConnectedhost(); +":" + streamPeerTCP::getConnectedPort();
			requestBody += "\n";

			nlohmann::json nativeJsonHeaders = headers;
			for (auto jsonVal : nativeJsonHeaders)
			{
				//add header key
				std::string key = jsonVal;
				requestBody += key + ":";

				//add header key value
				switch ((value_t)jsonVal.type())
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
					requestBody += jsonVal.get<std::string>();
					break;
				case value_t::boolean:
					requestBody += jsonVal.get<bool>();
					break;
				case value_t::number_integer:
					requestBody += jsonVal.get<int>();
					break;
				case value_t::number_unsigned:
					requestBody += jsonVal.get<unsigned int>();
					break;
				case value_t::number_float:
					requestBody += jsonVal.get<float>();
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
			}

			if(requestMethod != method::GET) //GET cannot have a body
			{
				requestBody += body;
			}
			
			return requestBody;
		}
	}
#pragma endregion
}

