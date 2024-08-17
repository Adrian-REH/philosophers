/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:09:28 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/17 19:38:10 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct s_philo
{
    int id;
    int x_ate;
    int l_fork_id;
    int r_fork_id;
    struct s_rule *rule;
    long long t_last_meal;
    pthread_t thread_id;
} t_philo;

typedef struct s_rule
{
    int time_eat;
    int time_sleep;
    int time_think;
    int time_die;
    int nb_philos;
    int finish;
    pthread_mutex_t forks[200];
    pthread_mutex_t meal_check;
    t_philo philos[200];
} t_rule;

long long timestamp(void);

#endif