/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resource_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 05:18:13 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/24 21:20:56 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int	ft_pthread_initializer(t_rule *rule)
{
	int	i;

	i = 0;
	while (i < rule->nb_philos)
		pthread_mutex_init(&rule->forks[i++], NULL);
	if (pthread_mutex_init(&(rule->meal_check), NULL) == -1 || \
		pthread_mutex_init(&(rule->writing), NULL) == -1 || \
		pthread_mutex_init(&(rule->died), NULL) == -1 || \
		pthread_mutex_init(&(rule->init_philos), NULL) == -1)
		return (printf("Error init mutex\n"), (-1));
	return (0);
}

int	init_resource(t_rule *rule, char **argv)
{
	if (ft_nbr(argv))
		(printf("NO es un argumento valido"), exit(0));
	rule->time_die = ((rule->nb_philos = ft_atoi(argv[1])), ft_atoi(argv[2]));
	rule->time_sleep = ((rule->time_eat = ft_atoi(argv[3])), ft_atoi(argv[4]));
	if (rule->nb_philos <= 0 || rule->nb_philos > 200 || \
	rule->time_die < 60 || rule->time_sleep < 60 || rule->time_eat < 60)
		return (printf("NO es un argumento valido\n"), (-1));
	if (argv[5])
	{
		rule->nb_eat = ft_atoi(argv[5]);
		if (rule->nb_eat <= 0)
			return (printf("NO es un argumento valido\n"), (-1));
	}
	else
		rule->nb_eat = 0;
	rule->finish = (0);
	rule->forks = malloc((rule->nb_philos + 1) * sizeof(pthread_mutex_t));
	rule->philos = (t_philo *)malloc((rule->nb_philos + 1) * sizeof(t_philo));
	if (ft_pthread_initializer(rule) == -1)
		return (-1);
	return (0);
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
	pthread_mutex_destroy(&(rule->died));
	pthread_mutex_destroy(&(rule->init_philos));
	free(rule->forks);
	free(rule->philos);
}
