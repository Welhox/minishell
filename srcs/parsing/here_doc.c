/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:06:47 by clundber          #+#    #+#             */
/*   Updated: 2024/04/24 14:28:48 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	here_doc(t_pipe ***pipe, t_utils *utils, t_ms *ms)
{
	int	i;
	int	x;

	x = -1;
	while ((*pipe)[++x])
	{
		i = -1;
		while ((*pipe)[x]->args[++i])
		{
			handle_sigquit(true);
			if ((*pipe)[x]->tokens[i] == SKIP_HD)
			{
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x], utils, ms);
				close_if_valid_fd((*pipe)[x]->hd_fd[0]);
			}
			else if ((*pipe)[x]->tokens[i] == IN_HD)
				here_doc_open((*pipe)[x]->args[i], (*pipe)[x], utils, ms);
			handle_sigquit(false);
			if (g_signal != 0)
			{
				close_if_valid_fd((*pipe)[x]->hd_fd[0]);
				return ;
			}
		}
	}
}

void	hd_env(char **buff, t_ms *ms)
{
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
	env_variable(ms, quote, dquote, buff);
}

static void	pipe_and_savestdin_hd(t_pipe *_pipe_i, t_ms *ms, int *save_stdin)
{
	*save_stdin = dup_and_check(STDIN_FILENO, ms);
	if (pipe(_pipe_i->hd_fd) == -1)
	{
		ft_putendl_fd("Pipe failed to open", 2);
		free_and_exit(&ms->pipe, &ms->utils, ms, 10);
	}
}

void	here_doc_open(char *eof, t_pipe *_pipe_i, t_utils *utils, t_ms *ms)
{
	char	*buff;
	int		save_stdin;

	pipe_and_savestdin_hd(_pipe_i, ms, &save_stdin);
	while (1)
	{
		buff = readline("> ");
		if (g_signal != 0)
		{
			dup2_and_check(save_stdin, STDIN_FILENO, ms);
			close_if_valid_fd (save_stdin);
			ft_putendl_fd(">", STDOUT_FILENO);
			utils->was_prev_line_null = 1;
			utils->err_code = 1;
			break ;
		}
		if (!buff || ft_strncmp(eof, buff, -1) == 0)
			break ;
		hd_env(&buff, ms);
		ft_putendl_fd(buff, _pipe_i->hd_fd[1]);
		free (buff);
	}
	close_if_valid_fd(_pipe_i->hd_fd[1]);
	close_if_valid_fd (save_stdin);
	free(buff);
}
