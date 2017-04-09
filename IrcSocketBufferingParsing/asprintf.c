#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int vasprintf(char **buffer, const char *format, va_list args)
{
	int len = 0;
	va_list wargs;

	va_copy(wargs, args);

	len = vsnprintf(NULL, len, format, wargs);

	va_end(wargs);

	if (len < 0) return -1;

	// allocation avec count + 1 for `\0'
	*buffer = (char *)malloc(len + 1);

	if (*buffer == NULL) return -1;

	len = vsprintf(*buffer, format, args);

	return len;
}

int asprintf(char **buffer, const char *format, ...)
{
	va_list args;
	int len;

	va_start(args, format);
	len = vasprintf(buffer, format, args);
	va_end(args);

	return len;
}
