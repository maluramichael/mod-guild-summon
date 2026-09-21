# mod-guild-summon

A tiny guild convenience module for AzerothCore (WotLK 3.3.5a). One command
teleports **every online member of your guild to you**:

```
.guildsummon        (alias: .gsummon)
```

Stand in Stormwind (or anywhere) and pull your whole guild to your feet in one
go — handy for a bot-filled guild. Summoned members are scattered over a small
radius around you so they don't stack on a single point; you are never summoned
to yourself.

Requires game-master rights (`SEC_GAMEMASTER`).

## Configuration

See [`conf/mod_guild_summon.conf.dist`](conf/mod_guild_summon.conf.dist):

| Key | Default | Meaning |
|-----|---------|---------|
| `GuildSummon.Enable` | `1` | Master on/off for the command |
| `GuildSummon.SpreadYards` | `5` | Radius over which summoned members are scattered (0 = stack on caster) |

## Install

Drop the folder into the fork's `modules/` directory, re-run `cmake .` in the
build tree (a new module directory requires a CMake re-run) and rebuild
`worldserver`. The `.conf.dist` deploys like any other module config.

## Notes

- The command summons **all** online guild members (bots and real players
  alike) except the caster. On a solo/bot server that is exactly the whole
  fleet; be aware it will also yank any human guildmate who happens to be online.
- Members currently mid-teleport or not in the world are skipped.

## License

GPL v2 (or, at your option, any later version).
