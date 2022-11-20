/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elraira- <elraira-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 13:53:22 by sguilher          #+#    #+#             */
/*   Updated: 2022/11/15 111::00020 by elraira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// FOV : Horizontal field of view in degrees in range [0,180]
int	transform_fov(char *fov_str, int *status) // deixa como double ou int??
{
	int	fov;

	if (ft_strlen(fov_str) > 3)
	{
		*status = print_error_msg2("invalid FOV range: ", fov_str);
		return (0);
	}
	fov = ft_atoi(fov_str);
	if (fov < 0 || fov > 180)
		*status = print_error_msg2("invalid FOV range: ", fov_str);
	return (fov);
}

static int	validate_camera_chars(char **infos)
{
	if (validate_coordinates_chars(infos[1]) == ERROR)
		return (ERROR);
	if (validate_coordinates_chars(infos[2]) == ERROR)
		return (ERROR);
	if (validate_nb_chars(infos[3]) == ERROR)
		return (ERROR);
	return (OK);
}

void	fill_camera(t_camera *cam, char **infos, int *status)
{
	double		fov;
	t_vector	orientation;
	t_matrix	transform;

	fov = transform_fov(infos[3], status);
	*cam = set_camera(fov * M_PI / 180, WIDTH, HEIGHT);
	cam->origin = transform_coordinates(infos[1], status);
	orientation = transform_orientation(infos[2], status);
	if (*status == ERROR)
		return ;
	transform = view_transform(cam->origin, set_point(orientation.x,
		orientation.y, orientation.z), set_vector(0, 1, 0)); // calcular o up vector
	/* transform = view_transform(origin, add_point_and_vector(origin,
		orientation), set_vector(0, 1, 0)); // */
	set_camera_transform(cam, transform);
}

int	handle_camera(char *line, t_camera *cam)
{
	char		**infos;
	int			status;

	status = OK;
	infos = ft_split(line, ' ');
	if (!infos)
		return (print_error_msg("malloc error on handle_camera"));
	if (total_infos(infos) != 4)
		status = print_error_msg2("to many or few arguments for camera", line);
	else if (validate_camera_chars(infos) == ERROR)
		status = ERROR;
	else
		fill_camera(cam, infos, &status);
	free_array(infos);
	return (status);
}
