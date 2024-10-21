/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:01:50 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/21 20:09:42 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Command
{
	public:
		virtual void execute() = 0;
		virtual ~Command();
};

class PrivMsg : public Command
{
	public:
		void execute();
};

class Nick : public Command
{
	public:
		void execute();
};

class User : public Command
{
	public:
		void execute();
};

class Quit : public Command
{
	public:
		void execute();
};
