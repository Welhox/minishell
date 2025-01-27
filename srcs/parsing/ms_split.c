/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 11:18:05 by clundber          #+#    #+#             */
/*   Updated: 2024/04/10 19:25:34 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ms_split(char *str, t_ms *ms)
{
	char	**array;
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
	array = NULL;
	if (!str)
		return (0);
	if (str_count(str) == 0)
		return (0);
	if (str)
	{
		if (str_count(str) == 0)
			return (0);
		array = malloc((str_count(str) + 1) * sizeof(char *));
		if (!array)
			malloc_check(NULL, ms);
		if (ms_splitter(str, &array, quote, dquote) == 1)
			malloc_check(NULL, ms);
		return (array);
	}
	return (0);
}

int	quote_count(char *str)
{
	int		i;
	int		count;
	bool	quote;
	bool	dquote;

	i = 0;
	count = 0;
	quote = false;
	dquote = false;
	while (str[i])
	{
		if (str[i] == '\'' && dquote == false)
		{
			quote_status(&quote);
			count++;
		}
		if (str[i] == '\"' && quote == false)
		{
			quote_status(&dquote);
			count++;
		}
		i++;
	}
	return (count);
}

int	ms_splitter(char *str, char ***array, bool quote, bool dquote)
{
	int		i;
	int		x;
	int		start;

	i = 0;
	x = 0;
	start = 0;
	while (str[x])
	{
		while (str[x] == ' ')
			x++;
		start = x;
		while (str[x] && (str[x] != ' ' || quote == true || dquote == true))
			quote_status2(&quote, &dquote, str[x++]);
		(*array)[i] = ft_substr(str, start, (x - start));
		if (!(*array)[i])
		{
			free_reverse(i -1, (*array));
			return (1);
		}
		i++;
	}
	(*array)[i] = NULL;
	return (0);
}

int	str_count(char *str)
{
	int		count;
	bool	quote;
	bool	dquote;
	int		x;

	x = 0;
	quote = false;
	dquote = false;
	count = 1;
	while (str[x])
	{
		while (str[x] == ' ' && dquote == false && quote == false)
			x++;
		if (str[x])
			count++;
		while (str[x] && (str[x] != ' ' || quote == true || dquote == true))
		{
			if (str[x] == '\'' && dquote == false)
				quote_status(&quote);
			else if (str[x] == '\"' && quote == false)
				quote_status(&dquote);
			x++;
		}
	}
	return (count);
}
