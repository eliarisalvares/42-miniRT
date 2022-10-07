/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguilher <sguilher@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:24:11 by elraira-          #+#    #+#             */
/*   Updated: 2022/10/07 11:27:54 by sguilher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_sphere	*create_sphere(t_point center, double radius, t_color color)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		minirt_malloc_error("create_sphere");
	sphere->center = center;
	sphere->radius = radius;
	sphere->color = color;
	return (sphere);
}

/**
 * @brief
 *
 * @param ray is the ray that will intersect the sphere.
 * @param sphere is the sphere that will be intersected by the ray.
 * @param projected_center sphere center projection into the ray direction.
 * @param projected_vector vector from ray origin to sphere's projected center.
 * @return t_intersection the t_interction struct.
 */
t_intersection	*sphere_intersection(t_ray ray, t_sphere sphere)
{
	t_intersection	*intersection;
	double			projected_center;
	t_point			projected_vector;
	double			x_sphere;
	double			y_sphere;

	projected_center = scalar_product(subtract_points(sphere.center,
		ray.origin), ray.direction);
	projected_vector = ray_position(ray, projected_center);
	y_sphere = vector_length(subtract_points(sphere.center,
		projected_vector));
	if (y_sphere > sphere.radius)
		return (NULL);
	x_sphere = sqrt(pow(sphere.radius, 2) - pow(y_sphere, 2));
	intersection = malloc(sizeof(t_intersection));
	if (!intersection)
		minirt_malloc_error("sphere_intersection");
	intersection->t1 = projected_center - x_sphere;
	intersection->t2 = projected_center + x_sphere;
	intersection->hit = get_hit(intersection->t1, intersection->t2);
	intersection->type = SPHERE;
	return (intersection);
}

double	get_hit(double t1, double t2)
{
	if (t1 < 0 && t2 < 0)
		return (-1);
	if (t1 < 0)
		return (t2);
	if (t2 < 0)
		return (t1);
	if (t1 < t2)
		return (t1);
	return (t2);
}

// The hit will be the closest intersection point with a t greater than 0.

