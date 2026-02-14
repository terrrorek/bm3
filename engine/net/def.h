#ifndef NETDEF_H
#define NETDEF_H

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <WinSock2.h>
#  include <ws2tcpip.h>
#  define NETERR WSAGetLastError()

#  undef EWOULDBLOCK
#  define EWOULDBLOCK WSAEWOULDBLOCK
#else
#  include <fcntl.h>
#  include <errno.h>
#  include <sys/socket.h>
#  include <netinet/in.h>

#  define closesocket close
#  define NETERR errno
#endif

#endif
