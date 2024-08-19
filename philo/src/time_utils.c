/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:09:13 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/19 18:21:34 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

long long	timestamp(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

/* Same as usleep but more precise with big numbers */
void	ft_usleep(long long time)
{
	long long	start;

	start = timestamp();
	while (timestamp() < start + time)
		usleep(9);
}

void	check_wait(t_rule *rule, int time)
{
	long long	i;

	i = timestamp();
	while (1)
	{
		if ((-i + timestamp()) >= time)
			break ;
		ft_usleep(1);
		pthread_mutex_lock(&(rule->dead_m));
		if ((rule->finish))
		{
			pthread_mutex_unlock(&(rule->dead_m));
			break ;
		}
		pthread_mutex_unlock(&(rule->dead_m));
	}
	return ;
}
