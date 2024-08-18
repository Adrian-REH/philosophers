/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:40:30 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/18 05:43:39 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void	init_resource(t_rule *rule, char **argv)
{
	rule->nb_philos = atoi(argv[1]);
	rule->time_die = atoi(argv[2]);
	rule->time_eat = atoi(argv[3]);
	rule->time_sleep = atoi(argv[4]);
	rule->time_think = 0;
	rule->finish = 0;
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_MEALCK);
	sem_unlink(SEM_WRITING);
	rule->forks = sem_open(SEM_FORKS, O_CREAT, 0644, rule->nb_philos);
	rule->meal_check = sem_open(SEM_MEALCK, O_CREAT, 0644, 1);
	rule->writing = sem_open(SEM_WRITING, O_CREAT, 0644, 1);
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
				kill(rule->philos[i].id, 15);
			break ;
		}
		i++;
	}
	sem_close(rule->forks);
	sem_close(rule->meal_check);
	sem_close(rule->writing);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_MEALCK);
	sem_unlink(SEM_WRITING);
}
