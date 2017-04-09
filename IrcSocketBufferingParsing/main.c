#include "main.h"

static volatile bool isRunning = true;

void consoleCtrlHandler(int i)
{
	isRunning = false;
}

int main(void)
{
	int sock, ret;
	char *buffer = NULL;
	struct parsed_line pline;
	unsigned int nbr_lines = 0, nbr_char_recv = 0, max_char_to_read = 0;
	char line[BUFFER_MAX_ELEMENTS + 1], recv_buffer[BUFFER_MAX_ELEMENTS + 1];
	static char irc_buffer[BUFFER_MAX_ELEMENTS];

	memset(&pline, 0, sizeof(struct parsed_line));

#ifdef WIN32
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		fprintf(stderr, "Error : Initialisation des Sockets Windows impossible.\n");
		exit(EXIT_FAILURE);
	}

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleCtrlHandler, TRUE);
#else
	struct sigaction act;
	act.sa_handler = consoleCtrlHandler;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
#endif

	sock = socket_connect(SERVER, PORT);

	switch (sock)
	{
	case SOCKET_CANNOT_RESOLVE:
		fprintf(stderr, "Error : Resolution de l'hote impossible.\n");
		exit(EXIT_FAILURE);
		break;
	case SOCKET_CANNOT_CONNECT:
		fprintf(stderr, "Error : Serveur inaccessible.\n");
		exit(EXIT_FAILURE);
		break;
	default:
		break;
	}

	asprintf(&buffer, "NICK %s\r\n", NICKNAME);
	ret = socket_send(sock, buffer); // Check ret ?
	free(buffer);

	asprintf(&buffer, "USER %s 0 * :%s\r\n", USERNAME, REALNAME);
	ret = socket_send(sock, buffer); // Check ret ?
	free(buffer);

	while (isRunning == true)
	{
		if (nbr_lines == 0)
		{
			nbr_char_recv = socket_receive(sock, recv_buffer, BUFFER_MAX_ELEMENTS - max_char_to_read);

			if (nbr_char_recv == SOCKET_CANNOT_RECEIVE)
			{
				fprintf(stderr, "Error : Reception impossible.\n");
				ret = 1;
				goto end;
			}

			if (nbr_char_recv != SOCKET_RECEIVE_NO_DATA)
			{
				max_char_to_read += nbr_char_recv;
				nbr_lines = set_irc_buffer(irc_buffer, recv_buffer);
			}
		}

		else
		{
			while (get_irc_line(irc_buffer, line, max_char_to_read))
			{
				printf("line: +%s+\n", line);

				max_char_to_read -= (strlen(line) + 2);
				nbr_lines--;

				parse_irc_line(line, &pline);

				if (!strcmp(pline.command, "PING"))
				{
					asprintf(&buffer, "PONG :%s\r\n", pline.trailing); // Sea IRC RFC
					ret = socket_send(sock, buffer); // Check ret ?
					free(buffer);
				}

				if (pline.trailing != NULL)
				{
					/*
					 * if(!strncmp(pline.trailing, "!join ", 6)) // Some treatment... (popen ? load library (.so .dll) ? ...)
					 */
				}
			}
		}
	}

	ret = 0;

end:
	closesocket(sock);
	free_pline(&pline);

#ifdef WIN32
	WSACleanup();
#endif

	return ret;
}
