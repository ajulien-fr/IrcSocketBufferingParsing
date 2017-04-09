#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "socket.h"
#include "parser.h"
#include "asprintf.h"
#include "conf.h"

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#define closesocket(x) close(x) // Permet d'utiliser closesocket() sur Windows et Linux
#endif

enum { SOCKET_CANNOT_RESOLVE, SOCKET_CANNOT_CONNECT, SOCKET_CANNOT_SEND, SOCKET_CANNOT_RECEIVE, SOCKET_RECEIVE_NO_DATA };

#define BUFFER_MAX_ELEMENTS 1024

#endif /* __MAIN_H__ */
