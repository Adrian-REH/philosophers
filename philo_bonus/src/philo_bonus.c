/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:10:29 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/20 03:30:01 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void grim_reaper(t_philo *philo)
{
	sem_wait(philo->rule->meal_check);
	if ((timestamp() - (philo->t_last_meal)) >= philo->rule->time_die)
	{
		printf("SE MUERE : %d | %lld | %lld\n", philo->id + 1, timestamp() - (philo->t_last_meal), (philo->t_last_meal) - philo->rule->first_timestamp);
		action_print(philo, "is dead");
		sem_wait(philo->rule->writing);
		philo->rule->finish = 1;
		exit(1);
	}
	sem_post(philo->rule->meal_check);
}

void *monitor_philo(void *v_philo)
{
	t_philo *philo;
	t_rule *rule;
	int i;

	rule = ((philo = (t_philo *)v_philo), philo->rule);
	while (1)
	{
		grim_reaper(philo);
		if (rule->finish)
			exit(1);
		ft_usleep(1);
		i = 0;
		while (rule->nb_eat != 0 && i < rule->nb_philos &&
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

void philo_eat(t_philo *philo)
{
	ft_usleep(1);
	sem_wait(philo->rule->forks);
	action_print(philo, "has taken a fork");
	sem_wait(philo->rule->forks);
	action_print(philo, "has taken a fork");
	sem_wait(philo->rule->meal_check);
	action_print(philo, "is eating");
	philo->t_last_meal = timestamp();
	sem_post(philo->rule->meal_check);
	check_wait(philo->rule, philo->rule->time_eat);
	philo->nb_meal++;
	sem_post(philo->rule->forks);
	sem_post(philo->rule->forks);
}

void philosopher(t_philo *philo)
{
	t_rule *rule;

	rule = philo->rule;
	philo->t_last_meal = timestamp();
	pthread_create(&(philo->death_check), NULL, monitor_philo, philo);
	if (philo->id % 2)
		ft_usleep(40);
	while (!(rule->finish))
	{
		philo_eat(philo);
		action_print(philo, "is sleep");
		check_wait(rule, rule->time_sleep);
		action_print(philo, "is thinking");
		if (rule->nb_eat != 0 && philo->nb_meal >= rule->nb_eat)
			exit(0);
	}
	pthread_join(philo->death_check, NULL);
	exit(1);
}

int main(int argc, char **argv)
{
	t_rule rule;
	int i;

	if (argc != 5 && argc != 6)
		return (0);
	init_resource(&rule, argv);
	if (rule.forks == SEM_FAILED || rule.meal_check == SEM_FAILED ||
		rule.writing == SEM_FAILED)
		(perror("sem_open failed"), exit(EXIT_FAILURE));
	i = -1;
	rule.first_timestamp = timestamp();
	while (++i < rule.nb_philos)
	{
		rule.philos[i].pid = fork();
		if (rule.philos[i].pid == -1)
			exit(EXIT_FAILURE);
		else if (rule.philos[i].pid == 0)
		{
			rule.philos[i].rule = &rule;
			rule.philos[i].nb_meal = 0;
			rule.philos[i].id = i;
			philosopher(&rule.philos[i]);
		}
	}
	return (destroy_resources(&rule), 0);
}
