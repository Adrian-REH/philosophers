/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:10:41 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/24 00:30:22 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <fcntl.h>
# include <time.h>
# include <sys/time.h>
# include <pthread.h>
# define SEM_FORKS "/forks_semaphore"
# define SEM_MEALCK "/philo_mealcheck"
# define SEM_WRITING "/philo_writing"
# define SEM_INIT "/philo_init"

typedef struct s_philo
{
	int				nb_meal;
	struct s_rule	*rule;
	long long		t_last_meal;
	int				id;
}					t_philo;

typedef struct s_rule
{
	int			time_eat;
	int			time_sleep;
	int			time_die;
	int			nb_philos;
	int			nb_eat;
	pid_t		*pid;
	long long	first_timestamp;
	sem_t		*forks;
	sem_t		*meal_check;
	sem_t		*writing;
	t_philo		philos;
}				t_rule;

long long	timestamp(void);
void		action_print(t_philo *philo, char *string);
void		init_resource(t_rule *rule, char **argv);
void		destroy_resources(t_rule *rule);
int			ft_atoi(const char *str);
void		ft_usleep(long long time);
void		check_wait(t_philo *philo, int time);
int			grim_reaper(t_philo *philo);

#endif