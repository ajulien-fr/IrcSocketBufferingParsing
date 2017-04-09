#ifndef __SOCKET_H__
#define __SOCKET_H__

int socket_connect(const char *address, const char *port);
int socket_send(int sock, const char *buf);
int socket_receive(int sock, char *buf, int len);

#endif /* __SOCKET_H__ */
