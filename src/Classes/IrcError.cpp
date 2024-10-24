/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babonnet <babonnet@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:37:09 by babonnet          #+#    #+#             */
/*   Updated: 2024/10/24 19:18:05 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IrcError.hpp>
#include <iostream>

/* bad because of cpp98 */
const std::string IrcError::_map_error[] = {
    "",          // NONE
    "Server",    // SERVER
    "Client"     // CLIENT
};

/* constructor */

IrcError::IrcError(const std::string &msg, e_IrcErrorType type)
    : _msg(msg),
      _type(type)
{}

/* getter */

e_IrcErrorType IrcError::getIrcErrorType(void) const { return _type; }
std::string    IrcError::getMsg(void) const { return _msg; }

/* function */

void IrcError::log(void) const
{
	std::cerr << _map_error[_type] << " Error: " << _msg << std::endl;
}
