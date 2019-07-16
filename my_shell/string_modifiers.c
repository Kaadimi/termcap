/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_modifiers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikadimi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 12:01:46 by ikadimi           #+#    #+#             */
/*   Updated: 2019/04/25 12:02:49 by ikadimi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell.h"

char	*ft_freejoin(char *s1, char const *s2)
{
	char	*fr;
	size_t	v;

	if (!s1 || !s2)
		return (NULL);
	v = ft_strlen(s1) + ft_strlen(s2);
	if (!(fr = (char *)malloc(sizeof(char) * v + 1)))
		return (NULL);
	ft_strcpy(fr, s1);
	ft_strcat(fr, s2);
	free(s1);
	s1 = NULL;
	return (fr);
}

char	*index_remover(char *str, int index)
{
	char *new;
	int	i;

	if (index == 0 && ft_intlen(str) == 0)
		return (ft_strdup(""));
	else if (index > ft_intlen(str) || index < 0)
		return NULL;
	i = -1;
	new = (char *)malloc(sizeof(char) * ft_intlen(str) + 1);
	while (++i < index)
		new[i] = str[i];
	while (str[++i])
		new[i - 1] = str[i];
	new[i - 1] = '\0';
	free(str);
	str = NULL;
	return (new);
}

char	*insert_char(char *str, char c, int index)
{
	char *new;
	int	i;
	int j;

	i = -1;
	new = (char *)malloc(sizeof(char) * (ft_intlen(str) + 2));
	while (++i < index)
		new[i] = str[i];
	j = i;
	new[i++] = c;
	while (str[j])
		new[i++] = str[j++];
	new[i] = '\0';
	free(str);
	str = NULL;
	return (new);
}