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
	if (!global)
		return ;
	pthread_mutex_destroy(&global->mutex_even);
	pthread_mutex_destroy(&global->mutex_odd);
	pthread_mutex_destroy(&global->mutex_count);
}

void	finish(t_global *global, const char *message)
{
	int	i;

	if (!global)
	{
		printf("Exiting program: %s\n", message);
		exit(EXIT_FAILURE);
	}
	printf("Terminando programa: %s\n", message);
	if (global->threads)
	{
		i = -1;
		while (++i < global->num_threads)
		{
			pthread_cancel(global->threads[i]);
			pthread_detach(global->threads[i]);
		}
	}
	destroy_mutex(global);
	free_resources(global);
	_exit(EXIT_SUCCESS);
}
