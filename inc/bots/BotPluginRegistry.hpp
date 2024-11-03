#pragma once

#include "BotPlugin.hpp"
#include <map>

#define REGISTER_BOT(name, classType)                                                              \
    static BotPlugin *create##classType() { return new classType(name); }                          \
    static bool       _registered_##classType =                                                    \
        (BotPluginRegistry::plugin_instance().add_bot(name, create##classType), true);

typedef BotPlugin                           *(*PluginFactory)();
typedef std::map<std::string, PluginFactory> MapBotPlugin;

class BotPluginRegistry
{
  public:
    static BotPluginRegistry &plugin_instance(void);

    const BotPlugin *get_bot(const std::string &bot_name) const;
    void             add_bot(const std::string &bot_name, PluginFactory factory);
	void			 runBots(const std::string &port, const std::string &password);

  private:
    // for singleton pattern
    BotPluginRegistry();
    // need to free the new class
    ~BotPluginRegistry();

    MapBotPlugin _bots;
};
