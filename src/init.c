//
// Created by angel on 6/04/25.
//

#include "../inc/par_impar.h"

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

void	init_mutex(t_global *global)
{
	if (pthread_mutex_init(&global->mutex_even, NULL) != 0)
		finish(global, ERR_INIT);
	if (pthread_mutex_init(&global->mutex_odd, NULL) != 0)
		finish(global, ERR_INIT);
	if (pthread_mutex_init(&global->mutex_count, NULL) != 0)
		finish(global, ERR_INIT);
}

void	init_threads(t_global *global)
{
	int i;

	// Asigna memoria para los identificadores de hilos
	global->threads = malloc(sizeof(pthread_t) * global->num_threads);
	if (!global->threads)
		finish(global, ERR_ALLOCATE);
	// Asigna memoria para los datos específicos de cada hilo
	global->thread_data = malloc(sizeof(t_thread_data) * global->num_threads);
	if (!global->thread_data)
		finish(global, ERR_ALLOCATE);
	i = -1;
	while (++i < global->num_threads)
	{
		global->thread_data[i].thread_id = i + 1;
		global->thread_data[i].global = global;
	}
}