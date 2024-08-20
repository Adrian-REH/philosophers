/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:10:59 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/20 02:18:59 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

long long timestamp(void)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void check_wait(t_rule *rule, int time)
{
	long long i;

	i = timestamp();
	while (!(rule->finish))
	{
		if ((-i + timestamp()) >= time)
			break;
		usleep(150);
	}
	return;
}
