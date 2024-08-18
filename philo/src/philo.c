/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:09:17 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/18 05:30:27 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void	philo_eat(t_rule *rules, t_philo *philo)
{
	pthread_mutex_lock(&(rules->forks[philo->r_fork_id]));
	action_print(rules, philo->id, "has taken a fork");
	pthread_mutex_lock(&(rules->forks[philo->l_fork_id]));
	action_print(rules, philo->id, "has taken a fork");
	pthread_mutex_lock(&(rules->meal_check));
	philo->t_last_meal = timestamp();
	action_print(rules, philo->id, "is eating");
	pthread_mutex_unlock(&(rules->meal_check));
	check_wait(rules, rules->time_eat);
	philo->nb_eat++;
	pthread_mutex_unlock(&(rules->forks[philo->r_fork_id]));
	pthread_mutex_unlock(&(rules->forks[philo->l_fork_id]));
}

void	*philosopher(void *v_philo)
{
	t_philo		*philo;
	t_rule		*rules;

	philo = (t_philo *)v_philo;
	rules = philo->rule;
	if (philo->id % 2)
		usleep(15000);
	while (!(rules->finish))
	{
		philo_eat(rules, philo);
		action_print(rules, philo->id, "is sleep");
		check_wait(rules, rules->time_sleep);
		action_print(rules, philo->id, "is thinking");
		if (rules->nb_eat != 0 && philo->nb_eat >= rules->nb_eat)
			break ;
	}
	return (NULL);
}

void	monitor_philos(t_rule *rule)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < rule->nb_philos && !(rule->finish))
		{
			pthread_mutex_lock(&(rule->meal_check));
			if ((timestamp() - (rule->philos[i].t_last_meal)) >= rule->time_die)
				rule->finish = \
				(action_print(rule, rule->philos[i].id, "is dead"), 1);
			pthread_mutex_unlock(&(rule->meal_check));
		}
		if (rule->finish)
			break ;
		i = 0;
		while (rule->nb_eat != 0 && i < rule->nb_philos && \
		rule->philos[i].nb_eat >= rule->nb_eat)
			i++;
		if (i == rule->nb_philos)
		{
			rule->finish = 1;
			break ;
		}
	}
}

int	main(int argc, char **argv)
{
	t_rule	r;
	int		i;

	if (argc != 5 && argc != 6)
		return (0);
	init_resource(&r, argv);
	i = -1;
	while (++i < r.nb_philos)
	{
		init_philo(&r, i);
		pthread_create(&r.philos[i].thread_id, NULL, philosopher, &r.philos[i]);
	}
	return (monitor_philos(&r), destroy_resources(&r), 0);
}
