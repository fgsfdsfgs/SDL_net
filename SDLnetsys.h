/*
  SDL_net:  An example cross-platform network library for use with SDL
  Copyright (C) 1997-2017 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/* $Id$ */

/* Include normal system headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(__OS2__) && !defined(__EMX__)
#include <nerrno.h>
#elif !defined(_WIN32_WCE)
#include <errno.h>
#endif

/* Include system network headers */
#if defined(__WIN32__) || defined(WIN32)
#define __USE_W32_SOCKETS
#ifdef _WIN64
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <winsock.h>
/* NOTE: windows socklen_t is signed
 * and is defined only for winsock2. */
typedef int socklen_t;
#endif /* W64 */
#include <iphlpapi.h>
#else /* UNIX */
#ifdef __OS2__
#include <sys/param.h>
#endif
#include <sys/types.h>
#ifdef __FreeBSD__
#include <sys/socket.h>
#endif
#ifndef __vita__
#include <sys/ioctl.h>
#endif
#include <sys/time.h>
#ifdef __vita__
#include <sys/select.h>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#ifndef __vita__
#ifndef __BEOS__
#include <arpa/inet.h>
#endif
#include <netinet/tcp.h>
#endif
#include <sys/socket.h>
#ifndef __vita__
#include <net/if.h>
#endif
#include <netdb.h>
#endif /* WIN32 */

#ifdef __OS2__
typedef int socklen_t;
#elif 0
/* FIXME: What platforms need this? */
typedef Uint32 socklen_t;
#endif

/* System-dependent definitions */
#ifndef __USE_W32_SOCKETS
#ifdef __OS2__
#define closesocket     soclose
#else  /* !__OS2__ */
#define closesocket close
#endif /* __OS2__ */
#define SOCKET  int
#define INVALID_SOCKET  -1
#define SOCKET_ERROR    -1
#endif /* __USE_W32_SOCKETS */

#ifdef __USE_W32_SOCKETS
#define SDLNet_GetLastError WSAGetLastError
#define SDLNet_SetLastError WSASetLastError
#ifndef EINTR
#define EINTR WSAEINTR
#endif
#else
int SDLNet_GetLastError(void);
void SDLNet_SetLastError(int err);
#endif

#ifdef __vita__
// These are all defined in SDLnetvita.c

int SDLNet_Vita_InitNet(void);
void SDLNet_Vita_QuitNet(void);

// These are missing from our newlib, so replacements are provided in
// SDLnetvita.c.

char *_vita_inet_ntoa(struct in_addr in);
in_addr_t _vita_inet_addr(const char *cp);
struct hostent *_vita_gethostbyaddr(const void *addr, socklen_t len, int type);

#undef inet_ntoa
#undef inet_addr
#undef gethostbyaddr
#define inet_ntoa _vita_inet_ntoa
#define inet_addr _vita_inet_addr
#define gethostbyaddr _vita_gethostbyaddr

// SDL2_net looks for these and uses them exactly as rand() and srand()
// for some reason, and we don't have them in libc.

#undef random
#undef srandom
#define random rand
#define srandom srand

#endif
