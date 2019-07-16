/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_and_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikadimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 12:21:39 by ikadimi           #+#    #+#             */
/*   Updated: 2019/04/25 12:21:40 by ikadimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"

t_history *create_list(char *command, int placement)
{
	t_history *new;

	new = (t_history *)malloc(sizeof(t_history));
	if (command)
		new->command = ft_strdup(command);
	new->placement = placement;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	node_to_list(t_history **list, t_history *node)
{
	node->next = *list;
	node->prev = NULL;
	if (*list != NULL)
		(*list)->prev = node;
	*list = node;
}

int main()
{
	char *command;
	int  placement;
	t_history *head;
	t_history *added;
	t_counters t;

	placement = 0;
	while (1)
	{
		command = ft_read_line(&head, &t);
		if (placement == 0)
			head = create_list(command, placement);
		else if (placement > 0)
		{
			added = create_list(command, placement);
			node_to_list(&head, added);
		}
		free(command);
		placement++;
	}
	return (0);
}