/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:40:30 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/24 21:14:22 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"
#include <signal.h>

int	ft_nbr(char **argv)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (argv[i])
	{
		j = -1;
		while (argv[i][++j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (1);
		}
		i++;
	}
	return (0);
}

void	init_resource(t_rule *rule, char **argv)
{
	if (ft_nbr(argv))
		(printf("NO es un argumento valido"), exit(0));
	rule->time_die = ((rule->nb_philos = ft_atoi(argv[1])), ft_atoi(argv[2]));
	rule->time_sleep = ((rule->time_eat = ft_atoi(argv[3])), ft_atoi(argv[4]));
	if (rule->nb_philos <= 0 || rule->nb_philos > 200 || \
	rule->time_die < 60 || rule->time_sleep < 60 || rule->time_eat < 60)
		(printf("NO es un argumento valido\n"), exit(0));
	if (argv[5])
	{
		rule->nb_eat = ft_atoi(argv[5]);
		if (rule->nb_eat <= 0)
			(printf("NO es un argumento valido\n"), exit(1));
	}
	else
		rule->nb_eat = 0;
	(sem_unlink(SEM_FORKS), sem_unlink(SEM_MEALCK));
	(sem_unlink(SEM_WRITING));
	rule->forks = sem_open(SEM_FORKS, O_CREAT, 0644, rule->nb_philos);
	rule->meal_check = sem_open(SEM_MEALCK, O_CREAT, 0644, 1);
	rule->writing = sem_open(SEM_WRITING, O_CREAT, 0644, 1);
	rule->pid = malloc(sizeof(pid_t) * (rule->nb_philos + 1));
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
				kill(rule->pid[i], 15);
			break ;
		}
		i++;
	}
	sem_close(rule->forks);
	sem_close(rule->meal_check);
	sem_close(rule->writing);
	sem_unlink(SEM_FORKS);
	(sem_unlink(SEM_MEALCK), sem_unlink(SEM_WRITING));
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
