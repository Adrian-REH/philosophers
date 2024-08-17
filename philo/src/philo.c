/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:09:17 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/17 19:38:54 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void *philosopher(void *v_philo)
{
    t_philo *philo;
    t_rule *rules;
    long long i;

    philo = (t_philo *)v_philo;
    rules = philo->rule;
    if (philo->id % 2)
        usleep(15000);
    while (!(rules->finish))
    {
        pthread_mutex_lock(&(rules->forks[philo->r_fork_id]));
        action_print(rules, philo->id, "has taken a fork");
        pthread_mutex_lock(&(rules->forks[philo->l_fork_id]));
        action_print(rules, philo->id, "has taken a fork");
        pthread_mutex_lock(&(rules->meal_check));
        philo->t_last_meal = timestamp();
        action_print(rules, philo->id, "is eating");
        pthread_mutex_unlock(&(rules->meal_check));
        i = timestamp();
        while (!(rules->finish))
        {
            if ((-i + timestamp()) >= rules->time_eat)
                break;
        }
        philo->nb_eat++;
        pthread_mutex_unlock(&(rules->forks[philo->l_fork_id]));
        pthread_mutex_unlock(&(rules->forks[philo->r_fork_id]));
        action_print(rules, philo->id, "is sleep");
        i = timestamp();
        while (!(rules->finish))
        {
            if ((-i + timestamp()) >= rules->time_sleep)
                break;
        }
        action_print(rules, philo->id, "is thinking");
    }
    return NULL;
}

void verify_death(t_rule *rule)
{
    // MURIO?
    int i;
    while (1)
    {
        i = -1;
        while (++i < rule->nb_philos && !(rule->finish))
        {
            pthread_mutex_lock(&(rule->meal_check));
            if ((timestamp() - (rule->philos[i].t_last_meal)) >= rule->time_die)
            {
                action_print(rule, rule->philos[i].id, "is dead");
                rule->finish = 1;
            }
            pthread_mutex_unlock(&(rule->meal_check));
        }
        if (rule->finish)
            break;
        while (i < rule->nb_philos && rule->philos[i].nb_eat >= rule->nb_eat)
            i++;
    }
}

static void init(t_rule *rule, char **argv)
{
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
}
static void ft_finish(t_rule *rule)
{
    int i;

    i = -1;
    while (++i < rule->nb_philos)
        pthread_join(rule->philos[i].thread_id, NULL);
    i = -1;
    while (++i < rule->nb_philos)
        pthread_mutex_destroy(&rule->forks[i]);
    pthread_mutex_destroy(&(rule->writing));
    pthread_mutex_destroy(&(rule->meal_check));
}

int main(int argc, char **argv)
{
    t_rule rule;
    int i;

    if (argc != 5 && argc != 6)
        return (0);
    init(&rule, argv);
    i = 0;
    while (i < rule.nb_philos)
    {
        pthread_mutex_init(&rule.forks[i++], NULL);
    }
    if (pthread_mutex_init(&(rule.meal_check), NULL) == -1)
        return (1);
    if (pthread_mutex_init(&(rule.writing), NULL) == -1)
        return (1);
    i = 0;
    while (i < rule.nb_philos)
    {
        rule.philos[i].id = i;
        rule.philos[i].rule = &rule;
        rule.philos[i].nb_eat = 0;
        rule.philos[i].l_fork_id = i;
        rule.philos[i].r_fork_id = (i + 1) % rule.nb_philos;
        rule.philos[i].t_last_meal = timestamp();
        pthread_create(&rule.philos[i].thread_id, NULL, philosopher, &rule.philos[i]);
        i++;
    }
    return (verify_death(&rule), ft_finish(&rule), 0);
}
