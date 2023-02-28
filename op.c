# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

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

void  flag_quote(char **sep_pipe, int *quote_flag)
{  
    if (**sep_pipe == '\'' || **sep_pipe == '\"')
    {
        if (*quote_flag == 0)
            *quote_flag = 1;
        else
        {
            *quote_flag = 0;
        }
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
			flag_quote(&sep_pipe, &quote_flag);
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
		while ((*sep_pipe != ' ' && *sep_pipe) || quote_flag)
		{
			flag_quote(&sep_pipe, &quote_flag);
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
		while ((*sep_pipe != ' ' && *sep_pipe) || quote_flag)
		{
			flag_quote(&sep_pipe, &quote_flag);
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

int main()
{
	int i = 0;
    char str[] = "<a ls \"asd bsas\"  \"asdasda\" >a>b-k";
	char **option;

	option = get_cmd_option(str);
	while (option[i])
	{
		printf("%s\n", option[i]);
		i++;
	}
}
