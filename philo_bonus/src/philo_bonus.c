/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:10:29 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/17 19:40:09 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void *verify_death(void *v_philo)
{
    t_philo *philo;
    t_rule *rule;

    philo = (t_philo *)v_philo;
    rule = philo->rule;
    while (rule->finish == 0)
    {
        sem_wait(rule->meal_check);
        if ((timestamp() - (philo->t_last_meal)) >= rule->time_die)
        {
            action_print(rule, philo->id, "died");
            rule->finish = 1;
            sem_wait(rule->writing);
            exit(1);
        }
        sem_post(rule->meal_check);
    }
    return NULL;
}

void philosopher(int id, t_philo *philo)
{
    t_rule *rule;

    rule = philo->rule;
    philo->t_last_meal = timestamp();
    pthread_create(&(philo->death_check), NULL, verify_death, philo);
    while (!(rule->finish))
    {
        sem_wait(rule->forks);
        action_print(rule, id, "has taken a fork");
        sem_wait(rule->forks);
        action_print(rule, id, "has taken a fork");
        sem_wait(rule->meal_check);
        action_print(rule, id, "is eating");
        philo->t_last_meal = timestamp();
        sem_post(rule->meal_check);
        long long i;
        i = timestamp();
        while (!(rule->finish))
        {
            if ((-i + timestamp()) >= rule->time_eat)
                break;
        }
        sem_post(rule->forks);
        sem_post(rule->forks);
        action_print(rule, id, "is sleep");
        i = timestamp();
        while (!(rule->finish))
        {
            if ((-i + timestamp()) >= rule->time_sleep)
                break;
        }
        action_print(rule, id, "is thinking");
    }
    pthread_join(philo->death_check, NULL);
    exit(1);
}

void init(t_rule *rule, char **argv)
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

void ft_finish(t_rule *rule)
{
    int status;
    int i;

    i = 0;
    while (i < rule->nb_philos)
    {
        waitpid(-1, &status, 0);
        if (status != 0)
        {
            i = -1;
            while (++i < rule->nb_philos)
                kill(rule->philos[i].id, 15);
            break;
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

int main(int argc, char **argv)
{
    t_rule rule;
    int i;

    if (argc != 5)
        return (0);
    init(&rule, argv);
    if (rule.forks == SEM_FAILED || rule.meal_check == SEM_FAILED)
        (perror("sem_open failed"), exit(EXIT_FAILURE));
    i = 0;
    while (i < rule.nb_philos)
    {
        rule.philos[i].id = fork();
        if (rule.philos[i].id == -1)
            exit(EXIT_FAILURE);
        else if (rule.philos[i].id == 0)
        {
            rule.philos[i].rule = &rule;
            philosopher(i, &rule.philos[i]);
        }
        i++;
    }
    ft_finish(&rule);
    return 0;
}
