#include "HTTPClient.h"
#include <core/object/methodDB.h>
#include <core/scripting/scriptingEngine.h>
namespace luna 
{
	namespace networking 
	{
		struct HTTPResponseComponent 
		{
			HTTPClient::status httpstatus;
			std::vector<uint8_t> data;
			std::string message;
			uint16_t responseCode; //TODO enum;
			utils::json headers;
			bool readingHeaders = true;
			uint64_t bodyStartIndex;
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

		static socketError HTTPClientPoll(entt::entity objectId)
		{
			return OBJ_GET(HTTPClient).poll();
		}

		static HTTPClient::status HTTPClientGetStatus(entt::entity objectId) 
		{
			return OBJ_GET(HTTPClient).getStatus();
		}

		static MonoArray* HTTPClientGetBody(entt::entity objectId)
		{
			std::vector<uint8_t> body = OBJ_GET(HTTPClient).getBody();
			MonoArray* monoBody = mono_array_new(mono_get_root_domain(), mono_get_byte_class(), body.size());
			char* nativeArray = mono_array_addr_with_size(monoBody, body.size(), 0);
			memcpy_s(nativeArray, mono_array_length(monoBody), body.data(), body.size());
			return monoBody;
		}

		static MonoString* HTTPClientGetHeaders(entt::entity objectId)
		{
			return mono_string_new(mono_get_root_domain(), OBJ_GET(HTTPClient).getHeaders().jsonData.dump().c_str());
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
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientPoll);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientGetStatus);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientGetBody);
			LN_ADD_INTERNAL_CALL(HTTPClient, HTTPClientGetHeaders);
		}

		void HTTPClient::init(luna::scene* scene)
		{
			streamPeerTCP::init(scene);
			streamPeerTCP::addComponent<HTTPResponseComponent>();
		}

		socketError HTTPClient::connectToHost(const std::string& hostName, int port)
		{
			ipAddress address(hostName);
			ipAddress finalizedAddress = address.isValid() || address.isWildcard() ? address : Ip::resolveHostname(hostName);
			return streamPeerTCP::connectToHost(finalizedAddress, port);
		}
		void HTTPClient::request(const method requestMethod, const std::string& destination, utils::json headers, std::string body)
		{
			status& clientStatus = getComponent<HTTPResponseComponent>().httpstatus;
			while (streamPeerTCP::getStatus() != STATUS_CONNECTED)
			{
				if (streamPeerTCP::getStatus() == STATUS_ERROR) return LN_CORE_ERROR("[HTTP]error host was not connected!");
				streamPeerTCP::poll();
			}
			std::string requestString = generateRequest(requestMethod, destination, headers, body);
			socketError error = streamPeerTCP::putData((const uint8_t*)requestString.data(), requestString.size());
			clientStatus = status::STATUS_REQUESTING;
			LN_ERR_FAIL_COND_MSG(error != SUCCESS, "could not send reqeust, reason: " + std::to_string(error));
			clientStatus = status::STATUS_ERROR;
		}

		bool HTTPClient::hasResponse()
		{
			return getStatus() == STATUS_RECEIVING;
		}
		socketError HTTPClient::poll()
		{
			if (streamPeerTCP::getStatus() == STATUS_CONNECTED)
			{
				HTTPResponseComponent& responseComponent = getComponent<HTTPResponseComponent>();
				std::array<std::uint8_t, 4096> tempBuffer{ 0 };
				int received;
				//NOTE http responses are delivered in chunks so read them in chunks!
				socketError error = streamPeerTCP::getPartialData(tempBuffer.data(), tempBuffer.size(), received);
				if (error == FAILED) return socketError::FAILED;
				responseComponent.httpstatus = STATUS_RECEIVING;
				responseComponent.data.insert(responseComponent.data.end(), tempBuffer.begin(), tempBuffer.begin() + received);
				if(responseComponent.readingHeaders) 
				{
					std::string headers;
					std::string delimeter = "\r\n\r\n";

					std::string responseData = std::string(responseComponent.data.begin(), responseComponent.data.end());
					
					size_t pos = responseData.find(delimeter);

					if (pos != std::string::npos) 
					{
						headers = responseData.substr(0, pos);
						responseComponent.bodyStartIndex = pos + 4;

						parseHeaders(headers);

						responseComponent.readingHeaders = false;
						return SUCCESS;
					}
				}

				if (!responseComponent.readingHeaders)
				{
					
					bool contentLoaded = responseComponent.data.size() == responseComponent.bodyStartIndex + atoi(responseComponent.headers.jsonData["content-length"].get<std::string>().c_str());
					if (received == 0  && contentLoaded) { // disconnected
						responseComponent.httpstatus = STATUS_DONE;
						return SUCCESS;
					}
				}
				return SUCCESS;
				
				
			}
			else if (streamPeerTCP::getStatus() == STATUS_ERROR) {
				LN_CORE_ERROR("[HTTP] an error occured while polling the http client!");
				return socketError::FAILED;
			}
		}
		HTTPClient::status HTTPClient::getStatus()
		{
			return getComponent<HTTPResponseComponent>().httpstatus;

		}
		HTTPClient::responseCode HTTPClient::getResponseCode()
		{
			return OK;
		}
		std::vector<uint8_t> HTTPClient::getBody()
		{
			LN_ERR_FAIL_COND_V_MSG(getStatus() != status::STATUS_DONE,std::vector<uint8_t>(),"body has not been fully received");
			HTTPResponseComponent& component = getComponent<HTTPResponseComponent>();
			return std::vector<uint8_t>(component.data.begin() + component.bodyStartIndex,component.data.end());
		}
		utils::json HTTPClient::getHeaders()
		{
			HTTPResponseComponent& component = getComponent<HTTPResponseComponent>();
			LN_ERR_FAIL_COND_V_MSG(component.readingHeaders, utils::json(), "headers have not been parsed yet");
			return component.headers;
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
		void HTTPClient::parseHeaders(const std::string& headersString)
		{
			std::string str = headersString;
			std::vector<std::string> headers;
			size_t start = 0;
			size_t found = str.find("\r\n");

			while (found != std::string::npos) {
				std::string header = str.substr(start, found - start);
				transform(header.begin(), header.end(), header.begin(), ::tolower);
				headers.push_back(header);
				start = found + 2; // Move past the "\r\n"
				found = str.find("\r\n", start);
			}

			if (start < str.length()) {
				headers.push_back(str.substr(start)); // Add the remainder of the string
			}
			nlohmann::json& jsonHeaders = getComponent<HTTPResponseComponent>().headers.jsonData;
			parseResponse(headers[0]);
			headers.erase(headers.begin());
			for (std::string header : headers)
			{
				std::vector<std::string> result;
				const static std::string delimiter = ":";
				size_t start = 0;
				size_t found = header.find(delimiter);

				while (found != std::string::npos)
				{
					result.push_back(header.substr(start, found - start));
					start = found + delimiter.length();
					found = header.find(delimiter, start);
				}

				if (start < header.length()) result.push_back(header.substr(start + 1));
				jsonHeaders[result[0]] = result[1];
			}
		}
		void HTTPClient::parseResponse(std::string responseString)
		{
			HTTPResponseComponent& component = getComponent<HTTPResponseComponent>();
			std::vector<std::string> result;
			const static std::string delimiter = " ";
			size_t start = 0;
			size_t found = responseString.find(delimiter);

			while (found != std::string::npos)
			{
				result.push_back(responseString.substr(start, found - start));
				start = found + delimiter.length();
				found = responseString.find(delimiter, start);
			}

			if (start < responseString.length()) result.push_back(responseString.substr(start + 1));

			component.responseCode = atoi(result[1].c_str());
			//component.message = result[3];
		}
	}
#pragma endregion
}

