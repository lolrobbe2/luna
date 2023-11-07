using System.Runtime.CompilerServices;

namespace Luna
{
    public class HTTPClient : LunaObject
    {
        public enum Method
        {
            GET,
            POST,
            PUT,
            HEAD,
            DELETE,
            PATCH, 
            OPTIONS,
            CONNECT,
            TRACE,

        };
        /// <summary>
        /// current status of the HTTPClient
        /// </summary>
        public enum Status
        {
            STATUS_NONE,
            STATUS_CONNECTING,
            STATUS_CONNECTED,
            STATUS_ERROR,
            STATUS_REQUESTING,
            STATUS_RECEIVING
        };

        public HTTPClient() : base(HTTPClientCreate()) { }
        /// <summary>
        /// connects the http client to a host http server.
        /// </summary>
        /// <remarks>will also connect to non http servers, it just won't be able to now wath to do with the HTTP response</remarks>
        /// <param name="Host">can be an <see cref="IpAddress"/></param>
        /// <param name="port"> number inclusive between 0-65536</param>
        /// <returns><see cref="SocketError.SUCCESS"/> when connetion to host whas succesful</returns>
        public SocketError ConnectToHost(string Host,int port = 80) { return HTTPClientConnectToHost(ObjectId, Host, port); }
        /// <summary>
        /// sends an HTPP request to the connected host.
        /// </summary>
        /// <param name="Method">http method to be used <see cref="HTTPClient.Method"/></param>
        /// <param name="Destination"></param>
        /// <param name="Headers"></param>
        /// <param name="body"></param>
        public void Request(HTTPClient.Method Method, string Destination, Json Headers, string body = "") { HTTPClientRequest(ObjectId, Method, Destination, Headers.ToString(), body); }
        /// <summary>
        /// checks the HTTPClient if a response is available.
        /// </summary>
        /// <returns></returns>
        public bool HasResponse() { return HTTPClientHasResponse(ObjectId); }
        /// <summary>
        /// Polls the HTPPClient for incomming response
        /// </summary>
        /// <remarks>
        /// as soon a the http client is receiving a response <see cref="HasResponse"/> will return true.
        /// but at this point the body might nog have been fully received. To know when the response body has not been fully received <see cref="Status.STATUS_RECEIVING"/> use the <see cref="GetStatus"/> function.
        /// </remarks>
        /// <returns><see cref="SocketError.SUCCESS"/> when polling whas succesful</returns>
        public SocketError Poll() { return HTTPClientPoll(ObjectId); }
        /// <summary>
        /// function to get the current HTTPClient <see cref="Status"/>
        /// </summary>
        /// <returns><see cref="Status"/></returns>
        public Status GetStatus() { return HTTPClientGetStatus(ObjectId); }
        #region glue

        /// <summary>
        /// function to create a StreamPeerTCP module
        /// </summary>
        /// <returns>entityHandle</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern ulong HTTPClientCreate();
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError HTTPClientConnectToHost(ulong ObjectId, string HostName, int Port);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void HTTPClientRequest(ulong ObjectId, HTTPClient.Method Method, string Destination, string HeadersJsonString, string body);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern bool HTTPClientHasResponse(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern SocketError HTTPClientPoll(ulong ObjectId);
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern Status HTTPClientGetStatus(ulong ObjectId);
        #endregion
    }
}
