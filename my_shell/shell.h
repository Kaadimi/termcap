/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikadimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 11:58:24 by ikadimi           #+#    #+#             */
/*   Updated: 2019/04/25 12:01:07 by ikadimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
#define SHELL_H

#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <term.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../minishell/libft/libft.h"
# define BUFF_SIZ 4
# define RIGHT (buf[2] == 67 && buf[1] == 91 && buf[0] == 27)
# define LEFT (buf[2] == 68 && buf[1] == 91 && buf[0] == 27)
# define UP (buf[2] == 65 && buf[1] == 91 && buf[0] == 27)
# define DOWN (buf[2] == 66 && buf[1] == 91 && buf[0] == 27)
# define HOME (buf[2] == 72 && buf[1] == 91 && buf[0] == 27)
# define END (buf[2] == 70 && buf[1] == 91 && buf[0] == 27)
# define DEL (buf[0] == 127)
# define EDGE (((t->index + t->diff_index) % tgetnum("co")) == 0 && t->index > 0)
# define INSERT (ft_isprint(buf[0]) && t->index < ft_intlen(t->str) && t->ret == 1)
# define LINE ((t->index + t->diff_index) / tgetnum("co"))
# define prev_word (buf[3] == 68 && buf[2] == 91 && buf[1] == 27 && buf[0] == 27)
# define prev_line (buf[3] == 65 && buf[2] == 91 && buf[1] == 27 && buf[0] == 27)
# define next_line (buf[3] == 66 && buf[2] == 91 && buf[1] == 27 && buf[0] == 27)
# define next_word (buf[3] == 67 && buf[2] == 91 && buf[1] == 27 && buf[0] == 27)
# define output (ft_isprint(buf[0]) && t->ret == 1 && t->index == ft_intlen(t->str))
# define str_is_print (str_print(buf[3]) && str_print(buf[2]) && str_print(buf[1]) && str_print(buf[0]))

typedef struct s_history
{
	char *command;
	int	 placement;
	struct s_history *next;
	struct s_history *prev;	
}				t_history;

typedef struct  s_counters
{
	int			index;
	int			inline_index;
	int			diff_index;
	int			back_up_index;
	int			node_counter;
	char		*str;
	int			ret;
	t_history	*start;
	int			bakara;
	int			quote;
	int			ttyfd;
	int			status;
	char		*copy_str;
}				t_counters;


void		struct_initialiser(t_counters *t);
int			ft_intlen(char *str);
char		*ft_freejoin(char *s1, char const *s2);
int			ft_outc(int c);
char		*index_remover(char *str, int index);
char		*insert_char(char *str, char c, int index);
void		set_input_mode();
void		up_to_right();
void		home_button(int index, int diff_index);
void		clear_all(int index, int diff_index);
void		end_button(int index);
void		delete_and_left(t_counters *t, char *buf);
void		insert_and_right(t_counters *t, char *buf);
void		edge_warden(t_counters *t, char *buf);
void		clear_quote(t_counters t);
void		up_key(t_history **head, t_counters *t, char *buf, int nb_nodes);
void		down_key(t_history **head, t_counters *t, char *buf);
void		home_and_print(t_counters *t, char *buf);
bool		space_before(t_counters t);
void		prev_next_line(t_counters *t, char *buf);
void		backword_march(t_counters *t);
void		forword_march(t_counters *t);
void		prev_next_word(t_counters *t, char *buf);
bool		quote_checker(char *str, int status);
void		cursor_clear(t_counters t);
char		*ft_read_line(t_history **head, t_counters *t);
t_history 	*create_list(char *command, int placement);
void		node_to_list(t_history **list, t_history *node);
void    	quote_edge(t_counters *t, char *buf);
int     	line_lenght(char *str);
void    	start_position_quote(t_counters *t);
void		q_delete_and_left(t_counters *t, char *buf);
void		q_insert_and_right(t_counters *t, char *buf);
void    	up_to_edge(t_counters *t);
int  		index_finder(char *find, char *str);
int     	line_nb_in_quote(char *str, int index);
int     	line_lenght(char *str);
void		q_home_and_print(t_counters *t, char *buf);
void		q_prev_next_word(t_counters *t, char *buf);
void    	copy_past(t_counters *t, char *buf);
void		q_prev_next_line(t_counters *t, char *buf);
void		ctl_copy(t_counters *t, int start, int direction, int zero);
void    	q_copy_past(t_counters *t, char *buf);
char    	*add_str_in_str(t_counters *t);
char    	*remove_str_in_str(t_counters *t, int direction, int start, int zero);
void    	ctl_copy(t_counters *t, int start, int direction, int zero);
void    	reverse_position(int index, int diff_index, int len);
void    	ctl_past(t_counters *t);
void    	ctl_cut(t_counters *t, int direction, int start, int  zero);
void    	selection_remover(t_counters *t, int *direction, int *start);
void		q_home_button(int index, int diff_index);
void		q_end_button(int index);
int			str_print(int c);
void		tab_remover(t_counters *t, char *buf);
void    	q_reverse_position(int len, t_counters *t);
void    	q_home_clear(t_counters t);
void		q_backword_march(t_counters *t);
void		q_forword_march(t_counters *t);

#endif
