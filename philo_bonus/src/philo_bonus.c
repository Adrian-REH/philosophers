/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:10:29 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/24 15:12:32 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

int	grim_reaper(t_philo *philo)
{
	sem_wait(philo->rule->meal_check);
	if ((timestamp() - (philo->t_last_meal)) > philo->rule->time_die)
	{
		sem_wait(philo->rule->writing);
		printf("%lli ", timestamp() - philo->rule->first_timestamp);
		printf("%i ", philo->id + 1);
		printf("%s\n", "is dead");
		return (-1);
	}
	sem_post(philo->rule->meal_check);
	return (1);
}

void	philo_eat(t_philo *philo)
{
	ft_usleep(1);
	sem_wait(philo->rule->forks);
	action_print(philo, "has taken a fork");
	sem_wait(philo->rule->forks);
	action_print(philo, "has taken a fork");
	sem_wait(philo->rule->meal_check);
	philo->t_last_meal = timestamp();
	sem_post(philo->rule->meal_check);
	action_print(philo, "is eating");
	check_wait(philo, philo->rule->time_eat);
	philo->nb_meal++;
	sem_post(philo->rule->forks);
	sem_post(philo->rule->forks);
}

void	*verify_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (grim_reaper(philo) != -1)
		usleep(1000);
	exit(1);
	return (NULL);
}

void	philosopher(t_philo *philo)
{
	t_rule		*rule;
	pthread_t	thread;

	rule = philo->rule;
	philo->t_last_meal = timestamp();
	pthread_create(&thread, NULL, verify_death, philo);
	if (philo->id % 2)
		usleep(10000);
	while (1)
	{
		philo_eat(philo);
		action_print(philo, "is sleep");
		check_wait(philo, rule->time_sleep);
		action_print(philo, "is thinking");
		if (rule->nb_eat != 0 && philo->nb_meal >= rule->nb_eat)
			exit(0);
	}
	pthread_join(thread, NULL);
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
		(printf("sem_open failed"), exit(EXIT_FAILURE));
	i = -1;
	rule.first_timestamp = timestamp();
	while (++i < rule.nb_philos)
	{
		rule.pid[i] = fork();
		if (rule.pid[i] == -1)
			exit(EXIT_FAILURE);
		else if (rule.pid[i] == 0)
		{
			rule.philos.rule = &rule;
			rule.philos.nb_meal = 0;
			rule.philos.id = i;
			philosopher(&rule.philos);
		}
	}
	return (destroy_resources(&rule), 0);
}
