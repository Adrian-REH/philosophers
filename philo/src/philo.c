
#include "../header/philo.h"

long long timestamp(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}
void *philosopher(void *p)
{
    t_philo *philo = (t_philo *)p;
    t_rule *rules;

    rules = philo->rule;
    if (philo->id % 2)
        usleep(15000);
    while (!(rules->finish))
    {
        long long time = timestamp();
        // Acquire both forks (mutexes)
        pthread_mutex_lock(&rules->forks[philo->r_fork_id]); // Lock left fork
        printf("%s%lld %d has taken a fork\033[0m\n", "\033[0;32m", timestamp() - time, philo->id);
        pthread_mutex_lock(&rules->forks[philo->l_fork_id]); // Lock right fork
        pthread_mutex_lock(&(rules->meal_check));

        printf("%s%lld %d has taken a fork\033[0m\n", "\033[0;32m", timestamp() - time, philo->id);
        philo->t_last_meal = timestamp();
        // Eat
        printf("%s%lld %d is eating\033[0m\n", "\033[0;32m", timestamp() - time, philo->id);
        usleep(rules->time_eat); // Simulate eating
        pthread_mutex_unlock(&(rules->meal_check));
        // Release both forks (mutexes)
        pthread_mutex_unlock(&rules->forks[philo->r_fork_id]); // Unlock right fork
        pthread_mutex_unlock(&rules->forks[philo->l_fork_id]); // Unlock left fork
                                                               // Think
        printf("%s%lld %d is thinking\033[0m\n", "\033[0;32m", time - time, philo->id);
    }
    return NULL;
}

void verify_death(t_rule *rule)
{
    // MURIO?
    for (int i = 0; i < rule->nb_philos && !rule->finish; i++)
    {
        pthread_mutex_lock(&(rule->meal_check));

        if (-(rule->philos[i].t_last_meal) + timestamp() > rule->time_die)
        {
            printf("%s%lld %d died\033[0m\n", "\033[0;31m", timestamp() - rule->philos[i].t_last_meal, rule->philos[i].id);
            rule->finish = 1;
            pthread_mutex_unlock(&(rule->meal_check));
            break;
        }
        pthread_mutex_unlock(&(rule->meal_check));
        usleep(100);
    }
}
int main(int argc, char **argv)
{
    t_rule rule;

    if (argc != 5)
        return (0);

    rule.nb_philos = atoi(argv[1]);
    rule.time_die = atoi(argv[2]);
    rule.time_eat = atoi(argv[3]);
    rule.time_sleep = atoi(argv[4]);
    rule.time_think = 0;
    rule.finish = 0;
    // Initialize mutexes
    for (int i = 0; i < rule.nb_philos; i++)
        pthread_mutex_init(&rule.forks[i], NULL);
    if (pthread_mutex_init(&(rule.meal_check), NULL))
        return (1);

    // Create philosopher threads
    for (int i = 0; i < rule.nb_philos; i++)
    {
        rule.philos[i].id = i;
        rule.philos[i].rule = &rule;
        rule.philos[i].l_fork_id = i;
        rule.philos[i].r_fork_id = (i + 1) % rule.nb_philos;
        rule.philos[i].t_last_meal = 0;
        pthread_create(&rule.philos[i].thread_id, NULL, philosopher, &rule.philos[i]);
    }

    verify_death(&rule);
    // EXIT
    //  Join threads (this will block indefinitely)
    for (int i = 0; i < rule.nb_philos; i++)
        pthread_join(rule.philos[i].thread_id, NULL);

    // Destroy mutexes
    for (int i = 0; i < rule.nb_philos; i++)
        pthread_mutex_destroy(&rule.forks[i]);

    return 0;
}
