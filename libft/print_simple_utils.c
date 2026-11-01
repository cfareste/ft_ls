/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_simple_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfidalgo <cfidalgo@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 13:06:57 by cfidalgo          #+#    #+#             */
/*   Updated: 2024/02/17 01:40:57 by cfidalgo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*ft_ltoa(long n)
{
	char			*digits;
	char			*num;
	unsigned long	magnitude;
	size_t			len;

	if (n >= 0)
		return (ft_uitoa((unsigned long)n));
	magnitude = (unsigned long)(-(n + 1)) + 1;
	digits = ft_uitoa(magnitude);
	if (!digits)
		return (0);
	len = ft_strlen(digits);
	num = malloc(len + 2);
	if (!num)
	{
		free(digits);
		return (0);
	}
	num[0] = '-';
	ft_memcpy(num + 1, digits, len + 1);
	free(digits);
	return (num);
}

int	print_character(char c, t_ft_printf_flags *flags)
{
	int	byte_to_print;
	int	bytes_written;

	byte_to_print = ' ';
	if (flags->minus)
	{
		if (print_raw_char(flags->fd, c) == -1
			|| print_width(flags->fd, flags->width - 1, byte_to_print) == -1)
			return (-1);
	}
	else
	{
		if (flags->zero)
			byte_to_print = '0';
		if (print_width(flags->fd, flags->width - 1, byte_to_print) == -1
			|| print_raw_char(flags->fd, c) == -1)
			return (-1);
	}
	bytes_written = flags->width;
	if (!bytes_written)
		return (1);
	return (bytes_written);
}

static int	print_sig_number_string(char *num_str, t_ft_printf_flags *flags, long n)
{
	int	byt_wri;
	int	wdt_wri;

	if (flags->minus)
	{
		if (check_for_signs_flags(n, flags) == -1)
			return (-1);
		byt_wri = print_precised_number(num_str, flags, n);
		if (byt_wri == -1)
			return (-1);
		wdt_wri = print_width(flags->fd, calc_total_width(num_str, flags, byt_wri), ' ');
		if (wdt_wri == -1)
			return (-1);
		return (byt_wri + wdt_wri);
	}
	wdt_wri = 0;
	if ((!flags->zero && flags->width)
		|| (flags->zero && flags->precision != -2 && flags->width))
		wdt_wri = print_width(flags->fd, calc_total_width(num_str, flags, 0), ' ');
	if (wdt_wri == -1 || check_for_signs_flags(n, flags) == -1)
		return (-1);
	byt_wri = print_precised_number(num_str, flags, n);
	if (byt_wri == -1)
		return (-1);
	return (byt_wri + wdt_wri);
}

static int	print_uns_nbr_string(char *num_str, t_ft_printf_flags *flags, unsigned long n)
{
	int	byt_wri;
	int	wdt_wri;

	if (flags->minus)
	{
		byt_wri = print_precised_number(num_str, flags, n);
		if (byt_wri == -1)
			return (-1);
		wdt_wri = print_width(flags->fd, calc_total_width(num_str, flags, byt_wri), ' ');
		if (wdt_wri == -1)
			return (-1);
		return (byt_wri + wdt_wri);
	}
	wdt_wri = 0;
	if ((!flags->zero && flags->width)
		|| (flags->zero && flags->precision != -2 && flags->width))
		wdt_wri = print_width(flags->fd, calc_total_width(num_str, flags, 0), ' ');
	if (wdt_wri == -1)
		return (-1);
	byt_wri = print_precised_number(num_str, flags, n);
	if (byt_wri == -1)
		return (-1);
	return (byt_wri + wdt_wri);
}

int	print_number(long n, t_ft_printf_flags *flags)
{
	char	*num_str;
	int		num_length;

	num_str = ft_ltoa(n);
	if (!num_str)
		return (-1);
	num_length = print_sig_number_string(num_str, flags, n);
	free(num_str);
	return (num_length);
}

int	print_unsigned_number(unsigned long n, t_ft_printf_flags *flags)
{
	char	*num_str;
	int		num_length;

	num_str = ft_uitoa(n);
	if (!num_str)
		return (-1);
	num_length = print_uns_nbr_string(num_str, flags, n);
	free(num_str);
	return (num_length);
}
