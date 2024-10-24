/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoron <bgoron@42angouleme.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 22:23:57 by bgoron            #+#    #+#             */
/*   Updated: 2024/10/24 19:36:51 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
        return (EXIT_FAILURE);
    }

    try
    {
        Server irc_server(argv[1], argv[2]);
        irc_server.run();
    } catch (const IrcError &e)
    {
        e.log();
        return (e.getIrcErrorType());
    }
    return (EXIT_SUCCESS);
}
