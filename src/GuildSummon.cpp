/*
 * mod-guild-summon
 *
 * A tiny guild convenience module for AzerothCore (WotLK 3.3.5a): a single
 * command that teleports every online member of your guild to you. Stand in
 * Stormwind (or anywhere) and pull your whole guild to your feet in one go -
 * handy for a bot-filled guild.
 *
 *   .guildsummon        (alias: .gsummon)
 *
 * Summoned members are spread over a small radius around the caster so they do
 * not stack on one point. The caster is never summoned to themselves.
 *
 * Released under GNU GPL v2; redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "Chat.h"
#include "CommandScript.h"
#include "Config.h"
#include "Guild.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "Random.h"

#include <cmath>

using namespace Acore::ChatCommands;

class guild_summon_commandscript : public CommandScript
{
public:
    guild_summon_commandscript() : CommandScript("guild_summon_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "guildsummon", HandleGuildSummon, SEC_GAMEMASTER, Console::No },
            { "gsummon",     HandleGuildSummon, SEC_GAMEMASTER, Console::No }
        };
        return commandTable;
    }

private:
    static bool HandleGuildSummon(ChatHandler* handler)
    {
        if (!sConfigMgr->GetOption<bool>("GuildSummon.Enable", true))
        {
            handler->PSendSysMessage("Guild summon is disabled on this server.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        Player* caster = handler->GetSession() ? handler->GetSession()->GetPlayer() : nullptr;
        if (!caster)
            return false;

        Guild* guild = caster->GetGuild();
        if (!guild)
        {
            handler->PSendSysMessage("You are not in a guild.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 const guildId = guild->GetId();
        float const spread = std::max(0.0f, sConfigMgr->GetOption<float>("GuildSummon.SpreadYards", 5.0f));

        float const baseX = caster->GetPositionX();
        float const baseY = caster->GetPositionY();
        float const baseZ = caster->GetPositionZ();
        float const baseO = caster->GetOrientation();
        uint32 const mapId = caster->GetMapId();

        uint32 summoned = 0;
        HashMapHolder<Player>::MapType const& players = ObjectAccessor::GetPlayers();
        for (auto const& pair : players)
        {
            Player* member = pair.second;
            if (!member || member == caster)
                continue;

            if (member->GetGuildId() != guildId)
                continue;

            if (!member->IsInWorld() || member->IsBeingTeleported())
                continue;

            float x = baseX;
            float y = baseY;
            if (spread > 0.0f)
            {
                constexpr float TWO_PI = 6.28318530717958647692f;
                float const angle = frand(0.0f, TWO_PI);
                float const dist = frand(1.0f, spread);
                x += dist * std::cos(angle);
                y += dist * std::sin(angle);
            }

            member->TeleportTo(mapId, x, y, baseZ, baseO);
            ++summoned;
        }

        handler->PSendSysMessage("Summoned {} guild member(s) to your location.", summoned);
        return true;
    }
};

void AddGuildSummonScripts()
{
    new guild_summon_commandscript();
}
