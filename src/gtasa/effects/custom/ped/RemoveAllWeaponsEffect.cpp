#include "RemoveAllWeaponsEffect.h"

RemoveAllWeaponsEffect::RemoveAllWeaponsEffect ()
    : EffectPlaceholder ("effect_remove_all_weapons")
{
    AddType ("weapons");
}

void
RemoveAllWeaponsEffect::Enable ()
{
    EffectPlaceholder::Enable ();

    for (CPed *ped : CPools::ms_pPedPool)
    {
        GameUtil::ClearWeaponsExceptParachute (ped);
    }
}
