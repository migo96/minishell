/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_set1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migo <migo@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 12:02:52 by migo              #+#    #+#             */
/*   Updated: 2023/02/28 17:05:29 by migo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	**ft_split(char const *str, char c);

t_cmd  *ft_lstlast(t_cmd *lst)
{
        while (lst != NULL)
        {
                if (lst->next == NULL)
                        return (lst);
                lst = lst->next;
        }
        return (0);
}

void    ft_lstadd_back(t_cmd **lst, t_cmd *new)
{
        if (new == 0)
                return ;
        else if (*lst == 0)
                *lst = new;
        else
                (ft_lstlast(*lst))->next = new;
        return ;
}

void	command_same(char **sep_pipe, int *flag)
{
	(*flag) = 0;
	while (**sep_pipe == ' ' && **sep_pipe)
		(*sep_pipe)++;
	if (**sep_pipe == '<' || **sep_pipe == '>')
	{
		if ((*sep_pipe)[1] == '<' || (*sep_pipe)[1] == '>')
			(*sep_pipe)++;
		(*flag) = 1;
		(*sep_pipe)++;
	}
	while (**sep_pipe == ' ' && **sep_pipe)
		(*sep_pipe)++;
}

void	flag_quote(char *sep_pipe, int *quote_flag)
{
	if (*sep_pipe == '\'')
	{
		if (*quote_flag == 0)
			*quote_flag = 1;
		else if (*quote_flag == 1)
			*quote_flag = 0;
	}
	if (*sep_pipe == '\"')
	{
		if (*quote_flag == 0)
			*quote_flag = 2;
		else if (*quote_flag == 2)
			*quote_flag = 0;
	}
}

int	num_option(char *sep_pipe)
{
	int		count;
	int		flag;
	int		quote_flag;

	count = 0;
	quote_flag = 0;
	while (*sep_pipe)
	{
		command_same(&sep_pipe, &flag);
		while ((*sep_pipe != ' ' && *sep_pipe) || quote_flag)
		{
			flag_quote(sep_pipe, &quote_flag);
			if ((*sep_pipe == '<' || *sep_pipe == '>') && quote_flag == 0)
				break ;
			sep_pipe++;
		}
		if (flag == 0 && sep_pipe[-1] != ' ')
			count++;
	}
	return (count);
}

void	word_count(char **option, char *sep_pipe)
{
	int		word_count;
	int		flag;
	int		quote_flag;
	int		i;

	i = 0;
	quote_flag = 0;
	while (*sep_pipe)
	{
		word_count = 0;
		command_same(&sep_pipe, &flag);
		while (*sep_pipe != ' ' && *sep_pipe || quote_flag)
		{
			flag_quote(sep_pipe, &quote_flag);
			if ((*sep_pipe == '<' || *sep_pipe == '>') && quote_flag == 0)
				break ;
			word_count++;
			sep_pipe++;
		}
		if (flag == 0 && sep_pipe[-1] != ' ')
		{
			option[i] = malloc(sizeof(char) * word_count + 1);
			if (option[i] == NULL)
				return ;
			option[i][word_count] = '\0';
			i++;
		}
	}
}

void	put_word(char **option, char *sep_pipe)
{
	int		word_count;
	int		flag;
	int		quote_flag;
	int		i;

	i = 0;
	while (*sep_pipe)
	{
		word_count = 0;
		command_same(&sep_pipe, &flag);
		while (*sep_pipe != ' ' && *sep_pipe || quote_flag)
		{
			flag_quote(sep_pipe, &quote_flag);
			if ((*sep_pipe == '<' || *sep_pipe == '>') && quote_flag == 0)
				break ;
			if (flag == 0 && option[i])
			{
				option[i][word_count] = *sep_pipe;
			}
			word_count++;
			sep_pipe++;
		}
		if (flag == 0)
			i++;
	}
}

char	**get_cmd_option(char *sep_pipe)
{
	char	**option;
	char	*str;
	int		count;

	count = num_option(sep_pipe);
	option = (char **)malloc(sizeof(char *) * count + 1);
	if (option == 0)
		return (0);
	option[count] = NULL;
	word_count(option, sep_pipe);
	put_word(option, sep_pipe);
	return (option);
}

int	count_redirection(char *sep_pipe)
{
	int	i;
	int quote_flag;
	int count;

	i = 0;
	count = 0;
	quote_flag = 0;
	while (sep_pipe[i])
	{
		flag_quote(sep_pipe, &quote_flag);
		if (sep_pipe[i] == '<' && quote_flag == 0)
		{
			if (sep_pipe[i + 1] == '<')
				i++;
			count++;
		}
		if (sep_pipe[i] == '>' && quote_flag == 0)
		{
			if (sep_pipe[i + 1] == '>')
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

void	put_redirection(t_file *file, char *sep_pipe)
{
	int	count;
	int quote_flag;

	count = 0;
	quote_flag = 0;
	while (*sep_pipe)
	{
		file[count].redirection = 0;
		flag_quote(sep_pipe, &quote_flag);
		if ((*sep_pipe == '<' && sep_pipe[1] == '<') && quote_flag == 0)
		{
			file[count].redirection = 1;
			sep_pipe++;
		}
		else if (*sep_pipe == '<' && sep_pipe[1] != '<' && quote_flag == 0)
				file[count].redirection = 2;
		else if (*sep_pipe == '>' && sep_pipe[1] != '>' && quote_flag == 0) 
			file[count].redirection = 3;
		else if (*sep_pipe == '>' && sep_pipe[1] == '>' && quote_flag == 0)
		{
			file[count].redirection = 4;
			sep_pipe++;
		}
		sep_pipe++;
		if (file[count].redirection != 0)
			count++;
	}
}

void	count_file(t_file *file, char *sep_pipe)
{
	int		file_count;
	int		quote_flag;
	int		flag;
	int		i;

	i = 0;
	quote_flag = 0;
	while (*sep_pipe)
	{
		file_count = 0;
		command_same(&sep_pipe, &flag);
		while ((*sep_pipe != ' ' && *sep_pipe) || quote_flag)
		{
			flag_quote(sep_pipe, &quote_flag);
			if ((*sep_pipe == '<' || *sep_pipe == '>') && quote_flag == 0)
				break ;
			file_count++;
			sep_pipe++;
		}
		if (flag == 1 && sep_pipe[-1] != ' ')
		{
			file[i].file = malloc(sizeof(char) * file_count + 1);
			if (file[i].file == NULL)
				return ;
			file[i].file[file_count] = '\0';
			i++;
		}
	}
}

void	put_file(t_file *file, char *sep_pipe)
{
	int		file_count;
	int		quote_flag;
	int		flag;
	int		i;

	i = 0;
	quote_flag = 0;
	while (*sep_pipe)
	{
		file_count = 0;
		command_same(&sep_pipe, &flag);
		while ((*sep_pipe != ' ' && *sep_pipe) || quote_flag)
		{
			flag_quote(sep_pipe, &quote_flag);
			if ((*sep_pipe == '<' || *sep_pipe == '>') && quote_flag == 0)
				break ;
			if (flag == 1 && file[i].redirection != 0)
			{
				file[i].file[file_count] = *sep_pipe;
			}
			file_count++;
			sep_pipe++;
		}
		if (flag == 1)
			i++;
	}
}

t_file	*ft_lstfile(char *sep_pipe)
{
	t_file	*file;
	int count;

	count = count_redirection(sep_pipe);
	file = malloc(sizeof(t_file) * (count + 1));
	if (file == 0)
		return (0);
	file[count].file = NULL;
	file[count].redirection = 0;
	count_file(file, sep_pipe);
	put_redirection(file, sep_pipe);
	put_file(file, sep_pipe);
	return (file);
}

t_cmd	*ft_lstcmd(char *sep_pipe)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd) * 1);
	if (cmd == 0)
		return (0);
	cmd->file = ft_lstfile(sep_pipe);
	cmd->option = get_cmd_option(sep_pipe);
	cmd->next = NULL;
	return (cmd);
}

t_cmd *make_set(char *str)
{
	t_cmd	*cmd;
	char	**sep_pipe;
	int		i;

	i = 1;
	sep_pipe = ft_split(str, '|');
	cmd = ft_lstcmd(sep_pipe[0]);
	while (sep_pipe[i])
	{
		ft_lstadd_back(&cmd, ft_lstcmd(sep_pipe[i]));
		i++;
	}
	return (cmd);
}

int main()
{
	t_cmd *cmd;
	char str[] = "cat -e >a | >>'a>                b -l' cat";
	int j = 1;

	cmd = make_set(str);
	while (cmd)
	{
		int i = 0;
		while (cmd->option[i])
		{
			printf("%d pipe -> option : %s\n",j, cmd->option[i]);
			i++;
		}
		i = 0;
		while (cmd->file[i].redirection != 0)
		{
			printf("redirection : %d\n", cmd->file[i].redirection);
			printf("file : %s\n", cmd->file[i].file);
			i++;
		}
		cmd = cmd->next;
		j++;
	}
}
