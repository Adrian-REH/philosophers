#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#define SEM_FORKS "/forks_semaphore"
#define SEM_MEALCK "/philo_mealcheck"

typedef struct s_philo
{
    pid_t id;
    int x_ate;
    int l_fork_id;
    int r_fork_id;
    struct s_rule *rule;
    long long t_last_meal;
    pthread_t death_check;
} t_philo;

typedef struct s_rule
{
    int time_eat;
    int time_sleep;
    int time_think;
    int time_die;
    int nb_philos;
    int finish;
    sem_t *forks;
    sem_t *meal_check;
    t_philo philos[200];
} t_rule;

#endif