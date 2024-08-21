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

/* Same as usleep but more precise with big numbers */
void ft_usleep(long long time)
{
	long long start;

	start = timestamp();
	while (timestamp() < start + time)
		usleep(10);
}

void check_wait(t_rule *rule, int time)
{
	long long i;

	i = timestamp();
	while (!(rule->finish))
	{
		if ((-i + timestamp()) >= time)
			break;
		usleep(50);
	}
	return;
}
