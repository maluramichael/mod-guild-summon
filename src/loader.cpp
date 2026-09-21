/*
 * mod-guild-summon loader.
 *
 * The playerbots fork auto-globs every module's sources into one lib and looks
 * up a loader symbol derived from the folder name: for folder "mod-guild-summon"
 * that symbol is exactly "Addmod_guild_summonScripts". It must exist and call
 * our real registration function.
 *
 * Released under GNU GPL v2 or (at your option) any later version.
 */

void AddGuildSummonScripts();

void Addmod_guild_summonScripts()
{
    AddGuildSummonScripts();
}
