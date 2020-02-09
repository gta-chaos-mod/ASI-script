#include "HitmanLevelForAllWeaponsCheat.h"

HitmanLevelForAllWeaponsCheat::HitmanLevelForAllWeaponsCheat ()
    : EffectPlaceholder ("cheat_hitman_leve_for_all_weapons")
{
}

void
HitmanLevelForAllWeaponsCheat::Enable ()
{
    EffectPlaceholder::Enable ();

    CStats::SetStatValue (eStats::STAT_PISTOL_SKILL, 1000.0f); // Pistol
    CStats::SetStatValue (eStats::STAT_SILENCED_PISTOL_SKILL,
                          1000.0f); // Silenced pistol
    CStats::SetStatValue (eStats::STAT_DESERT_EAGLE_SKILL,
                          1000.0f);                             // Desert eagle
    CStats::SetStatValue (eStats::STAT_SHOTGUN_SKILL, 1000.0f); // Shotgun
    CStats::SetStatValue (eStats::STAT_SAWN_OFF_SHOTGUN_SKILL,
                          1000.0f); // Sawn-off shotgun
    CStats::SetStatValue (eStats::STAT_COMBAT_SHOTGUN_SKILL,
                          1000.0f); // Combat shotgun
    CStats::SetStatValue (eStats::STAT_MACHINE_PISTOL_SKILL,
                          1000.0f);                           // Machine pistol
    CStats::SetStatValue (eStats::STAT_SMG_SKILL, 1000.0f);   // SMG
    CStats::SetStatValue (eStats::STAT_AK_47_SKILL, 1000.0f); // AK47
    CStats::SetStatValue (eStats::STAT_M4_SKILL, 1000.0f);    // M4
    CStats::SetStatValue (eStats::STAT_RIFLE_SKILL, 1000.0f); // Rifle
}
