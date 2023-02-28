/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hujeong <hujeong@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:ft_isalpha.c
/*   Created: 2022/11/15 14:54:23 by migo              #+#    #+#             */
/*   Updated: 2023/02/27 11:10:04 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if (65 <= c && c <= 90)
		return (0);
	else if (97 <= c && c <= 122)
		return (0);
	else
		return (1);
	return (1);
}
=======
/*   Created: 2023/02/23 13:26:04 by migo              #+#    #+#             */
/*   Updated: 2023/02/28 15:57:59 by hujeong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>

typedef struct s_env
{
	char			*env;
	struct s_env	*next;
}	t_env;

typedef struct s_file
{
	char	*file;
	int		redirection;
}	t_file;

typedef struct s_cmd
{
	t_file			*file;
	char			**option;
	struct s_cmd	*next;
}	t_cmd;

t_env	*g_global;

#endif

// cmd1 | cmd2 | cmd3
// 1       1     0
// dup2(fd[1], 1) dup2(fd[0], 0) dup2(fd[1], 1) dup2(fd[0], 0)
// env를 전역변수로 처리?
>>>>>>> master:src/minishell.h
