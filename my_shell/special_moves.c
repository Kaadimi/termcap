/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_moves.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikadimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 12:12:54 by ikadimi           #+#    #+#             */
/*   Updated: 2019/04/25 12:12:55 by ikadimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"

void	prev_next_line(t_counters *t, char *buf)
{
	int i;
	int co;

	co = tgetnum("co");
	i = (t->index + t->diff_index) / co;
	if (prev_line && ((t->index + t->diff_index) / co > 0))
	{
		tputs(tgetstr("up", NULL), 0, ft_outc);
		if (t->index < co)
		{
			while (t->index++ < co)
				tputs(tgetstr("nd", NULL), 0, ft_outc);
			t->index = 0;
		}
		else
			t->index -= co;
	}
	else if (next_line &&  i < ((ft_intlen(t->str) + t->diff_index) / co))
	{
		i = 0;
		tputs(tgetstr("do", NULL), 0, ft_outc);
		while (i++ < ((t->index + t->diff_index) % tgetnum("co")))
			tputs(tgetstr("nd", NULL), 0, ft_outc);
		t->index += co;
		while (t->index > ft_intlen(t->str))
		{
			tputs(tgetstr("le", NULL), 0, ft_outc);
			t->index--;
		}
	}
}

void	backword_march(t_counters *t)
{
	if (((t->index + t->diff_index) % tgetnum("co")) == 0)
		up_to_right();
	else
		tputs(tgetstr("le", NULL), 0, ft_outc);
	t->index--;
}

void	forword_march(t_counters *t)
{
	t->index++;
	if (((t->index + t->diff_index) % tgetnum("co")) == 0)
		tputs(tgetstr("do", NULL), 0, ft_outc);
	else
		tputs(tgetstr("nd", NULL), 0, ft_outc);	
}

bool	space_before(t_counters t)
{
	int i;

	i = 0;
	while (i < t.index)
		if (t.str[i++] == ' ')
			return (true);
	return (false);
}

void	prev_next_word(t_counters *t, char *buf)
{
	if (prev_word && space_before(*t))
	{
		if (t->str[t->index] == ' ')
			backword_march(t);
		while (t->str[t->index] != ' ')
			backword_march(t);
		while (t->str[t->index - 1] == ' ')
			backword_march(t);
	}
	else if (next_word)
	{
		while (t->str[t->index] == ' ' && t->str[t->index])
			forword_march(t);
		while (t->str[t->index] != ' ' && t->str[t->index])
			forword_march(t);
	}
}