/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 21:16:19 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/24 21:20:38 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int	taken_fork(t_rule *rules, t_philo *philo)
{
	if (philo->id % 2 && rules->nb_philos % 2 == 0)
	{
		pthread_mutex_lock(&(rules->forks[philo->r_fork_id]));
		action_print(rules, philo->id, "has taken a fork");
		if (rules->nb_philos == 1)
			return (pthread_mutex_unlock(& \
			(rules->forks[philo->r_fork_id])), -1);
		pthread_mutex_lock(&(rules->forks[philo->l_fork_id]));
		action_print(rules, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&(rules->forks[philo->l_fork_id]));
		action_print(rules, philo->id, "has taken a fork");
		if (rules->nb_philos == 1)
			return (pthread_mutex_unlock(& \
			(rules->forks[philo->l_fork_id])), -1);
		pthread_mutex_lock(&(rules->forks[philo->r_fork_id]));
		action_print(rules, philo->id, "has taken a fork");
	}
	return (0);
}

void	release_fork(t_rule *rules, t_philo *philo)
{
	if (philo->id % 2 && rules->nb_philos % 2 == 0)
	{
		pthread_mutex_unlock(&(rules->forks[philo->l_fork_id]));
		action_print(rules, philo->id, "has release a fork");
		pthread_mutex_unlock(&(rules->forks[philo->r_fork_id]));
		action_print(rules, philo->id, "has release a fork");
	}
	else
	{
		pthread_mutex_unlock(&(rules->forks[philo->r_fork_id]));
		action_print(rules, philo->id, "has release a fork");
		pthread_mutex_unlock(&(rules->forks[philo->l_fork_id]));
		action_print(rules, philo->id, "has release a fork");
	}
}

int	philo_eat(t_rule *rules, t_philo *philo)
{
	if (taken_fork(rules, philo) == -1)
		return (-1);
	pthread_mutex_lock(&(rules->meal_check));
	philo->t_last_meal = timestamp();
	action_print(rules, philo->id, "is eating");
	pthread_mutex_unlock(&(rules->meal_check));
	check_wait(rules, rules->time_eat);
	philo->nb_eat++;
	release_fork(rules, philo);
	return (0);
}
