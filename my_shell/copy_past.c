#include "shell.h"

char    *add_str_in_str(t_counters *t)
{
    int     i;
    int     j;
    char    *new;

    j = 0;
    i = ft_intlen(t->str) + ft_intlen(t->copy_str);
    new = (char *)malloc(sizeof(char) * (i + 1));
    i = 0;
    while (i < t->index)
    {
        new[i] = t->str[i];
        i++;
    }
    while (t->copy_str[j])
        new[i++] = t->copy_str[j++];
    j = t->index;
    while (t->str[j])
        new[i++] = t->str[j++];
    new[i] = '\0';
    free(t->str);
    return (new);
}

char    *remove_str_in_str(t_counters *t, int direction, int start, int zero)
{
    int     i;
    int     j;
    char    *new;

    i = 0;
    if (zero == -1 && t->index == 0)
        t->index--;
    if (direction < 0)
    {
        start++;
        new = (char *)malloc(sizeof(char) * (ft_intlen(t->str) + direction + 1));
        while (i <= t->index)
        {
            new[i] = t->str[i];
            i++;
        }
        while (t->str[start])
            new[i++] = t->str[start++];
    }
    else
    {
        new = (char *)malloc(sizeof(char) * (ft_intlen(t->str) - direction + 1));
        while (i < start)
        {
            new[i] = t->str[i];
            i++;
        }
        while (t->str[start + direction])
            new[i++] = t->str[start + direction++];
    }
    if (zero == -1 && t->index == -1)
        t->index++;
    new[i] = '\0';
    free(t->str);
    return (new);
}

void    ctl_copy(t_counters *t, int start, int direction, int zero)
{
    int i;

    i = 0;
    if (zero == -1)
        direction--;
    if (direction < 0)
    {
        if (t->copy_str != NULL)
            free(t->copy_str);
        t->copy_str = (char *)malloc(sizeof(char) * ((direction * -1) + 1));
        while (++direction <= 0)
            t->copy_str[i++] = t->str[start + direction];
    }
    else if (direction > 0)
    {
        if (t->copy_str != NULL)
            free(t->copy_str);
        t->copy_str = (char *)malloc(sizeof(char) * direction + 1);
         while (--direction >= 0)
            t->copy_str[i++] = t->str[start++];
    }
    t->copy_str[i] = '\0';
    dprintf(t->ttyfd, "copy_str == %s t->index == %d\n", t->copy_str, t->index);
}

void    reverse_position(int index, int diff_index, int len)
{
    int i;
    int k;

    i = len - index;
    k = len + diff_index;
    while (i-- > 0)
    {
        tputs(tgetstr("bw", NULL), 0, ft_outc);
        tputs(tgetstr("le", NULL), 0, ft_outc);
    }
}

void    ctl_past(t_counters *t)
{
    if (t->index < ft_intlen(t->str))
    {
        home_button(t->index, t->diff_index);
        tputs(tgetstr("cd", NULL), 0, ft_outc);
        t->str = add_str_in_str(t);
        ft_putstr(t->str);
    }
    else
    {
        ft_putstr(t->copy_str);
        t->str = ft_freejoin(t->str, t->copy_str);
    }
    t->index += ft_intlen(t->copy_str);
    reverse_position(t->index, t->diff_index, ft_intlen(t->str));
}

void    selection_remover(t_counters *t, int *direction, int *start)
{
        *direction = 0;
        *start = -1;
        tputs(tgetstr("sc", NULL), 0, ft_outc);
        home_button(t->index, t->diff_index);
        tputs(tgetstr("cd", NULL), 0, ft_outc);
        ft_putstr(t->str);
        tputs(tgetstr("rc", NULL), 0, ft_outc);
}

void    ctl_cut(t_counters *t, int direction, int start, int  zero)
{
    int i;

    if (zero == -1)
        direction--;
    if (ft_intlen(t->str) == direction || (zero == -1 && ft_intlen(t->str) == -direction))
    {
        home_button(t->index, t->diff_index);
        tputs(tgetstr("cd", NULL), 0, ft_outc);
        t->copy_str = ft_strdup(t->str);
        free(t->str);
        t->str = ft_strdup("");
        t->index = 0;
    }
    else
    {
        i = ft_intlen(t->str);
        home_button(t->index, t->diff_index);
        tputs(tgetstr("cd", NULL), 0, ft_outc);
        t->str = remove_str_in_str(t, direction, start, zero);
        ft_putstr(t->str);
        if (direction < 0)
            reverse_position(t->index, t->diff_index, ft_intlen(t->str));
        else
        {
            reverse_position(t->index, t->diff_index, i);
            t->index -= ft_intlen(t->copy_str);
        }
    }
  //  dprintf(t->ttyfd, "direction == %d start == %d zero == %d copy_str %s\n",
     //   direction, start, zero, t->copy_str);
}

void    copy_past(t_counters *t, char *buf)
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
        if (((t->index + t->diff_index) % tgetnum("co")) == 0)
            tputs(tgetstr("do", NULL), 0, ft_outc);
    }
    else if (!ft_strcmp(buf, "≤") && t->index >= 0 &&
    direction <= 0 && zero == 0 && ft_intlen(t->str) >= 1)
    {
        if (t->index == ft_intlen(t->str))
        {
            tputs(tgetstr("le", NULL), 0, ft_outc);
            t->index--;
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
                if (((t->index + t->diff_index) % tgetnum("co")) == 0)
                    up_to_right();
                else
                    tputs(tgetstr("le", NULL), 0, ft_outc);
                t->index--;
                direction--;
            }
            else
                zero = -1;
        }
    }
    else if (!ft_strcmp(buf, "≤") && t->index >= 0 && direction > 0)
    {
        direction--;
        t->index--;
        if (((t->index + 1 + t->diff_index) % tgetnum("co")) == 0)
            up_to_right();
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
            save = t->index;
            if (((t->index + t->diff_index) % tgetnum("co")) == 0)
                tputs(tgetstr("do", NULL), 0, ft_outc);
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
        selection_remover(t, &direction, &start);
    }
    else if (!ft_strcmp(buf, "√") && t->copy_str)
        ctl_past(t);
    else if (!ft_strcmp(buf, "≈") && (direction != 0 || zero == -1))
    {
        ctl_copy(t, start, direction, zero);
        ctl_cut(t, direction, start, zero);
        direction = 0;
        start = -1;
    }
    else if ((direction != 0 || zero == -1) && ft_strcmp(buf, "≥") && ft_strcmp(buf, "≤"))
        selection_remover(t, &direction, &start);
   // dprintf(t->ttyfd, "index == %d start == %d zero == %d direction == %d copy_str == %s\n"
   // , t->index, start, zero, direction, t->copy_str);
}