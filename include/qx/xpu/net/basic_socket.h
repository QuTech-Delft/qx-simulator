#pragma once

#include <cstdlib>   // for atoi
#include <cstring>   // for memset
#include <exception> // for exception class
#include <string>    // for std::string

#ifdef WIN32
#include <winsock.h> // for socket(), connect(), send(), and recv()
typedef int socklen_t;
typedef char raw_type; // Type used for raw data on this platform
#else
#include <arpa/inet.h>  // for inet_addr()
#include <netdb.h>      // for gethostbyname()
#include <netinet/in.h> // for sockaddr_in
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <sys/types.h>  // for data types
#include <unistd.h>     // for close()
typedef void raw_type; // Type used for raw data on this platform
#endif

#ifdef WIN32
static bool initialized = false;
#endif

#include "qx/xpu/net/socket_exception.h"

namespace xpu {

/**
 *   base class representing basic communication endpoint
 */
class basic_socket {

public:
    /**
     *   close and deallocate this socket
     */
    ~basic_socket();

    /**
     *   get the local address
     *   @return local address of socket
     *   @exception SocketException thrown if fetch fails
     */
    std::string get_local_address();

    /**
     *   get the local port
     *   @return local port of socket
     *   @exception SocketException thrown if fetch fails
     */
    unsigned short get_local_port();

    /**
     *   set the local port to the specified port and the local address
     *   to any interface
     *   @param localPort local port
     *   @exception SocketException thrown if setting local port fails
     */
    void set_local_port(unsigned short local_port);

    /**
     *   set the local port to the specified port and the local address
     *   to the specified address.  If you omit the port, a random port
     *   will be selected.
     *   @param localAddress local address
     *   @param localPort local port
     *   @exception SocketException thrown if setting local port or address
     * fails
     */
    void set_local_address_and_port(const std::string &local_address,
                                    unsigned short local_port = 0);

    /**
     *   If WinSock, unload the WinSock dlls; otherwise do nothing.  We ignore
     *   this in our sample client code but include it in the library for
     *   completeness.  If you are running on Windows and you are concerned
     *   about dll resource consumption, call this after you are done with all
     *   Socket instances.  If you execute this on Windows while some instance
     *   of Socket exists, you are toast.  for portability of client code, this
     * is an empty function on non-Windows platforms so you can always include
     *   it.
     *   @param buffer buffer to receive the data
     *   @param bufferLen maximum number of bytes to read into buffer
     *   @return number of bytes read, 0 for EOF, and -1 for error
     *   @exception SocketException thrown WinSock clean up fails
     */
    static void cleanup();

    /**
     *   resolve the specified service for the specified protocol to the
     *   corresponding port number in host byte order
     *   @param service service to resolve (e.g., "http")
     *   @param protocol protocol of service to resolve.  default is "tcp".
     */
    static unsigned short resolve_service(const std::string &service,
                                          const std::string &protocol = "tcp");

private:
    // prevent the user from trying to use value semantics on this object
    basic_socket(const basic_socket &sock);
    void operator=(const basic_socket &sock);

protected:
    int sock_desc; // socket descriptor
    basic_socket(int type, int protocol);
    basic_socket(int sock_desc);

}; // class basic_socket

void fill_addr(const std::string &address, unsigned short port,
                             sockaddr_in &addr);

} // namespace xpu