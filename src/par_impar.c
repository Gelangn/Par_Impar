//
// Created by angel on 6/04/25.
//

#include "../inc/par_impar.h"

void	help(void)
{
	printf("The purpose of this program is to generate random numbers and \
separate them into two lists, one of even numbers and one \
of odd numbers.\n");
	printf("Usage: ./ParImpar [-h|--help] or ./ParImpar [-f|--file] \
<file.txt>\n");
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