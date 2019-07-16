/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikadimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 12:09:27 by ikadimi           #+#    #+#             */
/*   Updated: 2019/04/25 12:09:29 by ikadimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"

void	delete_and_left(t_counters *t, char *buf)
{
	if (LEFT && t->index > 0)
	{
		if (!EDGE)
			tputs(tgetstr("le", NULL), 0, ft_outc);
		t->index--;
	}
	else if (DEL && t->index > 0)
	{
		if (EDGE)
				up_to_right();
		else
			tputs(tgetstr("le", NULL), 0, ft_outc);
		tputs(tgetstr("cd", NULL), 0, ft_outc);
		tputs(tgetstr("sc", NULL), 0, ft_outc);
		ft_putstr(t->str + t->index);
		tputs(tgetstr("rc", NULL), 0, ft_outc);
		t->index--;
		t->str = index_remover(t->str, t->index);
	}
}

void	insert_and_right(t_counters *t, char *buf)
{
	if (RIGHT)
	{
		if (t->index < ft_intlen(t->str))
		{
			tputs(tgetstr("nd", NULL), 0, ft_outc);
			t->index++;
		}
		else
			t->bakara = 1;
	}
	else if (INSERT)
	{
		tputs(tgetstr("cd", NULL), 0, ft_outc);
		write(1, &buf[0], 1);
		t->str = insert_char(t->str, buf[0], t->index);
		t->index++;
		tputs(tgetstr("sc", NULL), 0, ft_outc);
		ft_putstr(t->str + t->index);
		tputs(tgetstr("rc", NULL), 0, ft_outc);
	}
}

void	up_key(t_history **head, t_counters *t, char *buf, int nb_nodes)
{
	if ((ft_strstr(t->str, "\n") && t->index == line_lenght(t->str)) ||
	 (t->index == ft_intlen(t->str) && !ft_strstr(t->str, "\n")))
	if (UP && head && t->node_counter <= nb_nodes)
	{	
		t->node_counter++;
		if (t->node_counter == 2)
			ft_putstr((*head)->command);
		else {
			clear_all(line_lenght((*head)->command), t->diff_index);
		if ((*head)->next != NULL)
			*head = (*head)->next;
		ft_putstr((*head)->command);}
		t->str = ft_strdup((*head)->command);
		t->index = ft_intlen(t->str);
		if (ft_strstr(t->str, "\n"))
			start_position_quote(t);
	}
}

void	down_key(t_history **head, t_counters *t, char *buf)
{
	if ((ft_strstr(t->str, "\n") && t->index == line_lenght(t->str)) ||
	 (t->index == ft_intlen(t->str) && !ft_strstr(t->str, "\n")))
	if (DOWN && *head && t->node_counter > 1)
	{	
		t->node_counter--;
		if (t->node_counter > 1)
		{
			clear_all(line_lenght((*head)->command), t->diff_index);
			if ((*head)->prev != NULL)
				*head = (*head)->prev;
			ft_putstr((*head)->command);
			t->str = ft_strdup((*head)->command);
			t->index = ft_intlen(t->str);
		}
		if (t->node_counter == 1)
		{
			free(t->str);
			t->str = ft_strdup("");
			t->index = 0;
			clear_all(line_lenght((*head)->command), t->diff_index);
		}
		if (ft_strstr(t->str, "\n"))
			start_position_quote(t);
	}
}