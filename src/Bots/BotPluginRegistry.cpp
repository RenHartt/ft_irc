#include <bots/BotPluginRegistry.hpp>

BotPluginRegistry::BotPluginRegistry() {}
BotPluginRegistry::~BotPluginRegistry() {}

BotPluginRegistry &BotPluginRegistry::plugin_instance(void) { return _instance; }

const BotPlugin *BotPluginRegistry::get_bot(const std::string &bot) const
{
    MapBotPlugin::const_iterator it = _bots.find(bot);
    if (it != _bots.end())
        return it->second();
    return NULL;
}

void BotPluginRegistry::add_bot(const std::string bot_name, PluginFactory factory)
{
    _bots[bot_name] = factory;
}
