/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:56:02 by clundber          #+#    #+#             */
/*   Updated: 2024/03/29 13:31:55 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_handler(void)
{
	//struct sigaction sa;

/* 	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &sa, NULL); */
	//signal(SIGINT, handle_sigint);  //ctrt + c should quit back to promt
	//signal(SIGKILL, NULL); // ctrl + \ hould do nothing
									// ctr + D should act as eof
	struct sigaction	sa1;
	struct sigaction	sa2;

	sa1.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa1, NULL);
	sa2.sa_handler = &handle_sigint;
	sa2.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa2, NULL);

}

void	handle_sigint(int sig)
{
	(void)sig;
 	//int save_stdin;

	g_signal = 130;
	//ft_putendl_fd("g_signal = 130", 2);
	//write(save_stdin, '\0', 1);
	//dup2 (save_stdin, STDIN_FILENO);
	//ft_putstr_fd("\n", 1);
	// rl_on_new_line();
	//rl_redisplay();
	//ft_putchar_fd('\0', 0);
	//ft_putchar_fd('\n', STDIN_FILENO);
	close (STDIN_FILENO);
	//ft_putendl_fd("", 1);
	//rl_replace_line("", 0);
	//dup2 (save_stdin, STDIN_FILENO);
	//ft_putstr_fd("minishell-0.5$ ", 1);
}

