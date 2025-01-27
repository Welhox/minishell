/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 21:46:03 by clundber          #+#    #+#             */
/*   Updated: 2024/04/10 19:23:35 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_sep(char **temp, t_ms *ms)
{
	bool	quote;
	bool	dquote;
	int		i;
	int		x;

	x = 0;
	i = 0;
	quote = false;
	dquote = false;
	while (ms->line && ms->line[i])
	{
		quote_status2(&quote, &dquote, ms->line[i]);
		if (quote == false && dquote == false)
		{
			if (ms->line[i] == '>' || ms->line[i] == '|' || ms->line[i] == '<')
				x++;
		}
		i++;
	}
	(*temp) = malloc(sizeof(char) * (i +(x * 2) + 1));
	malloc_check(temp, ms);
}

static void	separator_loop(t_ms *ms, char **temp, int *i, int *x)
{
	int	count;

	(*temp)[(*x)++] = ' ';
	count = 0;
	while (ms->line[(*i)] && count <= 1
		&& ms->line[(*i)] == ms->line[(*i) - count++])
		(*temp)[(*x)++] = ms->line[(*i)++];
	if (ms->line[(*i)] == '<' && (*i) >= 2
		&& ms->line[(*i) - 1] == '<' && ms->line[(*i) - 2 == '<'])
		(*temp)[(*x)++] = ms->line[(*i)++];
	(*temp)[(*x)++] = ' ';
}

char	*separator(t_ms *ms, bool quote, bool dquote, int i)
{
	int		x;
	char	*temp;

	x = 0;
	init_sep(&temp, ms);
	while (ms->line && ms->line[i])
	{
		quote_status2(&quote, &dquote, ms->line[i]);
		if ((ms->line[i] == '>' || ms->line[i] == '|' || ms->line[i] == '<')
			&& (quote == false && dquote == false))
			separator_loop(ms, &temp, &i, &x);
		else
			temp[x++] = ms->line[i++];
	}
	temp[x] = '\0';
	ft_nullfree(&ms->line);
	return (temp);
}
