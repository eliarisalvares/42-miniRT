/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguilher <sguilher@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:24:11 by elraira-          #+#    #+#             */
/*   Updated: 2022/10/11 19:54:220 by sguilher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/**
 * @brief Will create a new sphere by allocating memory and setting the given
 * values to the struct.
 *
 * @param color The color of the sphere.
 * @return t_sphere* The created sphere.
 */
t_sphere	*create_sphere(t_color color)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		minirt_malloc_error("create_sphere");
	sphere->center = set_point(0, 0, 0);
	sphere->radius = 1.0;
	sphere->color = color;
	sphere->transform = identity_matrix(4);
	return (sphere);
}

/**
 * @brief Gets the intersections between a ray and a sphere.
 *
 * @param ray is the ray that will intersect the sphere.
 * @param sphere is the sphere that will be intersected by the ray.
 * @param projected_center sphere center projection into the ray direction.
 * @param projected_vector vector from ray origin to sphere's projected center.
 * @return t_xs a struct containing all intersections of the ray with the
 * sphere. If the ray doesn't intersect with any point of the sphere,the count
 * variable of t_xs struct will be 0. If the ray is tangent to the sphere, the
 * count variable will be 2 andt1 and t2 willhave the same value.
 */
void	sphere_intersection(t_ray ray, t_sphere s, t_intersection_list *list)
{
	double			projected_center;
	t_point			projected_vector;
	double			x_sphere;
	double			y_sphere;
	t_ray			transformed_ray;
	t_matrix		inverse;

	inverse = inverse_matrix(s.transform);
	transformed_ray = transform_ray(ray, inverse); // vai gerar um leak que teremos que tratar no futuro
	//transformed_ray = transform_ray(ray, inverse_matrix(s.transform)); // vai gerar um leak que teremos que tratar no futuro
	free_matrix(inverse);
	projected_center = scalar_product(subtract_points(s.center,
				transformed_ray.origin), transformed_ray.direction);
	projected_vector = ray_position(transformed_ray, projected_center);
	y_sphere = vector_length(subtract_points(s.center, projected_vector));
	if (y_sphere > s.radius)
		return ;
	else
	{
		x_sphere = sqrt(pow(s.radius, 2) - pow(y_sphere, 2));
		add_intersection_node(create_intersection(projected_center
			- x_sphere, SPHERE), list);
		add_intersection_node(create_intersection(projected_center
			+ x_sphere, SPHERE), list);
	}
}

double	check_hit_value(double hit, double t)
{
	if (hit < 0 && t < 0)
		return (-1);
	if (hit < 0)
		return (t);
	if (t < 0)
		return (hit);
	if (hit < t)
		return (hit);
	return (t);
}

/**
 * @brief The hit will be the closest intersection point with a t
 * greater than 0.
 *
 * @param t1 is the first intersection point.
 * @param t2 is the second intersection point.
 * @return double the hit.
 */
t_intersection	*get_hit_intersection(t_intersection_list list)
{
	t_intersection	*aux;
	t_intersection	*hit_intersection;
	double			hit;

	aux = list.head;
	hit = -1;
	hit_intersection = NULL;
	while (aux)
	{
		hit = check_hit_value(hit, aux->t);
		if (hit == aux->t)
			hit_intersection = aux;
		aux = aux->next;
	}
	return (hit_intersection);
}
