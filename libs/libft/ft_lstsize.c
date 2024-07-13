/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 20:53:59 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/06 21:00:39 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Parameters lst: The beginning of the list.
Return value The length of the list
External functs. None
Description Counts the number of nodes in a list.*/
int	ft_lstsize(t_list *lst)
{
	t_list	*curr;
	int		count;

	count = 0;
	curr = lst;
	while (curr)
	{
		curr = curr->next;
		count++;
	}
	return (count);
}
