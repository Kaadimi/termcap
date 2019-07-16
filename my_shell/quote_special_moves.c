#include "shell.h"
/*
void	line_before(t_counters *t)
{
	int i;

	i = 0;
	if (t->str[t->index] == '\n')
		t->index--;
	while (t->str[t->index--] != '\n')
		up_to_edge(t);
}

void    go_prev_line(t_counters *t)
{
    if (t->inline_index == 0)
    {
        tputs(tgetstr("up", NULL), 0, ft_outc);
        t->index--;
        t->inline_index = index_finder(t->str + t->index, t->str);
    }
    if ((t->index + t->diff_index) == t->inline_index)
    {
        q_home_button(t->inline_index, t->diff_index);
        t->index = 0;
        t->inline_index = t->diff_index;
    }
    else if (t->inline_index > 0)
    {
		line_before(t);
        q_home_button(t->inline_index, 0);
        t->index -= t->inline_index;
        t->inline_index = 0;
    }  
}

void	go_next_line(t_counters *t)
{
	if (t->inline_index == index_finder(t->str + t->index, t->str))
		{
			tputs(tgetstr("do", NULL), 0, ft_outc);
			tputs(tgetstr("cr", NULL), 0, ft_outc);
			t->inline_index = 0;
			t->index++;
		}
	if (t->inline_index > 0)
		go_prev_line(t);
	if (t->index == 0)
		q_end_button(line_lenght(t->str + t->index) + t->diff_index);
	else
		q_end_button(line_lenght(t->str + t->index));
	t->inline_index = index_finder(t->str + t->index, t->str);
	if (t->index == 0)
		t->index += (t->inline_index - t->diff_index);
	else
		t->index += t->inline_index;
}*/

void	q_backword_march(t_counters *t)
{
    t->index--;
    t->inline_index--;
    if (t->str[t->index] == '\n')
        up_to_edge(t);
	else if (((t->inline_index + 1) % tgetnum("co")) == 0)
		up_to_right();
	else
		tputs(tgetstr("le", NULL), 0, ft_outc);
}

void	q_forword_march(t_counters *t)
{
	t->index++;
    t->inline_index++;
	if (((t->inline_index) % tgetnum("co")) == 0)
    {
		tputs(tgetstr("do", NULL), 0, ft_outc);
        tputs(tgetstr("cr", NULL), 0, ft_outc);
    }
    else if (t->str[t->index - 1] == '\n')
    {
        tputs(tgetstr("do", NULL), 0, ft_outc);
        tputs(tgetstr("cr", NULL), 0, ft_outc);
        t->inline_index = 0;  
    }
	else
		tputs(tgetstr("nd", NULL), 0, ft_outc);	
}

bool	q_space_before(t_counters t)
{
	int i;

	i = 0;
	while (i < t.index)
		if (t.str[i++] == ' ')
			return (true);
	return (false);
}

bool	q_line_before(t_counters t)
{
	int i;

	i = 0;
	while (i < t.index)
		if (t.str[i++] == '\n')
			return (true);
	return (false);
}

void	q_prev_next_word(t_counters *t, char *buf)
{
	if (prev_word && space_before(*t))
	{
		if (t->str[t->index] == ' ' || t->str[t->index] == '\n')
			q_backword_march(t);
		while (t->str[t->index] != ' ' && t->str[t->index] != '\n')
			q_backword_march(t);
		while (t->str[t->index - 1] == ' ' || t->str[t->index - 1] == '\n')
			q_backword_march(t);
	}
	else if (next_word)
	{
		while ((t->str[t->index] == ' ' || t->str[t->index] == '\n')
        && t->str[t->index])
			q_forword_march(t);
		while (t->str[t->index] != ' ' && t->str[t->index] != '\n'
        && t->str[t->index])
			q_forword_march(t);
	}
}

void	q_prev_next_line(t_counters *t, char *buf)
{
	if (prev_line && q_line_before(*t))
	{
		if (t->str[t->index] == '\n')
			q_backword_march(t);
		while (t->str[t->index] != '\n')
			q_backword_march(t);
	}
	else if (next_line)
	{
		if (t->str[t->index] == '\n' && t->str[t->index])
			q_forword_march(t);
		while (t->str[t->index] != '\n' && t->str[t->index])
			q_forword_march(t);
	}
}