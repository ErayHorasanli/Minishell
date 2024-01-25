/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs_builtin1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erhorasa <erhorasa@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 19:57:28 by erhorasa          #+#    #+#             */
/*   Updated: 2024/01/25 20:12:49 by erhorasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void	declare_export(void *data)
{
	t_env	*new;
	int		i;

	new = (t_env *)data;
	printf("\x1b[31mdeclare -x \x1b[0m");
	if (new->key)
		printf("\x1b[92m%s\x1b[0m", new->key);
	if (new->value && *(new->value) && new->key[0] > 31)
		printf("\x1b[92m=\x1b[0m");
	i = 0;
	if (new->value)
		printf("\x1b[92m%s\x1b[0m", new->value);
	printf("\n");
}

int	export_print(char *text, char *cmd, t_shell *m_shell)
{
	char	*key;
	char	*value;

	if (!export_key_control(text))
	{
		write(2, "minishell: export: `", 20);
		write(2, text, ft_strlen(text));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	if (ft_strchrindex_0(text, '=') != -1)
	{
		key = ft_substr(text, 0, ft_strchrindex(text, '='));
		value = ft_substr(text, ft_strchrindex(text, '=') + 1, (ft_strlen(cmd)
					- 1));
	}
	else
	{
		key = ft_substr(text, 0, ft_strlen(text));
		value = NULL;
	}
	edit_env(m_shell->env, key, value, m_shell);
	return (0);
}

int	exec_export(t_parse *data, t_shell *m_shell)
{
	int	i;

	i = 0;
	if (data->text == NULL)
		ft_newlstiter(m_shell->env, declare_export, m_shell);
	else
	{
		while (data->text[i])
		{
			if (export_print(data->text[i], data->cmd, m_shell))
			{
				m_shell->exec_status = 1;
				return (1);
			}
			i++;
		}
	}
	m_shell->exec_status = 0;
	return (0);
}

int	unset_edit(t_list **node, t_list **prev_node, t_shell *m_shell)
{
	t_env	*tmp;

	tmp = (*node)->content;
	if (*prev_node == NULL)
		m_shell->env = (*node)->next;
	else
		(*prev_node)->next = (*node)->next;
	free(tmp->key);
	if (tmp->value)
		free(tmp->value);
	free((*node)->content);
	free(*node);
	return (1);
}

void	exec_unset(t_parse *data, t_shell *m_shell, int i, int x)
{
	t_list	*prev_node;
	t_list	*node;
	t_env	*env;

	env = NULL;
	if (data->text == NULL)
		return ;
	while (data->text[i])
	{
		prev_node = NULL;
		node = m_shell->env;
		while (node != NULL)
		{
			env = (t_env *)node->content;
			if (ft_strcmp(env->key, data->text[i]) == 0)
				x = unset_edit(&node, &prev_node, m_shell);
			if (x)
				break ;
			prev_node = node;
			node = node->next;
		}
		i++;
	}
}
