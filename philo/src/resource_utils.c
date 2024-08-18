/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:18:13 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/18 05:43:54 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void	init_resource(t_rule *rule, char **argv)
{
	int		i;

	rule->nb_philos = atoi(argv[1]);
	rule->time_die = atoi(argv[2]);
	rule->time_eat = atoi(argv[3]);
	rule->time_sleep = atoi(argv[4]);
	if (argv[5])
	{
		rule->nb_eat = atoi(argv[5]);
		if (rule->nb_eat <= 0)
			(perror("Error Argumentos"), exit(1));
	}
	else
		rule->nb_eat = 0;
	rule->time_think = 0;
	rule->finish = 0;
	i = 0;
	while (i < rule->nb_philos)
		pthread_mutex_init(&rule->forks[i++], NULL);
	if (pthread_mutex_init(&(rule->meal_check), NULL) == -1 || \
	pthread_mutex_init(&(rule->writing), NULL) == -1)
		exit (1);
}

void	init_philo(t_rule *rule, int i)
{
	rule->philos[i].id = i;
	rule->philos[i].rule = rule;
	rule->philos[i].nb_eat = 0;
	rule->philos[i].l_fork_id = i;
	rule->philos[i].r_fork_id = (i + 1) % rule->nb_philos;
	rule->philos[i].t_last_meal = timestamp();
}

void	destroy_resources(t_rule *rule)
{
	int	i;

	i = -1;
	while (++i < rule->nb_philos)
		pthread_join(rule->philos[i].thread_id, NULL);
	i = -1;
	while (++i < rule->nb_philos)
		pthread_mutex_destroy(&rule->forks[i]);
	pthread_mutex_destroy(&(rule->writing));
	pthread_mutex_destroy(&(rule->meal_check));
}
