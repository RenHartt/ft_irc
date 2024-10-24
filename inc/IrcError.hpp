/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babonnet <babonnet@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 18:21:32 by babonnet          #+#    #+#             */
/*   Updated: 2024/10/24 19:18:30 by babonnet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>

enum e_IrcErrorType {
    NONE,
    SERVER,
    CLIENT,
};

typedef std::map<enum e_IrcErrorType, std::string> MapError;

class IrcError
{

public:
    IrcError(const std::string &msg, e_IrcErrorType type);

    e_IrcErrorType getIrcErrorType(void) const;
    std::string    getMsg(void) const;

    void log(void) const;

  private:
    const std::string _msg;
    e_IrcErrorType    _type;
	static const std::string _map_error[];
};
