#ifndef __PARSER_H__
#define __PARSER_H__

struct parsed_line
{
	char *prefix;
	char *command;
	char **middle;
	char *trailing;
	unsigned int nbr_middle;
};

unsigned int set_irc_buffer(char *irc_buf, char *recv_buf);
bool get_irc_line(char *irc_buf, char *line, unsigned int max_char_to_read);
void parse_irc_line(char *line, struct parsed_line *pline);
void free_pline(struct parsed_line *pline);

#endif /* __PARSER_H__ */
