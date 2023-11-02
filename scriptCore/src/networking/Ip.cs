using System.Runtime.CompilerServices;

namespace Luna
{
    using ResolverId = int;
    public class Ip
    {
       /// <summary>
       /// Status enum of the hostanem resolver.
       /// </summary>
        public enum ResolverStatus
        {
            ///<summary>DNS hostname resolver status: No status.</summary>
            RESOLVER_STATUS_NONE,
            /// <summary>DNS hostname resolver status: Waiting.</summary>
            RESOLVER_STATUS_WAITING,
            /// <summary>DNS hostname resolver status: Done.</summary>
            RESOLVER_STATUS_DONE,
            /// <summary>DNS hostname resolver status: Error.</summary>
            RESOLVER_STATUS_ERROR,
        };

        public enum Type
        {
            TYPE_NONE = 0,
            TYPE_IPV4 = 1,
            TYPE_IPV6 = 2,
            TYPE_ANY = 3,
        };

        /// <summary>
        /// Returns a given hostname's IPv4 or IPv6 address as a string when resolved (blocking-type method). The address type returned depends on the Type constant given as IpType.
        /// </summary>
        /// <param name="Hostname">hostname to resolve</param>
        /// <param name="IpType"> type enum for Ip type</param>
        /// <returns>an ipAddress in string notation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string ResolveHostname(string Hostname,Type IpType = Type.TYPE_ANY);
        /// <summary>
        /// Returns a given hostname's IPv4 or IPv6 address as a string when resolved (blocking-type method). The address type returned depends on the Type constant given as IpType.
        /// <para>
        /// same as <see cref="ResolveHostname(string, Type)"/>, but string has been converted to IpAddress object.
        /// </para>
        /// </summary>
        /// <param name="Hostname">hostname to resolve</param>
        /// <param name="IpType"> type enum for Ip type</param>
        /// <returns>an ipAddress</returns>
        public static IpAddress ResolveHostnameAddress(string Hostname, Type IpType = Type.TYPE_ANY) { return new IpAddress(ResolveHostname(Hostname, IpType)); }

        /// <summary>
        /// Returns a given array of hostname's IPv4 or IPv6 addresses as a string when resolved (blocking-type method). The address type returned depends on the Type constant given as IpType.
        /// <para>
        /// same as <see cref="ResolveHostname(string, Type)"/>, but string has been converted to IpAddress object.
        /// </para>
        /// </summary>
        /// <param name="Hostname">hostname to resolve</param>
        /// <param name="IpType"> type enum for Ip type</param>
        /// <returns>an ipAddress</returns
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string[] ResolveHostnameAddresses(string Hostname,Type IpType = Type.TYPE_ANY);
    }
}
