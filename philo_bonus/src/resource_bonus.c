/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:40:30 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/20 03:21:24 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void	init_resource(t_rule *rule, char **argv)
{
	rule->time_die = ((rule->nb_philos = ft_atoi(argv[1])), ft_atoi(argv[2]));
	rule->time_sleep = ((rule->time_eat = ft_atoi(argv[3])), ft_atoi(argv[4]));
	rule->finish = ((rule->time_think = 0), 0);
	if (rule->nb_philos <= 0 || rule->nb_philos > 200
		|| rule->time_die < 60 || rule->time_sleep < 60 || rule->time_eat < 60)
		(printf("Error: Arg invalid\n"), exit(0));
	if (argv[5])
	{
		rule->nb_eat = ft_atoi(argv[5]);
		if (rule->nb_eat <= 0)
			(printf("Error Argumentos\n"), exit(1));
	}
	else
		rule->nb_eat = 0;
	rule->philos = (t_philo *)malloc((rule->nb_philos + 1) * sizeof(t_philo));
	(sem_unlink(SEM_FORKS), sem_unlink(SEM_MEALCK));
	(sem_unlink(SEM_WRITING), sem_unlink(SEM_DEAD), sem_unlink(SEM_INIT));
	rule->forks = sem_open(SEM_FORKS, O_CREAT, 0644, rule->nb_philos);
	rule->meal_check = sem_open(SEM_MEALCK, O_CREAT, 0644, 1);
	rule->writing = sem_open(SEM_WRITING, O_CREAT, 0644, 1);
	rule->dead = sem_open(SEM_DEAD, O_CREAT, 0644, 1);
	rule->init = sem_open(SEM_INIT, O_CREAT, 0644, 1);
}

void	destroy_resources(t_rule *rule)
{
	int	status;
	int	i;

	i = 0;
	while (i < rule->nb_philos)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = -1;
			while (++i < rule->nb_philos)
				kill(rule->philos[i].pid, 15);
			break ;
		}
		i++;
	}
	sem_close(rule->forks);
	sem_close(rule->meal_check);
	sem_close(rule->writing);
	sem_close(rule->dead);
	sem_close(rule->init);
	sem_unlink(SEM_FORKS);
	(sem_unlink(SEM_MEALCK), sem_unlink(SEM_WRITING));
	(sem_unlink(SEM_INIT), sem_unlink(SEM_DEAD));
	free(rule->philos);
}

int	ft_atoi(const char *str)
{
	int	sign;
	int	num;

	num = 0;
	sign = 1;
	while ((*str <= 13 && *str >= 9) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		num = num * 10 - 48 + *str++;
	return (num * sign);
}
