/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_logs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:20:04 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/19 18:16:27 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void action_print(t_rule *rules, int id, char *string)
{
	pthread_mutex_lock(&(rules->writing));
	pthread_mutex_lock(&(rules->dead_m));
	if (!(rules->finish))
	{
		pthread_mutex_unlock(&(rules->dead_m));
		printf("%lli ", timestamp() - rules->first_timestamp);
		printf("%i ", id + 1);
		printf("%s\n", string);
	}
	else
		pthread_mutex_unlock(&(rules->dead_m));
	pthread_mutex_unlock(&(rules->writing));
	return;
}
