/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clundber <clundber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:38:21 by clundber          #+#    #+#             */
/*   Updated: 2024/04/08 11:30:18 by clundber         ###   ########.fr       */
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
			if (utils->export[i +1] && ft_strncmp
				(utils->export[i], utils->export[i +1], -1) > 0)
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

int	export_error(char *arg)
{
	int	x;

	x = 0;
	while (arg[x] && (arg[x] == '\'' || arg[x] == '\"'))
		x++;
	if ((arg[x] >= '0' && arg[x] <= '9') || arg[x] == '=')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putendl_fd("\': not a valid identifier", 2);
		return (1);
	}
	while (arg[x] && arg[x] != '=')
	{
		if (!((arg[x] >= '0' && arg[x] <= '9') || (arg[x] >= 'a' && arg[x] <= 'z')
			|| (arg[x] >= 'A' && arg[x] <= 'Z') || (arg[x] == '_')))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(arg, 2);
			ft_putendl_fd("\': not a valid identifier", 2);
			return (1);
		}
		x++;
	}
	return (0);
}

void	export_loop(char *arg, t_utils *utils, bool quote, bool dquote)
{
	int		i;
	char	*temp;
	char	*temp2;

	temp2 = NULL;
	temp = NULL;
	i = 0;
	while (arg[i])
	{
		quote_status2(&quote, &dquote, arg[i]);
		if (arg[i] == '=' && quote == false && dquote == false)
		{
			temp2 = ft_substr(arg, 0, (i +1));
			malloc_error2(&temp2);
			change_env_var(&utils, temp2, arg);
			temp = jointhree("declare -x ", temp2, "\"");
			ft_nullfree(&temp2);
			malloc_error2(&temp);
			temp2 = ft_free_strjoin(temp, ft_substr(arg, (i +1), (ft_strlen(arg) - i)));
			malloc_error2(&temp2);
			temp = ft_strjoin(temp2, "\"");
			ft_nullfree(&temp2);
			malloc_error2(&temp);
			temp2 = ft_strdup("declare -x ");
			malloc_error2(&temp2);
			temp2 = ft_free_strjoin(temp2, ft_substr(arg, 0, (i +1)));
			malloc_error2(&temp2);
			change_exp_var(&utils, temp2, temp);
			ft_nullfree(&temp);
			ft_nullfree(&temp2);
			break ;
		}
		if (arg[++i] == '\0')
		{
			temp = ft_strjoin("declare -x ", arg);
			malloc_error2(&temp);
			add_exp_var(&utils, temp);
			ft_nullfree(&temp);
		}
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
		if (export_error(arg[i]) != 0)
			return (1);
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
void	unset_env(t_utils *utils, char **arg)
{
	int	y;
	int	x;
	int	i;

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
					remove_env(utils, i, x, y);
			}
			i++;
		}
		y++;
	}

}

void	unset_exp(t_utils *utils, char **arg, int j, int i)
{
	int	x;
	int	y;

	while (arg[++j])
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
				if (arg[j][x] == utils->export[i][y++])
					x++;
				else
					break ;
				if (arg[j][x] == '\0' && (utils->export[i][y] == '='
					|| utils->export[i][y] == '\0'))
					remove_exp(utils, i, x, y);
			}
			i++;
		}
	}
}

int	unset(t_utils *utils, char **arg)
{
	int		i;
	int		j;

	j = 0;
	i = 0;
	if (!arg || !arg[1] || !arg[1][0])
		return (0);
	unset_env(utils, arg);
	unset_exp(utils, arg, j, i);
	return (0);
}


int	remove_exp(t_utils *utils, int i, int x, int y)
{
	char	**temp_arr;

	y = 0;
	x = 0;
	while (utils->export[x])
		x++;
	temp_arr = malloc(sizeof(char *) * x);
	if (!temp_arr)
		malloc_error(1);
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

int	remove_env(t_utils *utils, int i, int x, int y)
{
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
