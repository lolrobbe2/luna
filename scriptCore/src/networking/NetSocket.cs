﻿
using System;
using System.Runtime.CompilerServices;

namespace Luna
{
    /// <summary>
    /// Enumaration of socketErrors
    /// </summary>
    public enum SocketError
    {
        SUCCESS,
        FAILED,
        INIT_FAILED,
        BIND_FAILED,
        CONNECT_FAILED,
        INVALID_IP_ADDRESS,
        ALREADY_INIT,
        CONNECTION_CLOSED,
        OUT_OF_BUFFER_MEMORY,
        BUSY,
        SOCKET_INVALID,
        FILE_EOF
    };

    /// <summary>
    /// Enumeration of socket protocols
    /// </summary>
    public enum Protocol
    {
        ///<summary>Transmission Control Protocol</summary> 
        TCP, ///<summary>Universal Datagram Protocol</summary>
        UDP
    };
    /// <summary>
    /// This is bare bones Network socket that is multiplatform.
    /// USE with CARE try to use the higher level networking options first. exmaple HTTPclient
    /// </summary>
    public class NetSocket : LunaObject
    {
        /// <summary>
        /// NetSocket constructor.
        /// creates a uninitialized socket
        /// </summary>
        public NetSocket() : base(NetSocketCreate()) { }
        /// <summary>
        /// NetSocket constructor.
        /// creates a potentiolay initialized socket
        /// </summary>
        public NetSocket(ulong ObjectId) : base(ObjectId) { } 
        /// <summary>
        /// closes the socket.
        /// </summary>
        ~NetSocket() { NetSocketDestroy(ObjectId); }

        public void Destroy() { NetSocketDestroy(ObjectId); }
        public SocketError Open(Protocol Proto) { return NetSocketOpen(ObjectId, Proto); }
        /// <summary>
        /// <para>Opens the socket, and binds it to the specified local address.</para>
        /// <para>This method is generally not needed, and only used to force the subsequent call to <see cref="ConnectToHost"/> to use the specified <see cref="Host"/> and <see cref="Port"/> as source address.This can be desired in some NAT punchthrough techniques, or when forcing the source network interface.
        /// </para>
        /// </summary>
        /// <param name="port">the port to bind the socket to</param>
        /// <param name="host">the hsot name. for example google.com</param>
        /// <param name="protocol"> <see cref="Protocol"/></param>
        /// <returns></returns>
        public SocketError Bind(int port,string host) { return NetSocketBind(ObjectId, port, host); }
        /// <summary>
        /// Connects to the specified host:port pair. A hostname will be resolved if valid. Returns <see cref="SocketError.SUCCESS"/>.OK on success.
        /// </summary>
        /// <param name="Port"></param>
        /// <param name="Host"></param>
        /// <param name="Protocol"></param>
        /// <returns></returns>
        public SocketError ConnetToHost(int Port, string Host) { return NetSocketConnectToHost(ObjectId, Port, Host); }
              
        public SocketError Recieve(byte[] buffer,int length) { buffer = new byte[length]; return NetSocketReceive(ObjectId,buffer, length); }
        public SocketError RecieveFrom(byte[] buffer,IpAddress address,int length, UInt16 port, bool peek) { if (buffer.Length < length) { Log.Error("buffer size was to small: size was {} bytes.", buffer.Length); return SocketError.OUT_OF_BUFFER_MEMORY; } return NetSocketReceiveFrom(ObjectId,buffer, address.getIpRaw(), port, peek); }
        public SocketError Send(byte[] buffer, int length) { return NetSocketSend(ObjectId, buffer, length); }
        public SocketError SendTo(byte[] buffer,int len,IpAddress address,UInt16 port) { return NetSocketSendTo(ObjectId, buffer, len, address.getIpRaw(), port); }
        public NetSocket Accept(IpAddress address,ref int port) { return new NetSocket(NetSocketAccept(ObjectId, address.getIpRaw(), out port)); }
        public SocketError Listen(int maxPending = 200) { return NetSocketListen(ObjectId, maxPending); }
        /// <summary>
        /// function to create a NetSocket module
        /// </summary>
        /// <returns>entityHandle</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern ulong NetSocketCreate();
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketOpen(ulong ObjectId,Protocol proto);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketBind(ulong ObjectId,int Port,string Host);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketConnectToHost(ulong ObjectId, int Port, string Host);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketReceive(ulong ObjectId,byte[] buffer,int lenght);

        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketReceiveFrom(ulong ObjectId,byte[] buffer, byte[] address, UInt16 port, bool peek);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketSend(ulong ObjectId, byte[] buffer, int len);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketSendTo(ulong ObjectId, byte[] buffer, int len, byte[] ipAddress, int port);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern ulong NetSocketAccept(ulong ObjectId, byte[] address,out int port);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketListen(ulong ObjectId,int maxPendding);

        /// <summary>
        /// externall call to destroy the socket
        /// </summary>
        /// <param name="ObjectId"></param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void NetSocketDestroy(ulong ObjectId);
    }
}