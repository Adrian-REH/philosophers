/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:10:59 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/17 16:11:12 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_bonus.h"

void action_print(t_rule *rules, int id, char *string)
{
    sem_wait(rules->writing);
    if (!(rules->finish))
    {
        printf("%lli ", timestamp());
        printf("%i ", id + 1);
        printf("%s\n", string);
    }
    sem_post(rules->writing);
    return;
}
long long timestamp(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}
