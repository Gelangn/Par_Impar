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
