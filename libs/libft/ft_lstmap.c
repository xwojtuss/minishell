/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:29:02 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/11 09:29:22 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Parameters lst: The address of a pointer to a node.
f: The address of the function used to iterate on
the list.
del: The address of the function used to delete
the content of a node if needed.
Return value The new list.
NULL if the allocation fails.
External functs. malloc, free
Description Iterates the list ’lst’ and applies the function
’f’ on the content of each node. Creates a new
list resulting of the successive applications of
the function ’f’. The ’del’ function is used to
delete the content of a node if needed.*/
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*start;

	if (!(lst))
		return (NULL);
	start = NULL;
	while (lst)
	{
		new = (t_list *)malloc(sizeof(t_list));
		if (!new)
			return (NULL);
		new->content = (*f)(lst->content);
		new->next = NULL;
		if (!new)
		{
			ft_lstclear(&new, (*del));
			return (NULL);
		}
		else
			ft_lstadd_back(&start, new);
		lst = lst->next;
	}
	return (start);
}
