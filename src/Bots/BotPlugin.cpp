
#include "bots/BotPlugin.hpp"

BotPlugin::BotPlugin(const std::string &bot_name) : _bot_name(bot_name) {}

void BotPlugin::connect(const std::string &port, const std::string &password)
{
	_network_manager.setPort(port);

}
