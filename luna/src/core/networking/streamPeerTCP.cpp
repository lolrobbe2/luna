#include "streamPeerTCP.h"
#include <core/platform/platformUtils.h>
#include <core/scripting/scriptingEngine.h>
namespace luna 
{
	namespace networking 
	{
#pragma region glue
		static socketError StreamPeerTCPBind(entt::entity objectId,int port,MonoArray* addressArray)
		{
			ipAddress* address = (ipAddress*)mono_array_addr_with_size(addressArray, mono_array_length(addressArray), 0);
			return OBJ_GET(streamPeerTCP).bind(port,*address);
		}
		static socketError StreamPeerTCPConnectToHost(entt::entity objectId, MonoArray* addressArray,int port)
		{
			ipAddress* address = (ipAddress*)mono_array_addr_with_size(addressArray, mono_array_length(addressArray), 0);
			return OBJ_GET(streamPeerTCP).connectToHost(*address, port);
		}

		static socketError StreamPeerTCPPoll(entt::entity objectId)
		{
			return OBJ_GET(streamPeerTCP).poll();
		}

		static status StreamPeerTCPGetStatus(entt::entity objectId)
		{
			return OBJ_GET(streamPeerTCP).getStatus();
		}
		static MonoArray* StreamPeerTCPGetConnectedHost(entt::entity objectId)
		{
			MonoArray* addressArray = scripting::scriptingEngine::createArray<uint8_t>(16);
			ipAddress* addr = (ipAddress*)mono_array_addr_with_size(addressArray, mono_array_length(addressArray), 0);
			*addr = OBJ_GET(streamPeerTCP).getConnectedhost();
			return addressArray;
		}
		static int StreamPeerTCPGetConnectedPort(entt::entity objectId)
		{
			return OBJ_GET(streamPeerTCP).getConnectedPort();
		}
		static int StreamPeerTCPGetLocalPort(entt::entity objectId)
		{
			return OBJ_GET(streamPeerTCP).getLocalPort();	
		}
		static void StreamPeerTCPDisconnectFromHost(entt::entity objectId)
		{
			OBJ_GET(streamPeerTCP).disconnectFromHost();
		}

		static void StreamPeerTCPSetNoDelay(entt::entity objectId,bool setDelay)
		{
			OBJ_GET(streamPeerTCP).setNoDelay(setDelay);
		}
		static socketError StreamPeerTCPPutData(entt::entity objectId,MonoArray* buffer)
		{
			uint8_t* p_buffer = (uint8_t*)mono_array_addr_with_size(buffer, mono_array_length(buffer), 0);
			return OBJ_GET(streamPeerTCP).putData(p_buffer, mono_array_length(buffer));
		}

		static socketError StreamPeerTCPPutPartialData(entt::entity objectId,MonoArray* buffer,int* p_sent)
		{
			uint8_t* p_buffer = (uint8_t*)mono_array_addr_with_size(buffer, mono_array_length(buffer), 0);
			return OBJ_GET(streamPeerTCP).putPartialData(p_buffer, mono_array_length(buffer),*p_sent);
		}
		static socketError StreamPeerTCPGetData(entt::entity objectId, MonoArray* buffer)
		{
			uint8_t* p_buffer = (uint8_t*)mono_array_addr_with_size(buffer, mono_array_length(buffer), 0);
			return OBJ_GET(streamPeerTCP).getData(p_buffer, mono_array_length(buffer));
		}
#pragma endregion
		socketError streamPeerTCP::poll()
		{
			tcpComponent& tcpData = getComponent<tcpComponent>();

			if (tcpData.status == STATUS_CONNECTED) {
				socketError err;
				err = netSocket::poll(POLL_TYPE_IN, 0);
				if (err == SUCCESS) {
					// FIN received
					if (netSocket::getAvailableBytes() == 0)
					{
						disconnectFromHost();
						return SUCCESS;
					}
				}
				// Also poll write
				err = netSocket::poll(POLL_TYPE_IN_OUT, 0);
				if (err != SUCCESS && err != BUSY) {
					// Got an error
					disconnectFromHost();
					tcpData.status = STATUS_ERROR;
					return err;
				}
			}
			else if (tcpData.status != STATUS_CONNECTING) return SUCCESS;

			socketError err = netSocket::connectToHost(tcpData.peerPort, tcpData.peerHost,TCP);

			if (err == SUCCESS) {
				tcpData.status = STATUS_CONNECTED;
				return SUCCESS;
			}
			else if (err == BUSY) {
				// Check for connect timeout
				if (platform::os::getTicksMsec() > tcpData.timeout) {
					disconnectFromHost();
					tcpData.status = STATUS_ERROR;
					return FAILED;
				}
				// Still trying to connect
				return SUCCESS;
			}

			disconnectFromHost();
			tcpData.status = STATUS_ERROR;
			return socketError::CONNECT_FAILED;
		}

		socketError streamPeerTCP::write(const uint8_t* data, int bytes, int& sent, bool block) {
			LN_ERR_FAIL_COND_V(!netSocket::isValid(), socketError::FAILED);

			tcpComponent& tcpData = getComponent<tcpComponent>();


			if (tcpData.status != STATUS_CONNECTED) {
				return FAILED;
			}

			socketError err;
			int dataToSend = bytes;
			const uint8_t* offset = data;
			int totalSent = 0;

			while (dataToSend) {
				int sentAmount = 0;
				err = netSocket::send(offset, dataToSend, sentAmount);

				if (err != SUCCESS) {
					if (err != BUSY) {
						disconnectFromHost();
						return FAILED;
					}

					if (!block) {
						sent = totalSent;
						return SUCCESS;
					}

					// Block and wait for the socket to accept more data
					err = netSocket::poll(POLL_TYPE_OUT, -1);
					if (err != SUCCESS) {
						disconnectFromHost();
						return FAILED;
					}
				}
				else {
					dataToSend -= sentAmount;
					offset += sentAmount;
					totalSent += sentAmount;
				}
			}

			sent = totalSent;

			return SUCCESS;
		}

		socketError streamPeerTCP::read(uint8_t* buffer, int bytes, int& received, bool block) {
			tcpComponent& tcpData = getComponent<tcpComponent>();

			if (tcpData.status != STATUS_CONNECTED) {
				return FAILED;
			}

			socketError err;
			int toRead = bytes;
			int totalRead = 0;
			received = 0;

			while (toRead) {
				int read = 0;
				err = netSocket::receive(buffer + totalRead, toRead, read);

				if (err != SUCCESS) {
					if (err != BUSY) {
						disconnectFromHost();
						return FAILED;
					}

					if (!block) {
						received = totalRead;
						return SUCCESS;
					}

					err = netSocket::poll(POLL_TYPE_IN, -1);

					if (err != SUCCESS) {
						disconnectFromHost();
						return FAILED;
					}

				}
				else if (read == 0) {
					disconnectFromHost();
					received = totalRead;
					return FILE_EOF;

				}
				else {
					toRead -= read;
					totalRead += read;

					if (!block) {
						received = totalRead;
						return SUCCESS;
					}
				}
			}

			received = totalRead;

			return SUCCESS;
		}

		void streamPeerTCP::setNoDelay(bool enabled) {
			LN_ERR_FAIL_COND(!netSocket::isValid() || !netSocket::isOpen());
			netSocket::setTcpNoDelayEnabled(enabled);
		}

		status streamPeerTCP::getStatus() {
			return getComponent<tcpComponent>().status;
		}


		socketError streamPeerTCP::wait(pollType type, int timeout)
		{
			LN_ERR_FAIL_COND_V(netSocket::isValid()|| !netSocket::isOpen(), socketError::FAILED);
			return netSocket::poll(type, timeout);
		}

		socketError streamPeerTCP::putData(const uint8_t* data, int bytes)
		{
			int total;
			return write(data, bytes, total, true);
		}
		socketError streamPeerTCP::putPartialData(const uint8_t* data, int bytes, int& sent)
		{
			return write(data, bytes, sent, false);
		}
		socketError streamPeerTCP::getData(uint8_t* buffer, int bytes)
		{
			int total;
			return read(buffer,bytes,total,true);
		}
		socketError streamPeerTCP::getPartialData(uint8_t* buffer, int bytes, int& received)
		{
			return read(buffer, bytes, received, false);
		}
		void streamPeerTCP::setSocket(ref<netSocket> socket)
		{
			socketComponent& socketData = getComponent<socketComponent>();
			LN_ERR_FAIL_COND_MSG(socket->isValid(), "socket was invalid, pls enter a valid socket handle");
			socketData = socket->getComponent<socketComponent>();
		}

		socketError streamPeerTCP::bind(int port, const ipAddress& host) {
			LN_ERR_FAIL_COND_V(!netSocket::isValid(), socketError::FAILED);
			LN_ERR_FAIL_COND_V(netSocket::isOpen(), socketError::ALREADY_INIT);
			LN_ERR_FAIL_COND_V_MSG(port < 0 || port > 65535, socketError::FAILED, "The local port number must be between 0 and 65535 (inclusive).");

			netSocket::setBlockingEnabled(false);
			return netSocket::bind(port,host,TCP);
		}

		socketError streamPeerTCP::connectToHost(const ipAddress& host, int port)
		{
			tcpComponent& tcpData = getComponent<tcpComponent>();
			LN_ERR_FAIL_COND_V(!netSocket::isValid(), socketError::INIT_FAILED);
			LN_ERR_FAIL_COND_V(tcpData.status != STATUS_NONE, socketError::ALREADY_INIT);
			LN_ERR_FAIL_COND_V(!host.isValid(), socketError::INVALID_IP_ADDRESS);
			LN_ERR_FAIL_COND_V_MSG(port < 1 || port > 65535, socketError::FAILED, "The remote port number must be between 1 and 65535 (inclusive).");

			if (!netSocket::isOpen()) {
				Ip::Type ip_type = host.isIpv4() ? Ip::TYPE_IPV4 : Ip::TYPE_IPV6;
				socketError err = netSocket::open(TCP);
				if (err != SUCCESS) {
					return err;
				}
				netSocket::setBlockingEnabled(false);
			}

			tcpData.timeout = platform::os::getTicksMsec() + 5000; //5000 msec timeout delay = 5sec
			socketError err = connectToHost(host, port);

			if (err == SUCCESS) {
				tcpData.status = STATUS_CONNECTED;
			}
			else if (err == BUSY) {
				tcpData.status = STATUS_CONNECTING;
			}
			else {
				LN_CORE_ERROR("Connection to remote host failed!");
				disconnectFromHost();
				return FAILED;
			}

			tcpData.peerHost = host;
			tcpData.peerPort = port;

			return SUCCESS;
		}

		int streamPeerTCP::getLocalPort()
		{
			return getComponent<socketComponent>().port;
		}

		ipAddress streamPeerTCP::getConnectedhost()
		{
			return getComponent<tcpComponent>().peerHost;
		}

		void streamPeerTCP::disconnectFromHost()
		{
			tcpComponent& tcpData = getComponent<tcpComponent>();
			if (netSocket::isValid() && netSocket::isOpen()) netSocket::destroy();
			
			tcpData.timeout = 0;
			tcpData.status = STATUS_NONE;
			tcpData.peerHost = ipAddress();
			tcpData.peerPort = 0;
		}

		int streamPeerTCP::getConnectedPort()
		{
			return getComponent<tcpComponent>().peerPort;
		}
	}
}