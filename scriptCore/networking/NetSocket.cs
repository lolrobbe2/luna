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
        BUSY
    };

    /// <summary>
    /// Enumeration of socket protocols
    /// </summary>
    public enum Protocol
    {
        TCP, // Transmission Control Protocol
        UDP // Unicersal Datagram Protocol
    };
    /// <summary>
    /// This is bare bones Network socket that is multiplatform.
    /// </summary>
    public class NetSocket : LunaObject
    {
        /// <summary>
        /// NetSocket constructor.
        /// creates a uninitialized socket
        /// </summary>
        public NetSocket() : base(NetSocketCreate()) { }

        /// <summary>
        /// closes the socket.
        /// </summary>
        ~NetSocket() { NetSocketDestroy(ObjectId); }

        /// <summary>
        /// <para>Opens the TCP socket, and binds it to the specified local address.</para>
        /// <para>This method is generally not needed, and only used to force the subsequent call to <see cref="ConnectToHost"/> to use the specified <see cref="Host"/> and <see cref="Port"/> as source address.This can be desired in some NAT punchthrough techniques, or when forcing the source network interface.
        /// </para>
        /// </summary>
        /// <param name="port">the port to bind the socket to</param>
        /// <param name="host">the hsot name. for example google.com</param>
        /// <param name="protocol"> <see cref="Protocol"/></param>
        /// <returns></returns>
        public SocketError Bind(int port,string host,Protocol protocol) { return NetSocketBind(ObjectId, port, host, protocol); }
        /// <summary>
        /// Connects to the specified host:port pair. A hostname will be resolved if valid. Returns <see cref="SocketError.SUCCESS"/>.OK on success.
        /// </summary>
        /// <param name="Port"></param>
        /// <param name="Host"></param>
        /// <param name="Protocol"></param>
        /// <returns></returns>
        public SocketError ConnetToHost(int Port, string Host, Protocol Protocol) { return NetSocketConnectToHost(ObjectId, Port, Host, Protocol); }
              
        public SocketError Recieve(byte[] buffer,int length) { buffer = new byte[length]; return NetSocketReceive(ObjectId,buffer, length); }
        public SocketError RecieveFrom(byte[] buffer,IpAddress address,int length, UInt16 port, bool peek) { if (buffer.Length < length) { Log.Error("buffer size was to small: size was {} bytes.", buffer.Length); return SocketError.OUT_OF_BUFFER_MEMORY; } return NetSocketReceiveFrom(ObjectId,buffer, address.getIpRaw(), port, peek); }
        public SocketError Send(byte[] buffer, int length) { return NetSocketSend(ObjectId, buffer, length); }
        public SocketError SendTo(byte[] buffer,int len,IpAddress address,UInt16 port) { return NetSocketSendTo(ObjectId, buffer, len, address.getIpRaw(), port); }

        /// <summary>
        /// function to create a NetSocket module
        /// </summary>
        /// <returns>entityHandle</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern ulong NetSocketCreate();

        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketBind(ulong ObjectId,int Port,string Host,Protocol proto);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketConnectToHost(ulong ObjectId, int Port, string Host, Protocol proto);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketReceive(ulong ObjectId,byte[] buffer,int lenght);

        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketReceiveFrom(ulong ObjectId,byte[] buffer, byte[] address, UInt16 port, bool peek);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketSend(ulong ObjectId, byte[] buffer, int len);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError NetSocketSendTo(ulong ObjectId, byte[] buffer, int len, byte[] ipAddress, int port);
        /// <summary>
        /// externall call to destroy the socket
        /// </summary>
        /// <param name="ObjectId"></param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void NetSocketDestroy(ulong ObjectId);
    }
}
