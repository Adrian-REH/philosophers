/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_logs_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:39:05 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/18 05:43:29 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void	action_print(t_rule *rules, int id, char *string)
{
	sem_wait(rules->writing);
	if (!(rules->finish))
	{
		printf("%lli ", timestamp());
		printf("%i ", id + 1);
		printf("%s\n", string);
	}
	sem_post(rules->writing);
	return ;
}
