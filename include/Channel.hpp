/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:56:11 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/20 20:56:00 by bgoron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include <string>
#include <vector>

class Channel
{
	public:
		Channel();
		Channel(const std::string& channel_name);
	
	private:
		std::string name;
		std::vector<Client*> clients;
};
