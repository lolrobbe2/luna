#include "streamPeerTCP.h"
#include <core/platform/platformUtils.h>
#include <zlib.h>
namespace luna 
{
	namespace networking 
	{
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
		void streamPeerTCP::setSocket(ref<netSocket> socket)
		{
			socketComponent& socketData = getComponent<socketComponent>();
			LN_ERR_FAIL_COND_MSG(socket->isValid(), "socket was invalid, pls enter a valid socket handle");

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

			//timeout = OS::get_singleton()->get_ticks_msec() + (((uint64_t)GLOBAL_GET("network/limits/tcp/connect_timeout_seconds")) * 1000);
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

		void streamPeerTCP::disconnectFromHost()
		{
			tcpComponent& tcpData = getComponent<tcpComponent>();
			if (netSocket::isValid() && netSocket::isOpen()) netSocket::destroy();
			
			tcpData.timeout = 0;
			tcpData.status = STATUS_NONE;
			tcpData.peerHost = ipAddress();
			tcpData.peerPort = 0;
		}
	}
}