//
// Created by angel on 6/04/25.
//

#include "../inc/par_impar.h"

int	thread_create(t_global *global)
{
	int	i;

	i = -1;
	while (++i < global->num_threads)
	{
		printf("Creating thread %d\n", global->thread_data[i].thread_id);
		if (pthread_create(&global->threads[i], NULL, gen_unique_rand_nums,
				(void *)&global->thread_data[i]) != 0)
			finish(global, ERR_THREAD);
	}
	thread_join(global);
	return (EXIT_SUCCESS);
}

void	thread_join(t_global *global)
{
	int	i;

	i = -1;
	while (++i < global->num_threads)
	{
		if (pthread_join(global->threads[i], NULL) != 0)
			finish(global, ERR_THREAD);
		printf("Thread %d joined\n", i + 1);
	}
}