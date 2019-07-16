#include "shell.h"

void    q_reverse_position(int len, t_counters *t)
{
    int i;
    int k;
    int virgin;

    i = 0;
    virgin = 0;
    i = len - t->index;
    k = len + t->diff_index;
    while (i-- >= 0)
    {
        if (t->str[len--] == '\n')
        {
            up_to_edge(t);
            virgin = 1;
        }
        else
        {
            tputs(tgetstr("bw", NULL), 0, ft_outc);
            tputs(tgetstr("le", NULL), 0, ft_outc);
        }
        if (virgin == 0)
            t->inline_index++;
        else
            t->inline_index--;
    }
    t->inline_index++;
}

void    q_home_clear(t_counters t)
{
    int i;

    i = line_nb_in_quote(t.str, t.index);
    while (i-- > 0)
        tputs(tgetstr("up", NULL), 0, ft_outc);
    tputs(tgetstr("cr", NULL), 0, ft_outc);
    i = 0;
    while (i++ < t.diff_index)
        tputs(tgetstr("nd", NULL), 0, ft_outc);
    tputs(tgetstr("cd", NULL), 0, ft_outc);
}

void    q_ctl_past(t_counters *t)
{
    if (t->index < ft_intlen(t->str))
    {
        q_home_clear(*t);
        t->str = add_str_in_str(t);
        ft_putstr(t->str);
    }
    else
    {
        ft_putstr(t->copy_str);
        t->str = ft_freejoin(t->str, t->copy_str);
    }
    t->index += ft_intlen(t->copy_str);
    q_reverse_position(ft_intlen(t->str), t);
}

void    q_selection_remover(t_counters *t, int *direction, int *start)
{
    *direction = 0;
    *start = -1;
    tputs(tgetstr("sc", NULL), 0, ft_outc);
    q_home_clear(*t);
    ft_putstr(t->str);
    tputs(tgetstr("rc", NULL), 0, ft_outc);
}

void    q_ctl_cut(t_counters *t, int direction, int start, int  zero)
{
    int j;

    if (zero == -1)
        direction--;
    if (ft_intlen(t->str) == direction || (zero == -1 && ft_intlen(t->str) == -direction))
    {
        q_home_clear(*t);
        t->copy_str = ft_strdup(t->str);
        free(t->str);
        t->str = ft_strdup("");
        t->index = 0;
    }
    else
    {
        q_home_clear(*t);
        t->str = remove_str_in_str(t, direction, start, zero);
        ft_putstr(t->str);
        if (direction < 0)
            q_reverse_position(ft_intlen(t->str), t);
        else
        {
            t->index -= direction;
            q_reverse_position(ft_intlen(t->str), t);
        }
    }
}

void    q_copy_past(t_counters *t, char *buf)
{
    static int start = -1;
    static int direction = 0;
    int        save = 0;
    static int zero = 0;

    if (t->index > 0)
        zero = 0;
    if (!ft_strcmp(buf, "≥") && t->index < ft_intlen(t->str)
     && direction >= 0 && ft_intlen(t->str) >= 1)
    {
        if (start == -1)
            start = t->index;
        direction++;
        tputs(tgetstr("cd", NULL), 0, ft_outc);
        tputs(tgetstr("mr", NULL), 0, ft_outc);
        ft_putstr("\x1b[36m");
        write(1, &t->str[t->index], 1);
        tputs(tgetstr("me", NULL), 0, ft_outc);
        tputs(tgetstr("sc", NULL), 0, ft_outc);
        ft_putstr(t->str + t->index + 1);
        tputs(tgetstr("rc", NULL), 0, ft_outc);
        t->index++;
        t->inline_index++;
        if (((t->inline_index % tgetnum("co")) == 0
        && t->inline_index > 0))
            tputs(tgetstr("do", NULL), 0, ft_outc);
        else if (t->str[t->index - 1] == '\n')
            t->inline_index = 0;
    }
    else if (!ft_strcmp(buf, "≤") && t->index >= 0 &&
    direction <= 0 && zero == 0 && ft_intlen(t->str) >= 1)
    {
        if (t->index == ft_intlen(t->str))
        {
            tputs(tgetstr("le", NULL), 0, ft_outc);
            t->index--;
            t->inline_index--;
        }
        else
        {
            if (start == -1)
                start = t->index;
            save = t->index;
            tputs(tgetstr("cd", NULL), 0, ft_outc);
            tputs(tgetstr("sc", NULL), 0, ft_outc);
            tputs(tgetstr("mr", NULL), 0, ft_outc);
            ft_putstr("\x1b[35m");
            while (save <= start)
                write(1, &t->str[save++], 1);
            tputs(tgetstr("me", NULL), 0, ft_outc);
            ft_putstr(t->str + save);
            tputs(tgetstr("rc", NULL), 0, ft_outc);
            if (t->index > 0)
            {
                t->index--;
                t->inline_index--;
                direction--;
                if (t->str[t->index] == '\n')
                    up_to_edge(t);
                else
                    tputs(tgetstr("le", NULL), 0, ft_outc);
            }
            else
                zero = -1;
        }
    }
    else if (!ft_strcmp(buf, "≤") && t->index >= 0 && direction > 0)
    {
        direction--;
        t->index--;
        t->inline_index--;
        if (t->str[t->index] == '\n')
            up_to_edge(t);
        else
            tputs(tgetstr("le", NULL), 0, ft_outc);
        tputs(tgetstr("cd", NULL), 0, ft_outc);
        tputs(tgetstr("sc", NULL), 0, ft_outc);
        ft_putstr(t->str + t->index);
        tputs(tgetstr("rc", NULL), 0, ft_outc);
    }
    else if (!ft_strcmp(buf, "≥") && t->index >= 0 && direction < 0)
    {
        if (zero == 0)
        {
            direction++;
            t->index++;
            t->inline_index++;
            save = t->index;
            if (((t->inline_index) % tgetnum("co")) == 0)
                tputs(tgetstr("do", NULL), 0, ft_outc);
            else if (t->str[t->index - 1] == '\n')
            {
                tputs(tgetstr("do", NULL), 0, ft_outc);
                t->inline_index = 0;
            }
            else
                tputs(tgetstr("nd", NULL), 0, ft_outc);
        }
        else
            zero = 0;
        tputs(tgetstr("cd", NULL), 0, ft_outc);
        tputs(tgetstr("sc", NULL), 0, ft_outc);
        write(1, &t->str[save++], 1);
        tputs(tgetstr("mr", NULL), 0, ft_outc);
        ft_putstr("\x1b[35m");
        while (save <= start)
            write(1, &t->str[save++], 1);
        tputs(tgetstr("me", NULL), 0, ft_outc);
        ft_putstr(t->str + save);
        tputs(tgetstr("rc", NULL), 0, ft_outc);
    }
    else if (!ft_strcmp(buf, "ç") && (direction != 0 || zero == -1))
    {
        ctl_copy(t, start, direction, zero);
        q_selection_remover(t, &direction, &start);
    }
    else if (!ft_strcmp(buf, "√") && t->copy_str)
        q_ctl_past(t);
    else if (!ft_strcmp(buf, "≈") && (direction != 0 || zero == -1))
    {
        ctl_copy(t, start, direction, zero);
        q_ctl_cut(t, direction, start, zero);
        direction = 0;
        start = -1;
    }
    else if ((direction != 0 || zero == -1) && ft_strcmp(buf, "≥") && ft_strcmp(buf, "≤"))
        q_selection_remover(t, &direction, &start);
   // dprintf(t->ttyfd, "index == %d start == %d zero == %d direction == %d copy_str == %s\n"
   // , t->index, start, zero, direction, t->copy_str);
}