/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_tests_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguilher <sguilher@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 15:42:43 by sguilher          #+#    #+#             */
/*   Updated: 2022/10/15 14:42:25 by sguilher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unit_tests.h"

bool	check_equal_points(t_point a, t_point b)
{
	return (
		check_double_values(a.x, b.x)
		&& check_double_values(a.y, b.y)
		&& check_double_values(a.z, b.z)
		&& check_double_values(a.w, b.w)
	);
}

bool	check_equal_vectors(t_vector a, t_vector b)
{
	return (
		check_double_values(a.x, b.x)
		&& check_double_values(a.y, b.y)
		&& check_double_values(a.z, b.z)
		&& check_double_values(a.w, b.w)
	);
}
