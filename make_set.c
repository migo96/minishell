/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 15:04:25 by migo              #+#    #+#             */
/*   Updated: 2023/02/28 16:27:30 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	ft_isalnum(int c)
{
	if ('a' <= c && c <= 'z')
		return (c);
	else if ('A' <= c && c <= 'Z')
		return (c);
	else if ('0' <= c && c <= '9')
		return (c);
	else
		return (0);
}

char	**ft_split(char const *str, char c);

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	num_env_word(char *str, t_list *env)
{
	int	i;

	while (env)
	{
		i = 0;
		while (str[i] != ' ' && str[i])
		{
			if (str[i] != env->env[i])
				break ;
			i++;
		}
		if (env->env[i] == '=' && ft_isalnum(str[i]))
			return (ft_strlen(&(env->env[i + 1])));
		env = env->next;
	}
	while (str[i] != ' ' && str[i])
		i++;
	return (-i - 1);
}

char	*quote(char *str, t_list *env)
{
	int		i;
	int		j;
	int		flag;
	char	*src;

	i = 0;
    j = 0;
	flag = 0;
	while (str[i])
	{
		if (str[i] == '$' && flag == 0)
			j += num_env_word(&str[i + 1], env);
		if (str[i] == '\'')
		{
			if (flag == 0)
				flag = 1;
			else
				flag = 0;
		}
		i++;
	}
	printf("%d %d \n", i, j);
	src = malloc(sizeof(char) * (i + j + 1));
	if (src == 0)
		return (0);
    return (src);
}

int main()
{
	t_list *env;
	char str[] = "$ABC 123123$222";

	env =malloc(sizeof(t_list) * 1);
	env->env = "ABC=12345";
	env->next = NULL;
	quote(str, env);
}
