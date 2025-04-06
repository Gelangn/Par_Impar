//
// Created by angel on 6/04/25.
//

#include "../inc/par_impar.h"

void	free_resources(t_global *global)
{
	if (!global)
		return ;
	if (global->list_even)
	{
		free_list(global->list_even);
		global->list_even = NULL;
	}
	if (global->list_odd)
	{
		free_list(global->list_odd);
		global->list_odd = NULL;
	}
	if (global->numbers)
	{
		free(global->numbers);
		global->numbers = NULL;
	}
	if (global->threads)
	{
		free(global->threads);
		global->threads = NULL;
	}
	if (global->thread_data)
	{
		free(global->thread_data);
		global->thread_data = NULL;
	}
	free(global);
	global = NULL;
}

void	free_list(t_list *list)
{
	t_list	*current;
	t_list	*next;

	current = list;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

void	destroy_mutex(t_global *global)
{
	pthread_mutex_destroy(&global->mutex_even);
	pthread_mutex_destroy(&global->mutex_odd);
	pthread_mutex_destroy(&global->mutex_count);
}

void	finish(t_global *global, const char *message)
{
	if (errno == 0)
	{
		printf("Exiting program: %s\n", message);
		free_resources(global);
		exit(EXIT_SUCCESS);
	}
	else
	{
		perror(message);
		printf("Exiting program\n");
		free_resources(global);
		exit(EXIT_FAILURE);
	}
}

void	help(void)
{
	printf("The purpose of this program is to generate random numbers and \
separate them into two lists, one of even numbers and one \
of odd numbers.\n");
	printf("Usage: ./ParImpar [-h|--help] or ./ParImpar [-f|--file] \
<file.txt>\n");
}
void	initialize_global(t_global *global)
{
	global->numbers = malloc(sizeof(int) * global->num_per_threads
			* global->num_threads);
	if (!global->numbers)
		finish(global, ERR_ALLOCATE);
	global->list_even = NULL;
	global->list_odd = NULL;
	global->threads = NULL;
	global->thread_data = NULL;
}

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
t_global	*handle_file_option(int argc, char *argv[])
{
	t_global	*global;

	(void)argc;
	global = malloc(sizeof(t_global));
	if (!global)
	{
		finish(global, ERR_ALLOCATE);
	}
	global->list_even = NULL;
	global->list_odd = NULL;
	global->numbers = NULL;
	global->threads = NULL;
	global->thread_data = NULL;
	if (!argv[2])
		finish(global, ERR_ARG);
	if (!file_exists(argv[2]))
		finish(global, ERR_FILE_NOT_FOUND);
	if (file_is_empty(argv[2]))
		finish(global, ERR_EMPTY_FILE);
	check_file_extension(global, argv[2]);
	process_config_file(global, argv[2]);
	if (global->num_per_threads == 0 || global->num_threads == 0)
		finish(global, ERR_CONFIG);
	initialize_global(global);
	return (global);
}
int	main(int argc, char *argv[])
{
	t_global *global;

	global = NULL;
	if (argc < 2 || argc > 3)
	{
		return (EXIT_FAILURE);
	}
	else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1],
				"--help") == 0))
	{
		help();
		exit(EXIT_SUCCESS);
	}
	else if (argc == 3 && (strcmp(argv[1], "-f") == 0 || strcmp(argv[1],
				"--file") == 0))
	{
		global = handle_file_option(argc, argv);
		if (!global)
			exit(EXIT_FAILURE);
		printf("File is valid ...\n");
		exit(EXIT_SUCCESS);
	}
	return (0);
}