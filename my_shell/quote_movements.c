/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_movements.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikadimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 12:56:48 by ikadimi           #+#    #+#             */
/*   Updated: 2019/04/29 12:56:50 by ikadimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	q_delete_and_left(t_counters *t, char *buf)
{
	if (LEFT && t->index > 0)
	{
		if (t->str[t->index - 1] != '\n'
		&& (t->inline_index % tgetnum("co")) != 0)
			tputs(tgetstr("le", NULL), 1, ft_outc);
		t->index--;
		t->inline_index--;
	}
	else if (DEL && t->index > 0)
	{
		t->index--;
		t->inline_index--;
		if (t->str[t->index] == '\n')
			 up_to_edge(t);
		else if (((t->inline_index + 1) % tgetnum("co")) == 0 && t->inline_index > 1)
				up_to_right();
		else
			tputs(tgetstr("le", NULL), 0, ft_outc);
		tputs(tgetstr("cd", NULL), 0, ft_outc);
		tputs(tgetstr("sc", NULL), 0, ft_outc);
		ft_putstr(t->str + t->index + 1);
		tputs(tgetstr("rc", NULL), 0, ft_outc);
		t->str = index_remover(t->str, t->index);
	}
}

void	q_insert_and_right(t_counters *t, char *buf)
{
	if (RIGHT)
	{
		if (t->index < ft_intlen(t->str))
		{
			tputs(tgetstr("nd", NULL), 0, ft_outc);
			t->index++;
			t->inline_index++;
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
		t->inline_index++;
		tputs(tgetstr("sc", NULL), 0, ft_outc);
		ft_putstr(t->str + t->index);
		tputs(tgetstr("rc", NULL), 0, ft_outc);
	}
}