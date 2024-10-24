/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:23:57 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 17:05:16 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
        return (EXIT_FAILURE);
    }

    Server irc_server(argv[1], argv[2]);
    irc_server.run();

    return (EXIT_SUCCESS);
}
