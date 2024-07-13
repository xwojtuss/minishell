/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 21:26:15 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/06 21:49:03 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Parameters lst: The address of a pointer to a node.
del: The address of the function used to delete
the content of the node.
Return value None
External functs. free
Description Deletes and frees the given node and every
successor of that node, using the function ’del’
and free(3).
Finally, the pointer to the list must be set to
NULL.*/
void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;

	if (!(del) || !(lst) || !(*lst))
		return ;
	while (lst && (*lst))
	{
		tmp = (*lst)->next;
		if (!(*lst) || !(del))
			return ;
		(*del)((*lst)->content);
		(*lst)->next = NULL;
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
