/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:09:17 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/27 17:20:46 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void	*philosopher(void *v_philo)
{
	t_philo	*philo;
	t_rule	*rules;

	philo = (t_philo *)v_philo;
	rules = philo->rule;
	pthread_mutex_lock(&(rules->init_philos));
	pthread_mutex_unlock(&(rules->init_philos));
	if (philo->id % 2)
		ft_usleep(50);
	pthread_mutex_lock(&(rules->died));
	while (!(rules->finish))
	{
		pthread_mutex_unlock(&(rules->died));
		if (philo_eat(rules, philo) == -1)
			break ;
		action_print(rules, philo->id, "is sleep");
		check_wait(rules, rules->time_sleep);
		action_print(rules, philo->id, "is thinking");
		if (rules->nb_eat != 0 && philo->nb_eat >= rules->nb_eat)
			break ;
		pthread_mutex_lock(&(rules->died));
	}
	pthread_mutex_unlock(&(rules->died));
	return (NULL);
}

int	grim_reaper(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->nb_philos && !(rule->finish))
	{
		pthread_mutex_lock(&(rule->meal_check));
		if ((timestamp() - (rule->philos[i].t_last_meal)) >= rule->time_die)
		{
			pthread_mutex_lock(&(rule->writing));
			printf("%lli ", timestamp() - rule->first_timestamp);
			printf("%i ", rule->philos->id + 1);
			printf("%s\n", "is dead");
			pthread_mutex_lock(&(rule->died));
			rule->finish = 1;
			pthread_mutex_unlock(&(rule->meal_check));
			pthread_mutex_unlock(&(rule->writing));
			pthread_mutex_unlock(&(rule->died));
			return (-1);
		}
		pthread_mutex_unlock(&(rule->meal_check));
	}
	return (0);
}

void	monitor_philos(t_rule *rule)
{
	int	i;

	while (1)
	{
		if (grim_reaper(rule) == -1)
			return ;
		i = 0;
		while (rule->nb_eat != 0 && i < rule->nb_philos && \
		rule->philos[i].nb_eat >= rule->nb_eat)
			i++;
		if (i == rule->nb_philos)
		{
			pthread_mutex_lock(&(rule->died));
			rule->finish = 1;
			pthread_mutex_unlock(&(rule->died));
			break ;
		}
		if (rule->finish)
			break ;
	}
}

int	main(int argc, char **argv)
{
	t_rule	r;
	int		i;

	if (argc != 5 && argc != 6)
		return (0);
	if (init_resource(&r, argv) == -1)
		return (0);
	i = -1;
	pthread_mutex_lock(&(r.init_philos));
	while (++i < r.nb_philos)
	{
		init_philo(&r, i);
		pthread_create(&r.philos[i].thread_id, NULL, philosopher, &r.philos[i]);
	}
	r.first_timestamp = timestamp();
	pthread_mutex_unlock(&(r.init_philos));
	return (monitor_philos(&r), destroy_resources(&r), 0);
}
