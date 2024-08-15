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
#define NUM_FORKS 5
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

const char *colors[] = {
    "\033[0;31m", // Rojo
    "\033[0;32m", // Verde
    "\033[0;33m", // Amarillo
    "\033[0;34m", // Azul
    "\033[0;35m", // Magenta
    "\033[0;36m", // Cian
};
long long timestamp(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}
void *verify_death(void *v_philo)
{
    t_philo *philo;
    t_rule *rule;

    philo = (t_philo *)v_philo;
    rule = philo->rule;

    // MURIO?
    sem_wait(rule->meal_check);

    if (-(philo->t_last_meal) + timestamp() > rule->time_die)
    {
        printf("%s%lld %d DIED\033[0m\n", "\033[0;31m", timestamp() - philo->t_last_meal, philo->id);
        rule->finish = 1;
        sem_post(rule->meal_check);
        exit(1);
    }
    sem_post(rule->meal_check);
    usleep(100);
    return NULL;
}
void philosopher(int id, t_philo *philo)
{
    t_rule *rule;

    rule = philo->rule;
    philo->t_last_meal = timestamp();
    pthread_create(&(philo->death_check), NULL, verify_death, philo);
    if (id % 2)
        usleep(15000);
    while (!(rule->finish))
    {
        // Tomar tenedores (esperar a que haya dos disponibles)
        sem_wait(rule->forks);
        printf("%s%lld %d has taken a fork\033[0m\n", "\033[0;32m", timestamp(), philo->id);
        sem_wait(rule->forks);
        printf("%s%lld %d has taken a fork\033[0m\n", "\033[0;32m", timestamp(), philo->id);
        sem_wait(rule->meal_check);
        printf("%s%lld %d is eating\033[0m\n", "\033[0;32m", timestamp(), philo->id);
        philo->t_last_meal = timestamp();
        sem_post(rule->meal_check);
        sleep(rule->time_eat); // Simula el tiempo de comer
        // Devolver tenedores
        sem_post(rule->forks);
        sem_post(rule->forks);

        printf("%sFilósofo %d está pensando.\n\033[0m", "\033[0;31m", id);
        long long i;

        i = timestamp();
        while (!(rule->finish))
        {
            if (-i + timestamp() >= rule->time_sleep)
                break;
            usleep(50);
        }
        printf("%s %d is thinking\033[0m\n", "\033[0;32m", philo->id);
    }
    pthread_join(philo->death_check, NULL);
    if (rule->finish)
        exit(1);
    exit(0);
}

int main(int argc, char **argv)
{
    t_rule rule;
    int status;

    if (argc != 5)
        return (0);
    rule.nb_philos = atoi(argv[1]);
    rule.time_die = atoi(argv[2]);
    rule.time_eat = atoi(argv[3]);
    rule.time_sleep = atoi(argv[4]);
    rule.time_think = 0;
    rule.finish = 0;
    sem_unlink(SEM_FORKS);
    sem_unlink(SEM_MEALCK);
    rule.forks = sem_open(SEM_FORKS, O_CREAT, 0644, rule.nb_philos);
    rule.meal_check = sem_open(SEM_MEALCK, O_CREAT, 0644, 1);
    if (rule.forks == SEM_FAILED || rule.meal_check == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rule.nb_philos; i++)
    {
        rule.philos[i].id = fork();
        if (rule.philos[i].id == -1)
            exit(EXIT_FAILURE);
        else if (rule.philos[i].id == 0)
        {
            rule.philos[i].rule = &rule;
            philosopher(i, &rule.philos[i]);
        }
        usleep(100);
    }

    for (int i = 0; i < rule.nb_philos; i++)
    {
        waitpid(rule.philos[i].id, &status, 0);
        if (status != 0)
        {
            i = -1;
            while (++i < rule.nb_philos)
                kill(rule.philos[i].id, 15);
            break;
        }
    }

    sem_close(rule.forks);
    sem_close(rule.meal_check);
    sem_unlink(SEM_FORKS);
    sem_unlink(SEM_MEALCK);

    return 0;
}
