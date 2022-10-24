/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguilher <sguilher@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 17:59:26 by sguilher          #+#    #+#             */
/*   Updated: 2022/10/24 18:02:21 by sguilher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "./libraries/libft/libft.h"
# include <stdio.h> // printf
# include <math.h>
# include <stdbool.h>
# include <fcntl.h> // open flags

# include "mlx_utils.h"
# include "matrix.h"
# include "tuples.h"

# define AMBIENT_LIGHT 'A'
# define CAMERA 'C'
# define LIGHT 'L'
# define FILE_PLANE 'pl'
# define FILE_SPHERE 'sp'
# define FILE_CYLINDER 'cy'

enum e_status
{
	OK,
	ERROR
};

enum e_yes_no
{
	NO,
	YES
};

enum e_objects
{
	SPHERE,
	PLANE,
	CYLINDER
};

typedef union u_color
{
	int		color;
	char	rgb[4];
}				t_color;

/**
 * @param x x axis value (right)
 * @param y y axis value (up)
 * @param z z axis value (out of screen)
 * @param w type (1 = point, 0 = vector)
 */

typedef struct s_ray
{
	t_point		origin;
	t_vector	direction;
}				t_ray;

typedef struct s_light
{
	t_point		position;
	t_vector	intensity; // normalized color
	t_color		color;
	double		brightness;
}				t_light;

typedef struct s_ambient
{
	t_vector	intensity; // normalized color
	t_color		color;
	double		ratio;
}				t_ambient;

/**
 * @param color normalized color with range [0-1]
 * @param diffuse diffuse color with range [0-1]
 * @param ambient ambient color with range [0-1]
 * @param specular specular color with range [0-1]
 * @param shininess shininess value with range [10-200]
 * (at least?)(very small highlight)
 */
typedef struct s_material
{
	t_vector	normalized_color;
	double		diffuse;
	double		ambient;
	double		specular;
	double		shininess;
}				t_material;

typedef struct s_shading
{
	t_vector diffuse;
	t_vector ambient;
	t_vector specular;
}				t_shading;

typedef struct s_sphere
{
	t_point		center;
	double		radius;
	t_color		color; //
	t_matrix	transform;
	t_matrix	inverse;
	t_matrix	transpose_inverse;
	t_material	material;
}				t_sphere;

typedef struct s_plane
{
	t_point		point;
	t_vector	orientation;
	t_color		color;
}				t_plane;

typedef struct s_cylinder
{
	t_point		center;
	t_vector	orientation;
	double		radius;
	double		height;
	t_color		color;
}				t_cylinder;

typedef struct s_object
{
	void			*obj;
	t_point			xyz;
	t_color			color;
	enum e_objects	type;
	struct s_object	*next;
}				t_object;

typedef struct s_bhaskara
{
	double	a;
	double	b;
	double	c;
	double	delta;
}				t_bhaskara;

typedef struct s_camera
{
	t_point		origin;
	t_vector	orientation;
	double		fov;
	/* data */
}				t_camera;

/**
 * @param t1 the distance from the ray origin to the first intersection
 * @param t2 the distance from the ray origin to the second intersection
 * @param count the number of intersections. If there's only one intersection,
 * count = 2 and t2 = t1 and if there are no intersections, count = 0.
 */
typedef struct s_xs
{
	double	t1;
	double	t2;
	int		count;
}				t_xs;

/* if hit < 0, the intersection is behind the camera
 * if hit = 0, the intersection is at the camera
 * if hit > 0, the intersection is ahead of the camera
 * the visible intersection is the smaller positive number
 */
typedef struct s_intersection
{
	double					t;
	int						object;
	struct s_intersection	*next;
}				t_intersection;

typedef struct intersection_list
{
	t_intersection	*head;
	t_intersection	*last;
	int				total;
}				t_intersection_list;

typedef struct s_rt
{
	int			oi; //
	t_ambient	ambient;
	t_camera	camera;
	t_light		light;
	t_object	*objects;
	t_intersection_list	intersections;
}				t_rt;

// input
int				handle_input(int argc, char *filename, t_rt	*rt);
int				check_file_extension(char *file);
int				open_file(char *file);
int				read_file(int fd, char **content);
int				handle_content(char *content, t_rt	*rt);
int				handle_line(char *line, t_rt *rt);
int				handle_ambient_light(char *line, t_ambient *amb);
int				handle_camera(char *line, t_camera *cam);
int				handle_light(char *line, t_light *light);
int				handle_sphere(char *line, t_object **objs);
int				handle_plane(char *line);//, t_object *objs);
int				handle_cylinder(char *line);//, t_object *objs);

// input utils
int				total_infos(char **infos);
int				validate_double_chars(char *str);
int				validate_color_chars(char *str);
int				validate_coordinates_chars(char *str);
int				validate_nb_chars(char *str);
t_color			transform_color(char *rgb_str, int *status);
double	transform_ratio(char *ratio_str, int *status);
t_point	transform_coordinates(char *xyz_str, int *status);
t_vector	transform_orientation(char *xyz_str, int *status);
double	transform_double(char *str, int *status);
int	transform_fov(char *fov_str, int *status);

// intersections
t_intersection	*create_intersection(double t, int object);
void			add_intersection_node(t_intersection *node,
					t_intersection_list *list);

// colors
int				rgb_to_int(short int red, short int green, short int blue);
t_color			set_color(short int red, short int green, short int blue);
t_color 		transform_vector_to_color(t_vector color);
t_vector		multiply_colors(t_vector a, t_vector b);
t_vector		normalize_color(t_color color);

// rays
t_ray			set_ray(t_point origin, t_vector direction);
t_point			ray_position(t_ray ray, double distance);
t_ray			transform_ray(t_ray ray, t_matrix m);

//elements
t_object	*create_object(enum e_objects type, void *shape);
void	append_object(t_object **head, t_object *obj);
void	free_objects(t_object **head);
t_sphere		*create_sphere();
void			free_sphere(t_sphere *sphere);
void			sphere_intersection(t_ray ray, t_sphere sphere,
					t_intersection_list *list);
t_intersection	*get_hit_intersection(t_intersection_list list);
void			set_transform(t_sphere *sphere, t_matrix transform);
t_vector		sphere_normal_at(t_sphere *s, t_point p);

// close
int				close_minirt(t_mlx *mlx);

// material
t_material		set_material(void);

// light
t_vector		reflect(t_vector incident, t_vector normal);
t_light			set_point_light(t_point position, t_vector intensity);
t_vector		lighting(t_material material, t_light light, t_point point,
					t_vector normal, t_vector eye);

// utils
bool			check_double_values(double a, double b);
double	ft_atod(char *nb);

// error
int				print_error_msg(char *msg);
int				print_error_msg2(char *msg, char *str);
int				minirt_error(t_mlx *mlx, char *msg);
void			minirt_malloc_error(char *function);

// clean
void			free_array(char **arr);

#endif
