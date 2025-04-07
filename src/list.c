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

void	print_lists(t_global *global)
{
	t_list	*current_even;
	t_list	*current_odd;

	printf("Even numbers:\n");
	current_even = global->list_even;
	while (current_even != NULL)
	{
		printf("%d ", current_even->value);
		current_even = current_even->next;
	}
	printf("\n");
	printf("Odd numbers:\n");
	current_odd = global->list_odd;
	while (current_odd != NULL)
	{
		printf("%d ", current_odd->value);
		current_odd = current_odd->next;
	}
	printf("\n");
}
