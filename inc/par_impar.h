//
// Created by angel on 6/04/25.
//

#ifndef PAR_IMPAR_H
# define PAR_IMPAR_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>

# define ERR_INVALID_EXTENSION "Invalid argument, expected .txt extension\n"
# define ERR_ARG "Usage: ./ParImpar [-h|--help] or ./ParImpar [-f|--file] <file.txt>\n"
# define ERR_ARGS "Incorrect number of arguments, expected 2, " ERR_ARG "\n"
# define ERR_CONFIG "Missing or invalid configuration values\n"
# define ERR_FILE_NOT_FOUND "File not found, please check the file path\n"
# define ERR_OPENING_FILE "Error opening file\n"
# define ERR_EMPTY_FILE "File is empty\n"
# define ERR_INIT "Error initializing resources\n"
# define ERR_ALLOCATE "Failed to allocate memory for thread handles\n"
# define ERR_THREAD "Failed to create/modify threads\n"

/* Structs */

typedef struct s_list
{
	int				value;
	struct s_list	*next;
}					t_list;

// Declaración anticipada de t_global
struct s_global;

// Definición de t_thread_data
typedef struct s_thread_data
{
	int thread_id;           // Identificador único del hilo
	struct s_global *global; // Puntero a la estructura global compartida
}					t_thread_data;

// Definición de t_global
typedef struct s_global
{
	int				num_per_threads;
	int				num_threads;
	int				*numbers;
	t_list			*list_even;
	t_list			*list_odd;
	pthread_t		*threads;
	t_thread_data	*thread_data;
	pthread_mutex_t	mutex_even;
	pthread_mutex_t	mutex_odd;
	pthread_mutex_t	mutex_count;
}					t_global;

/* Prototypes */

// Parsed
bool				file_exists(const char *filename);
bool				file_is_empty(const char *filename);
void				parse_config_line(t_global *global, char *line);
void				process_config_file(t_global *global, const char *filename);
void				check_file_extension(t_global *global,
						const char *filename);
t_global			*handle_file_option(int argc, char *argv[]);

// Inits
void				initialize_global(t_global *global);
void				init_mutex(t_global *global);
void				init_threads(t_global *global);

// Threads
int					thread_create(t_global *global);
void				thread_join(t_global *global);

// Mutexs
void				destroy_mutex(t_global *global);

// Utils
void				finish(t_global *global, const char *message);
bool				process_random_number(t_thread_data *thread_data, int new_num,
		int count);
void				*gen_unique_rand_nums(void *arg);

// Frees
void				free_resources(t_global *global);
void				free_list(t_list *list);
void				destroy_mutex(t_global *global);

#endif // PAR_IMPAR_H
