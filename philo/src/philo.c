/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:09:17 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/17 17:04:30 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void *philosopher(void *v_philo)
{
    t_philo *philo;
    t_rule *rules;

    philo = (t_philo *)v_philo;
    rules = philo->rule;
    if (philo->id % 2)
        usleep(15000);
    while (!(rules->finish))
    {
        pthread_mutex_lock(&rules->forks[philo->r_fork_id]); // Lock left fork
        printf("%s%lld %d has taken a fork\033[0m\n", "\033[0;32m", timestamp(), philo->id);
        pthread_mutex_lock(&rules->forks[philo->l_fork_id]); // Lock right fork
        pthread_mutex_lock(&(rules->meal_check));
        printf("%s%lld %d has taken a fork\033[0m\n", "\033[0;32m", timestamp(), philo->id);
        philo->t_last_meal = timestamp();
        printf("%s%lld %d is eating\033[0m\n", "\033[0;32m", timestamp(), philo->id);
        usleep(rules->time_eat);
        pthread_mutex_unlock(&(rules->meal_check));
        pthread_mutex_unlock(&rules->forks[philo->r_fork_id]);
        pthread_mutex_unlock(&rules->forks[philo->l_fork_id]);
        usleep(rules->time_sleep);
        printf("%s%lld %d is thinking\033[0m\n", "\033[0;32m", timestamp(), philo->id);
    }
    return NULL;
}

void verify_death(t_rule *rule)
{
    // MURIO?
    for (int i = 0; i < rule->nb_philos && !(rule->finish); i++)
    {
        pthread_mutex_lock(&(rule->meal_check));

        if (-(rule->philos[i].t_last_meal) + timestamp() > rule->time_die)
        {
            printf("%s%lld %d died\033[0m\n", "\033[0;31m", timestamp() - rule->philos[i].t_last_meal, rule->philos[i].id);
            rule->finish = 1;
            return;
        }
        pthread_mutex_unlock(&(rule->meal_check));
        usleep(100);
    }
}

static void init(t_rule *rule, char **argv)
{
    rule->nb_philos = atoi(argv[1]);
    rule->time_die = atoi(argv[2]);
    rule->time_eat = atoi(argv[3]);
    rule->time_sleep = atoi(argv[4]);
    rule->time_think = 0;
    rule->finish = 0;

}
static void ft_finish(t_rule *rule)
{
    int i;

    i = 0;
    while (i < rule->nb_philos)
        pthread_join(rule->philos[i++].thread_id, NULL);
    i = 0;
    while (i < rule->nb_philos)
        pthread_mutex_destroy(&rule->forks[i++]);
}
int main(int argc, char **argv)
{
    t_rule rule;
    int i;

    if (argc != 5)
        return (0);
    init(&rule, argv);
    i = 0;
    while (i < rule.nb_philos)
    {
        pthread_mutex_init(&rule.forks[i++], NULL);
        if (pthread_mutex_init(&(rule.meal_check), NULL))
        return (1);
    }
    i = 0;
    while (i < rule.nb_philos)
    {
        rule.philos[i].id = i;
        rule.philos[i].rule = &rule;
        rule.philos[i].l_fork_id = i;
        rule.philos[i].r_fork_id = (i + 1) % rule.nb_philos;
        rule.philos[i].t_last_meal = 0;
        pthread_create(&rule.philos[i].thread_id, NULL, philosopher, &rule.philos[i]);
        i++;
    }
    return (verify_death(&rule), ft_finish(&rule), 0);
}
