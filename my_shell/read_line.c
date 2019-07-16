/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikadimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 16:41:03 by ikadimi           #+#    #+#             */
/*   Updated: 2019/03/17 10:41:01 by ikadimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int		ft_intlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	clear_quote(t_counters t)
{
    int		line;
	int		index_line;

    line = line_nb_in_quote(t.str, -1);
	index_line = line_nb_in_quote(t.str, t.index);
	while (index_line <= line)
	{
		tputs(tgetstr("do", NULL), 0, ft_outc);
		index_line++;
	}
}

bool  quote_checker(char *str, int status)
{
	int i;
	int singles;
	int doubles;
	int open;

	open = 0;
	i = 0;
	singles = 0;
	doubles = 0;
	while (str[i])
	{
		if (str[i] == 39 && (status == 0 || status == 1))
		{
			if (open == 3)
				{
					open = -1;
					singles = -1;
					doubles = 0;
				}
			open += 1;
			singles++;
		}
		if (str[i] == '\"' && (status == 0 || status == 2))
		{
			if (open == 3)
				{
					open = -2;
					singles = 0;
					doubles = -1;
				}
			open += 2;
			doubles++;
		}
		i++;
	}
	if ((singles % 2) != 0 || (doubles % 2) != 0)
		return (false);
	return (true);
}

void    cursor_clear(t_counters t)
{
	int  index_line;
	int  max_line;

	index_line = ((t.index + t.diff_index) / tgetnum("co"));
	max_line = ((ft_intlen(t.str) + t.diff_index) / tgetnum("co"));
	while (index_line <= max_line)
	{
		tputs(tgetstr("do", NULL), 0, ft_outc);
		index_line++;
	}
}

char	*ft_read_line(t_history **head, t_counters *t)
{
	char			buf[BUFF_SIZ + 1];
	static int		nb_nodes = 0;
	int				blinker = 0;
	char			*tmp;

	t->ttyfd = open("/dev/ttys001", O_RDWR);
	tmp = ft_strdup("");
	t->start = *head;
	t->str = ft_strdup("");
	struct_initialiser(t);
	set_input_mode();
	ft_putstr("\e[32m21sh>\e[0m ");
	tputs(tgetstr("mr", NULL), 0, ft_outc);
	tputs(tgetstr("me", NULL), 0, ft_outc);
	while (1)
	{
		blinker = 0;
		t->bakara = 0;
		t->ret = read(0, buf, BUFF_SIZ);
		buf[t->ret] = '\0';
		//dprintf(t->ttyfd, buf, "%s\n");
		if ((buf[0] == '\n' && t->ret == 1 && ft_intlen(t->str) > 0) &&
		((quote_checker(t->str, t->status) && t->quote == 0) ||
		 (t->quote > 0 && quote_checker(ft_strjoin(tmp, t->str), t->status))))
		{
			nb_nodes++;
			*head = t->start;
			clear_quote(*t);
			if (t->quote > 0)
				return (ft_strjoin(tmp, ft_strjoin("\n", t->str)));
			return (t->str);
		}
		else if (buf[0] == '\n' && ft_intlen(t->str) > 0 && t->ret == 1)
		{
			if (ft_strstr(t->str, "\"") && t->status == 0)
				t->status = 2;
			else if (ft_strstr(t->str, "\'") && t->status == 0)
				t->status = 1;
			if (t->quote > 0)
				tmp = ft_freejoin(tmp, "\n");
			tmp = ft_freejoin(tmp, t->str);
			cursor_clear(*t);
			free(t->str);
			t->str = ft_strdup("");
			t->diff_index = 13;
			ft_putstr("\e[31mawaFinGhadi>\e[0m ");
			t->index = 0;
			t->quote++;
		}
		up_key(head, t, buf, nb_nodes);
		down_key(head, t, buf);
		if (!ft_strstr(t->str, "\n"))
		{
			copy_past(t, buf);
			prev_next_line(t, buf);
			prev_next_word(t, buf);
			home_and_print(t, buf);
			delete_and_left(t, buf);  
			insert_and_right(t, buf);
			edge_warden(t, buf);
		}
		else if (ft_strstr(t->str, "\n"))
		{
			q_prev_next_line(t, buf);
			q_copy_past(t, buf);
			q_prev_next_word(t, buf);
			q_home_and_print(t, buf);
			q_delete_and_left(t, buf);  
			q_insert_and_right(t, buf);
			quote_edge(t, buf);
		}
	dprintf(t->ttyfd, "real index == %d inline == %d str_lenght == %d\n", t->index, t->inline_index, ft_intlen(t->str));
	}
	return (NULL);
}

