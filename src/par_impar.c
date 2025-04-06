//
// Created by angel on 6/04/25.
//

#include "../inc/par_impar.h"

t_list	*create_node(int val)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->value = val;
	new_node->next = NULL;
	return (new_node);
}

void	add_to_even_list(t_global *global, int num)
{
	t_list	*new_node;
	t_list	*current;

	printf("Adding %d to even list\n\n", num);
	new_node = create_node(num);
	if (!new_node)
		return ;
	if (global->list_even == NULL)
		global->list_even = new_node;
	else
	{
		current = global->list_even;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

void	add_to_odd_list(t_global *global, int num)
{
	t_list	*new_node;
	t_list	*current;

	printf("Adding %d to odd list\n\n", num);
	new_node = create_node(num);
	if (!new_node)
		return ;
	if (global->list_odd == NULL)
		global->list_odd = new_node;
	else
	{
		current = global->list_odd;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

/* Procesa un número aleatorio,
	verifica duplicados y lo añade a la lista correspondiente */
bool	process_random_number(t_thread_data *thread_data, int new_num,
		int count)
{
	t_global	*global;
	int			i;

	i = -1;
	global = thread_data->global;
	pthread_mutex_lock(&global->mutex_count);
	if (count >= global->num_per_threads * global->num_threads)
	{
		pthread_mutex_unlock(&global->mutex_count);
		fprintf(stderr, "Error: Index out of bounds in global->numbers\n");
		return (false);
	}
	while (++i < count)
	{
		if (global->numbers[i] == new_num)
		{
			pthread_mutex_unlock(&global->mutex_count);
			return (false);
		}
	}
	global->numbers[count] = new_num;
	printf("The number generated is %d\n", new_num);
	pthread_mutex_unlock(&global->mutex_count);
	if (new_num % 2 == 0)
	{
		pthread_mutex_lock(&global->mutex_even);
		printf("Thread %d: even number: %d\n", thread_data->thread_id, new_num);
		add_to_even_list(global, new_num);
		pthread_mutex_unlock(&global->mutex_even);
	}
	else
	{
		pthread_mutex_lock(&global->mutex_odd);
		printf("Thread %d: odd number : %d\n", thread_data->thread_id, new_num);
		add_to_odd_list(global, new_num);
		pthread_mutex_unlock(&global->mutex_odd);
	}
	return (true);
}

// Genera números aleatorios únicos dentro del range [INT_MIN, INT_MAX]
void	*gen_unique_rand_nums(void *arg)
{
	t_thread_data *thread_data;
	t_global *global;
	int thread_id;
	int num_per_thread;
	int count;
	long long range;
	int new_num;
	long long new_long;

	thread_data = (t_thread_data *)arg;
	global = thread_data->global;
	thread_id = thread_data->thread_id;
	num_per_thread = global->num_per_threads;
	count = 0;
	printf("Thread %d starting to generate %d random numbers\n", thread_id,
		num_per_thread);

	unsigned seed = (unsigned)time(NULL) ^ ((unsigned)thread_data->thread_id << 16);
	srand(seed);

	while (count < num_per_thread)
	{
		range = (long long)INT_MAX - (long long)INT_MIN + 1;
		new_long = (long long)rand() * RAND_MAX + rand();
		new_long = new_long % range + INT_MIN;
		new_num = (int)new_long;

		if (process_random_number(thread_data, new_num, count))
		{
			count++;
		}
	}
	printf("Thread %d finished generating numbers\n\n", thread_id);
	pthread_exit(NULL);
}





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
		finish(global, ERR_ALLOCATE);
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
		finish(global, ERR_ARGS);
		return (EXIT_FAILURE);
	}
	else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1],
				"--help") == 0))
	{
		help();
		exit(EXIT_SUCCESS); // No hace falta liberar nada
	}
	else if (argc == 3 && (strcmp(argv[1], "-f") == 0 || strcmp(argv[1],
				"--file") == 0))
	{
		global = handle_file_option(argc, argv);
		if (!global)
			finish(global, ERR_ALLOCATE);
		printf("File is valid ...\n");
		init_mutex(global);
		init_threads(global);
		thread_create(global);
		destroy_mutex(global);
		free_resources(global);
		exit(EXIT_SUCCESS);
	}
	return (0);
}