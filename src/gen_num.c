//
// Created by angel on 6/04/25.
//

#include "../inc/par_impar.h"

/* Procesa un número aleatorio,
	verifica duplicados y lo añade a la lista correspondiente */
bool	p_rand_num(t_thread_data *thread_data, int new_num, int count)
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
	t_thread_data	*thread_data;
	t_global		*global;
	int				thread_id;
	int				num_per_thread;
	int				count;
	long long		range;
	int				new_num;
	long long		new_long;
	unsigned		seed;

	thread_data = (t_thread_data *)arg;
	global = thread_data->global;
	thread_id = thread_data->thread_id;
	num_per_thread = global->num_per_threads;
	count = 0;
	printf("Thread %d starting to generate %d random numbers\n", thread_id,
		num_per_thread);
	seed = (unsigned)time(NULL) ^ ((unsigned)thread_data->thread_id << 16);
	srand(seed);
	while (count < num_per_thread)
	{
		range = (long long)INT_MAX - (long long)INT_MIN + 1;
		new_long = (long long)rand() * RAND_MAX + rand();
		new_long = new_long % range + INT_MIN;
		new_num = (int)new_long;
		if (p_rand_num(thread_data, new_num, count))
		{
			count++;
		}
	}
	printf("Thread %d finished generating numbers\n\n", thread_id);
	pthread_exit(NULL);
}
