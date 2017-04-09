#include "main.h"

unsigned int set_irc_buffer(char *irc_buf, char *recv_buf)
{
	static unsigned int pos = 0;
	unsigned int nbr_lines = 0;

	for (unsigned int a = 0; recv_buf[a] != 0; a++, pos = (pos + 1) % BUFFER_MAX_ELEMENTS)
	{
		irc_buf[pos] = recv_buf[a];

		if (irc_buf[pos] == '\n')
			nbr_lines++;
	}

	return nbr_lines;
}

bool get_irc_line(char *irc_buf, char *line, unsigned int max_char_to_read)
{
	static unsigned int pos = 0;
	unsigned int last_pos = pos;

	for (unsigned int a = 0; a < max_char_to_read; a++, pos = (pos + 1) % BUFFER_MAX_ELEMENTS)
	{
		line[a] = irc_buf[pos];

		if (irc_buf[pos] == '\n')
		{
			a--; // Erase '\r' // TODO: Check if it's present ?

			line[a] = 0;

			pos = (pos + 1) % BUFFER_MAX_ELEMENTS;

			return true;
		}
	}

	pos = last_pos;

	return false;
}

void parse_irc_line(char *line, struct parsed_line *pline)
{
	char *wptr = line;
	char *wptrtwo = NULL;

	free_pline(pline);

	/* prefix (optionnal) */
	if (*wptr == ':')
	{
		wptr++;
		wptrtwo = wptr;

		for (; (*wptr) && (*wptr != ' '); wptr++);

		pline->prefix = (char *)malloc(sizeof(char) * ((wptr - wptrtwo) + 1));
		strncpy(pline->prefix, wptrtwo, wptr - wptrtwo);
		pline->prefix[wptr - wptrtwo] = 0;

		if (*wptr == ' ') wptr++;
	}

	/* command (obligatory) */
	wptrtwo = wptr;

	for (; (*wptr) && (*wptr != ' ') && (*wptr != ':'); wptr++);

	if (wptr - wptrtwo == 0) return;

	pline->command = (char *)malloc(sizeof(char) * ((wptr - wptrtwo) + 1));
	strncpy(pline->command, wptrtwo, wptr - wptrtwo);
	pline->command[wptr - wptrtwo] = 0;

	if (*wptr == ' ') wptr++;

	/* middle (optional) */
	for (; (*wptr) && (*wptr != ':'); pline->nbr_middle++)
	{
		wptrtwo = wptr;

		for (; (*wptr) && (*wptr != ' ') && (*wptr != ':'); wptr++);

		if (wptr - wptrtwo != 0)
		{
			pline->middle = (char **)realloc(pline->middle, (pline->nbr_middle + 1) * sizeof(char *));
			pline->middle[pline->nbr_middle] = (char *)malloc(sizeof(char) * (wptr - wptrtwo) + 1);
			strncpy(pline->middle[pline->nbr_middle], wptrtwo, wptr - wptrtwo);
			pline->middle[pline->nbr_middle][wptr - wptrtwo] = 0;
		}

		else pline->nbr_middle--;

		if (*wptr == ' ') wptr++;
	}

	/* trailing (optional) */
	if (*wptr == ':')
	{
		wptr++;
		pline->trailing = (char *)malloc(sizeof(char) * (strlen(wptr) + 1));
		strcpy(pline->trailing, wptr);
	}
}

void free_pline(struct parsed_line *pline)
{
	if (pline->prefix != NULL)
	{
		free(pline->prefix);
		pline->prefix = NULL;
	}

	if (pline->command != NULL)
	{
		free(pline->command);
		pline->command = NULL;
	}

	if (pline->middle != NULL)
	{
		for (unsigned int a = 0; a < pline->nbr_middle; a++) free(pline->middle[a]);

		free(pline->middle);
		pline->middle = NULL;
		pline->nbr_middle = 0;
	}

	if (pline->trailing != NULL)
	{
		free(pline->trailing);
		pline->trailing = NULL;
	}
}
