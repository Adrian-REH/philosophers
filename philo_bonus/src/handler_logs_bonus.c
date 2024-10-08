/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_logs_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:39:05 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/24 00:25:51 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void	action_print(t_philo *philo, char *string)
{
	sem_wait(philo->rule->writing);
	printf("%lli ", timestamp() - philo->rule->first_timestamp);
	printf("%i ", philo->id + 1);
	printf("%s\n", string);
	sem_post(philo->rule->writing);
	return ;
}
