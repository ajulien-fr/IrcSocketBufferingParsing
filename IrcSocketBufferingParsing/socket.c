#include "main.h"

int socket_connect(const char *address, const char *port)
{
	int sock = -1, ret;
	struct addrinfo *res, *rp, hints;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;		/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;	/* TCP */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;				/* Any protocol */

	ret = getaddrinfo(address, port, &hints, &res);

	if (ret) return (SOCKET_CANNOT_RESOLVE);

	for (rp = res; rp != NULL; rp = rp->ai_next)
	{
		sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

		if (sock == -1) continue;

		if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) break; /* Success */

		closesocket(sock);
	}

	freeaddrinfo(res);

	if (rp == NULL)
	{
		closesocket(sock);
		return (SOCKET_CANNOT_CONNECT);
	}

	return (sock);
}

int socket_send(int sock, const char *buf)
{
	int ret;

	ret = send(sock, buf, strlen(buf), 0);

	if (ret <= 0) return (SOCKET_CANNOT_SEND);

	return ret;
}

int socket_receive(int sock, char *buf, int len)
{
	fd_set readfds;
	struct timeval tv;
	int ret;

	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);

	tv.tv_sec = 0;
	tv.tv_usec = 250000;

	ret = select(sock + 1, &readfds, NULL, NULL, &tv);

	if ((ret < 0) && (errno != EINTR)) return (SOCKET_CANNOT_RECEIVE);

	if (ret == 0) return (SOCKET_RECEIVE_NO_DATA);

	buf[0] = 0;

	ret = recv(sock, buf, len, 0);

	if (ret <= 0) return (SOCKET_CANNOT_RECEIVE);

	buf[ret] = 0;

	return ret;
}
