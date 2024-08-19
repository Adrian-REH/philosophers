/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:10:29 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/19 14:10:05 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void	*monitor_philo(void *v_philo)
{
	t_philo	*philo;
	t_rule	*rule;
	int		i;

	rule = ((philo = (t_philo *)v_philo), philo->rule);
	while (1)
	{
		sem_wait(rule->meal_check);
		if ((timestamp() - (philo->t_last_meal)) >= rule->time_die)
		{
			rule->finish = (action_print(rule, philo->id, "died"), 1);
			(sem_wait(rule->writing), exit(1));
		}
		i = (sem_post(rule->meal_check), 0);
		while (rule->nb_eat != 0 && i < rule->nb_philos && \
		rule->philos[i].nb_meal >= rule->nb_eat)
			i++;
		if (i == rule->nb_philos)
		{
			rule->finish = 1;
			exit(1);
		}
	}
	return (NULL);
}

void	philo_eat(t_rule *rule, t_philo *philo, int id)
{
	sem_wait(rule->forks);
	action_print(rule, id, "has taken a fork");
	sem_wait(rule->forks);
	action_print(rule, id, "has taken a fork");
	sem_wait(rule->meal_check);
	action_print(rule, id, "is eating");
	philo->t_last_meal = timestamp();
	sem_post(rule->meal_check);
	check_wait(rule, rule->time_eat);
	sem_post(rule->forks);
	sem_post(rule->forks);
}

void	philosopher(int id, t_philo *philo)
{
	t_rule	*rule;

	rule = philo->rule;
	philo->t_last_meal = timestamp();
	pthread_create(&(philo->death_check), NULL, monitor_philo, philo);
	while (!(rule->finish))
	{
		philo_eat(rule, philo, id);
		philo->nb_meal++;
		action_print(rule, id, "is sleep");
		check_wait(rule, rule->time_sleep);
		action_print(rule, id, "is thinking");
		if (rule->nb_eat != 0 && philo->nb_meal >= rule->nb_eat)
			exit(0);
	}
	pthread_join(philo->death_check, NULL);
	exit(1);
}

int	main(int argc, char **argv)
{
	t_rule	rule;
	int		i;

	if (argc != 5 && argc != 6)
		return (0);
	init_resource(&rule, argv);
	if (rule.forks == SEM_FAILED || rule.meal_check == SEM_FAILED || \
	rule.writing == SEM_FAILED)
		(perror("sem_open failed"), exit(EXIT_FAILURE));
	i = -1;
	while (++i < rule.nb_philos)
	{
		rule.philos[i].id = fork();
		if (rule.philos[i].id == -1)
			exit(EXIT_FAILURE);
		else if (rule.philos[i].id == 0)
		{
			rule.philos[i].rule = &rule;
			rule.philos[i].nb_meal = 0;
			philosopher(i, &rule.philos[i]);
		}
	}
	return (destroy_resources(&rule), 0);
}
