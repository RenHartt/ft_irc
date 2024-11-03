#include "bots/BotPingPong.hpp"
#include <bots/BotPluginRegistry.hpp>
#include <iostream>

BotPluginRegistry::BotPluginRegistry() {}
BotPluginRegistry::~BotPluginRegistry() {}

BotPluginRegistry &BotPluginRegistry::plugin_instance(void)
{
    static BotPluginRegistry instance;
    return instance;
}

const BotPlugin *BotPluginRegistry::get_bot(const std::string &bot) const
{
    MapBotPlugin::const_iterator it = _bots.find(bot);
    if (it != _bots.end())
        return it->second();
    return NULL;
}

void BotPluginRegistry::add_bot(const std::string &bot_name, PluginFactory factory)
{
	std::cout << bot_name << " added" << std::endl;
    _bots[bot_name] = factory;
}

void BotPluginRegistry::runBots(const std::string &port ,const std::string &password)
{
	std::cout << "init Bot" << std::endl;
	for (MapBotPlugin::iterator it = _bots.begin(); it != _bots.end(); it++ )
	{
		std::cout << it->first << std::endl;
		BotPlugin *bot = it->second();
		bot->connect(port, password);
	}

}

REGISTER_BOT("PING", BotPING)
