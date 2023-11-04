

using System.Runtime.CompilerServices;

namespace Luna
{
    public enum Status
    {
        STATUS_NONE,
        STATUS_CONNECTING,
        STATUS_CONNECTED,
        STATUS_ERROR,
    };  
    /// <summary>
    /// TCP client stream.
    /// <para>this module allows you to connect to TCPservers or is returned by a TCPserver as a connection</para>
    /// </summary>
    public class StreamPeerTCP : LunaObject
    {
        /// <summary>
        /// creates a StreamPeerTCP module
        /// </summary>
        public StreamPeerTCP() : base(StreamPeerTCPCreate()) { }
        /// <summary>
        /// Opens the TCP socket, and binds it to the specified local address.
        /// This method is generally not needed, and only used to force the subsequent call to connect_to_host to use the specified host and port as source address.This can be desired in some NAT punchthrough techniques, or when forcing the source network interface.
        /// 
        /// </summary>
        /// <param name="Port">valid port between 0-65536 inclusive</param>
        /// <param name="Host">Valid ipv4/6 address or hostname (example.com) default = "*" </param>
        /// <returns><see cref="SocketError.SUCCESS"/> on success.</returns>
        public SocketError Bind(int Port, IpAddress Host) { return StreamPeerTCPBind(ObjectId, Port, Host.getIpRaw()); }
        /// <summary>
        /// Opens the TCP socket, and binds it to the specified local address.
        /// This method is generally not needed, and only used to force the subsequent call to connect_to_host to use the specified host and port as source address.This can be desired in some NAT punchthrough techniques, or when forcing the source network interface.
        /// 
        /// </summary>
        /// <param name="Port">valid port between 0-65536 inclusive</param>
        /// <returns><see cref="SocketError.SUCCESS"/> on success.</returns>
        public SocketError Bind(int Port) { return StreamPeerTCPBind(ObjectId, Port, new IpAddress("*").getIpRaw()); }
        /// <summary>
        /// Connects to the specified host:port pair. A hostname will be resolved if valid.
        /// </summary>
        /// <param name="Host">Valid ipv4/6 address or hostname (example.com)</param>
        /// <param name="Port">valid port between 0-65536 inclusive</param>
        /// <returns><see cref="SocketError.SUCCESS"/> on success.</returns>
        public SocketError ConnectToHost(IpAddress Host,int Port) { return StreamPeerTCPConnectToHost(ObjectId, Host.getIpRaw(), Port); }
        /// <summary>
        /// Poll the socket, updating its state. <see cref="GetStatus"/>.
        /// </summary>
        /// <returns>SocketError</returns>
        public SocketError Poll() { return StreamPeerTCPPoll(ObjectId); }
        /// <summary>
        /// Returns the status of the connection, see <see cref="Status"/>.
        /// </summary>
        /// <returns><see cref="Status"/></returns>
        public Status GetStatus() { return StreamPeerTCPGetStatus(ObjectId); }
        /// <summary>
        /// Returns the IP of this peer.
        /// </summary>
        /// <returns>IpAddress of peer</returns>
        public IpAddress GetConnectedHost() { IpAddress address = new IpAddress();address.setIpv6(StreamPeerTCPGetConnectedHost(ObjectId)); return address; }
        /// <summary>
        /// Returns the port of this peer.
        /// </summary>
        /// <returns>Returns the port of this peer.</returns>
        public int GetConnectedPort() { return StreamPeerTCPGetConnectedPort(ObjectId); }
        /// <summary>
        /// Returns the local port to which this peer is bound.
        /// </summary>
        /// <returns></returns>
        public int GetLoaclPort() { return StreamPeerTCPGetLocalPort(ObjectId); }
        /// <summary>
        /// Disconnects from host.
        /// </summary>
        public void DisconnectFromHost() {  StreamPeerTCPDisconnectFromHost(ObjectId); }
        /// <summary>
        /// Sends a chunk of data through the connection, blocking if necessary until the data is done sending. This function returns an <see cref="SocketError"/> code.
        /// </summary>
        /// <param name="Data">byte array</param>
        /// <returns><see cref="SocketError.SUCCESS"/> when data has been send succesfully.</returns>
        public SocketError PutData(byte[] Data) { return StreamPeerTCPPutData(ObjectId, Data); }
        /// <summary>
        /// Sends a partial chunk of data through the connection. This function returns an <see cref="SocketError"/> code.
        /// </summary>
        /// <param name="Data"></param>
        /// <param name="BytesSent"></param>
        /// <returns></returns>
        public SocketError PutPartialData(byte[] Data,out int BytesSent) { return StreamPeerTCPPutPartialData(ObjectId, Data,out BytesSent); }
        /// <summary>
        /// Gets data and fills the Buffer Array
        /// </summary>
        /// <param name="Buffer">byte array to wich the data will be placed into</param>
        /// <returns></returns>
        public SocketError GetData(byte[] Buffer) { return StreamPeerTCPGetData(ObjectId,Buffer); }
        /// <summary>
        /// Gets partial data and fills the Buffer Array.
        /// </summary>
        /// <param name="Buffer"></param>
        /// <param name="Received"></param>
        /// <returns></returns>
        public SocketError GetPartialData(byte[] Buffer,out int Received) { return StreamPeerTCPGetPartialData(ObjectId,Buffer,out Received); }
        /// <summary>
        /// Returns the number of bytes this StreamPeer has available.
        /// </summary>
        /// <returns></returns>
        public int GetAvailableBytes() { return StreamPeerTCPGetAvailableBytes(ObjectId); }       
        /// <summary>
        /// function to create a StreamPeerTCP module
        /// </summary>
        /// <returns>entityHandle</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern ulong StreamPeerTCPCreate();
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static SocketError StreamPeerTCPBind(ulong ObjectId, int port, byte[] AddressArray);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static SocketError StreamPeerTCPConnectToHost(ulong ObjectId, byte[] AddressArray, int port);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static SocketError StreamPeerTCPPoll(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static Status StreamPeerTCPGetStatus(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static byte[] StreamPeerTCPGetConnectedHost(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static int StreamPeerTCPGetConnectedPort(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static int StreamPeerTCPGetLocalPort(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static void StreamPeerTCPDisconnectFromHost(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static void StreamPeerTCPSetNoDelay(ulong ObjectId, bool setDelay);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static SocketError StreamPeerTCPPutData(ulong ObjectId, byte[] Buffer);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static SocketError StreamPeerTCPPutPartialData(ulong ObjectId, byte[] Buffer,out int p_sent);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static SocketError StreamPeerTCPGetData(ulong ObjectId, byte[] Buffer);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static SocketError StreamPeerTCPGetPartialData(ulong ObjectId, byte[] Buffer,out int p_received);
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static int StreamPeerTCPGetAvailableBytes(ulong ObjectId);

    }
}
