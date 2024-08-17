/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adherrer <adherrer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:09:13 by adherrer          #+#    #+#             */
/*   Updated: 2024/08/17 16:09:42 by adherrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

long long timestamp(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void action_print(t_rule *rules, int id, char *string)
{
    pthread_mutex_lock(&(rules->writing));
    if (!(rules->finish))
    {
        printf("%lli ", timestamp());
        printf("%i ", id + 1);
        printf("%s\n", string);
    }
    pthread_mutex_unlock(&(rules->writing));
    return;
}