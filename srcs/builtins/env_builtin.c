/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:38:21 by clundber          #+#    #+#             */
/*   Updated: 2024/03/28 18:02:23 by clundber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_exp(t_utils *utils, int fd)
{
	int	i;

	i = 0;
	if (!utils->export)
		return ;
	while (utils->export[i])
	{
		ft_putendl_fd(utils->export[i], fd);
		i++;
	}
}

void	sort_export(t_utils *utils)
{
	int		i;
	char	*temp;
	bool	sorted;

	sorted = false;
	while (sorted == false)
	{
		sorted = true;
		i = 0;
		while (utils->export[i])
		{
			if (utils->export[i +1] &&
				ft_strncmp(utils->export[i], utils->export[i +1], -1) > 0)
			{
				sorted = false;
				temp = utils->export[i];
				utils->export[i] = utils->export[i +1];
				utils->export[i +1] = temp;
			}
			i++;
		}
	}
}

int	export_error(char *arg, t_utils *utils)
{
	int	x;

	x = 0;
	while (arg[x] && (arg[x] == '\'' || arg[x] == '\"'))
		x++;
	if (arg[x] >= '0' && arg[x] <= '9')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("\': not a valid identifier", 2);
		utils->err_code = 1;
		return (1);
	}
	return (0);
}

void	export_loop(char *arg, t_utils *utils, bool quote, bool dquote)
{
	int		i;
	char	*temp;

	temp = NULL;
	i = 0;
	while (arg[i])
	{
		quote_status2(&quote, &dquote, arg[i]);
		if (arg[i] == '=' && quote == false && dquote == false)
		{
			temp = jointhree("declare -x ", ft_substr(arg, 0, (i +1)), "\"");
			if (!temp)
				malloc_error (1);
			temp = jointhree(temp, ft_substr(arg, (i +1),
						(ft_strlen(arg) - i)), "\"");
			if (!temp)
				malloc_error (1);
			change_env_var(&utils, ft_substr(arg, 0, (i +1)), arg);
			change_exp_var(&utils, ft_strjoin("declare -x ",
					ft_substr(arg, 0, (i +1))), temp);
			break ;
		}
		i++;
		if (arg[i] == '\0')
			add_exp_var(&utils, ft_strjoin("declare -x ", arg));
	}
}

int	export(t_utils *utils, char **arg)
{
	int		i;
	bool	quote;
	bool	dquote;

	quote = false;
	dquote = false;
	i = 1;
	if (!utils)
		return (1);
	if (!arg || !arg[0])
		return (0);
	if (!arg[1])
	{
		print_exp(utils, 1);
		return (0);
	}
	while (arg[0] && arg[i])
	{
		if (export_error(arg[i], utils) == 0)
			export_loop(arg[i], utils, quote, dquote);
		i++;
	}
	sort_export(utils);
	return (0);
}

int	env(t_utils *utils)
{
	int	i;

	i = 0;
	if (!utils->envp)
		return (0);
	while (utils->envp[i])
	{
		ft_putendl_fd(utils->envp[i], 1);
		i++;
	}
	return (0);
}

int	pwd(t_utils *utils)
{
	int		i;
	char	*temp;

	temp = NULL;
	i = 0;
	if (!utils->envp)
		return (0);
	while (utils->envp[i])
	{
		if (strncmp(utils->envp[i], "PWD=", 4) == 0)
		{
			temp = ft_substr(utils->envp[i], 4, ft_strlen(utils->envp[i]) -4);
			printf("%s\n", temp);
			free (temp);
		}
		i++;
	}
	return (0);
}

int	unset(t_utils *utils, char **arg)
{
	int		i;
	int		x;
	int		y;
	int		j;

	if (!arg || !arg[1] || !arg[1][0])
		return (0);
	y = 1;
	while (arg[y])
	{
		i = 0;
		while (arg[y] && utils->envp[i])
		{
			x = 0;
			while (arg[y][x] && utils->envp[i][x])
			{
				if (arg[y][x] == utils->envp[i][x])
					x++;
				else
					break ;
				if (arg[y][x] == '\0' && utils->envp[i][x] == '=')
					remove_env(utils, i);
			}
			i++;
		}
		y++;
	}
	j = 1;
	while (arg[j])
	{
		i = 0;
		while (arg[j] && utils->export[i])
		{
			x = 0;
			y = 0;
			while (utils->export[i][y] && y < 11)
				y++;
			while (arg[j][x] && utils->export[i][y])
			{
				if (arg[j][x] == utils->export[i][y])
				{
					x++;
					y++;
				}
				else
					break ;
				if (arg[j][x] == '\0' && (utils->export[i][y] == '=' || utils->export[i][y] == '\0'))
					remove_exp(utils, i);
			}
			i++;
		}
		j++;
	}
	return (0);
}

int	remove_exp(t_utils *utils, int i)
{
	int		x;
	int		y;
	char	**temp_arr;

	y = 0;
	x = 0;
	while (utils->export[x])
		x++;
	temp_arr = malloc(sizeof(char *) * x);
	if (!temp_arr)
		return (1);
	x = 0;
	while (utils->export[x])
	{
		if (x != i)
		{
			temp_arr[y] = ft_strdup(utils->export[x]);
			y++;
			x++;
		}
		else
			x++;
	}
	temp_arr[y] = NULL;
	ft_arrfree(utils->export);
	utils->export = temp_arr;
	return (0);
}

int	remove_env(t_utils *utils, int i)
{
	int		x;
	int		y;
	char	**temp_arr;

	y = 0;
	x = 0;
	while (utils->envp[x])
		x++;
	temp_arr = malloc(sizeof(char *) * x);
	if (!temp_arr)
		return (1);
	x = 0;
	while (utils->envp[x])
	{
		if (x != i)
		{
			temp_arr[y] = ft_strdup(utils->envp[x]);
			y++;
			x++;
		}
		else
			x++;
	}
	temp_arr[y] = NULL;
	ft_arrfree(utils->envp);
	utils->envp = temp_arr;
	return (0);
}
