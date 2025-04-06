//
// Created by angel on 6/04/25.
//

#include "../inc/par_impar.h"

bool	file_exists(const char *filename)
{
	FILE	*file;

	file = fopen(filename, "r");
	if (file)
	{
		fclose(file);
		return (true);
	}
	return (false);
}

bool	file_is_empty(const char *filename)
{
	FILE	*file;
	int		ch;

	file = fopen(filename, "r");
	if (!file)
		return (true);
	ch = fgetc(file);
	fclose(file);
	return (ch == EOF);
}

void	check_file_extension(t_global *global, const char *filename)
{
	const char	*ext = strrchr(filename, '.');

	if (!ext)
		finish(global, ERR_ARGS);
	else if (strcmp(ext, ".txt") != 0)
		finish(global, ERR_INVALID_EXTENSION);
}

void	parse_config_line(t_global *global, char *line)
{
	char	*equals_sign;
	char	*key;
	char	*value;
	char	*end;

	equals_sign = strchr(line, '=');
	if (!equals_sign)
		return ;
	*equals_sign = '\0';
	key = line;
	value = equals_sign + 1;
	end = equals_sign - 1;
	while (end > key && (*end == ' ' || *end == '\t'))
		*end-- = '\0';
	while (*key == ' ' || *key == '\t')
		key++;
	while (*value == ' ' || *value == '\t')
		value++;
	end = value + strlen(value) - 1;
	while (end > value && (*end == ' ' || *end == '\t' || *end == '\n'
			|| *end == '\r'))
		*end-- = '\0';
	if (strcmp(key, "number_per_thread") == 0)
		global->num_per_threads = atoi(value);
	else if (strcmp(key, "thread_num") == 0)
		global->num_threads = atoi(value);
}

void	process_config_file(t_global *global, const char *filename)
{
	FILE	*file;
	char	*line;
	size_t	len;
	ssize_t	chars_read;

	file = fopen(filename, "r");
	if (!file)
		finish(global, ERR_OPENING_FILE);
	line = NULL;
	len = 0;
	while ((chars_read = getline(&line, &len, file)) != -1)
		parse_config_line(global, line);
	if (line)
		free(line);
	fclose(file);
	if (global->num_per_threads == 0 || global->num_threads == 0)
		finish(global, ERR_CONFIG);
}
