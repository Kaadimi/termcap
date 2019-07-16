#include "shell.h"

void	q_home_button(int index, int diff_index)
{
	int i;

	i = 0;
	while (i < ((index + diff_index) / tgetnum("co")))
	{
		tputs(tgetstr("up", NULL), 0, ft_outc);
		i++;
	}
	tputs(tgetstr("cr", NULL), 0, ft_outc);
	i = 0;
	while (i++ < diff_index)
		tputs(tgetstr("nd", NULL), 0, ft_outc);
}

void	q_end_button(int index)
{
	int i;

	i = 0;
	while (i < (index / tgetnum("co")))
	{
		tputs(tgetstr("do", NULL), 0, ft_outc);
		i++;
	}
	tputs(tgetstr("cr", NULL), 0, ft_outc);
	i = 0;
	while (i++ < (index % tgetnum("co")))
		tputs(tgetstr("nd", NULL), 0, ft_outc);
}

void    go_home(t_counters *t)
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
        q_home_button(t->inline_index, 0);
        t->index -= t->inline_index;
        t->inline_index = 0;
    }  
}

int		str_print(int c)
{
	if ((c >= 32 && c <= 126) || c == 10 || c == '\t')
		return (1);
	return (0);
}

void	go_end(t_counters *t)
{
	if (t->inline_index == index_finder(t->str + t->index, t->str))
		{
			tputs(tgetstr("do", NULL), 0, ft_outc);
			tputs(tgetstr("cr", NULL), 0, ft_outc);
			t->inline_index = 0;
			t->index++;
		}
		if (t->inline_index > 0)
			go_home(t);
		if (t->index == 0)
			q_end_button(line_lenght(t->str + t->index) + t->diff_index);
		else
			q_end_button(line_lenght(t->str + t->index));
		t->inline_index = index_finder(t->str + t->index, t->str);
		if (t->index == 0)
			t->index += (t->inline_index - t->diff_index);
		else
			t->index += t->inline_index;
}

void	q_home_and_print(t_counters *t, char *buf)
{
	if (HOME)
		while (t->index > 0)
			q_backword_march(t);
	else if (END && t->index < ft_intlen(t->str))
		while (t->index < ft_intlen(t->str))
			q_forword_march(t);
	else if (!DEL && t->ret > 1 && str_is_print)
	{
		tab_remover(t, buf);
		if (t->index == ft_intlen(t->str))
		{
			t->str = ft_freejoin(t->str, t->copy_str);
			ft_putstr(t->copy_str);
			t->index += ft_intlen(t->copy_str);
			t->inline_index = index_finder(t->str + t->index, t->str);
		}
		else
		{
			tputs(tgetstr("cd", NULL), 0, ft_outc);
			ft_putstr(t->copy_str);
			t->str = add_str_in_str(t);
			t->index += ft_intlen(t->copy_str);
			ft_putstr(t->str + t->index);
			q_reverse_position(ft_intlen(t->str), t);
			tputs(tgetstr("nd", NULL), 0, ft_outc);
		}
	}
	else if (!DEL && output)
	{
		t->str = ft_freejoin(t->str, buf);
		ft_outc(buf[0]);
		t->index++;
	}
}