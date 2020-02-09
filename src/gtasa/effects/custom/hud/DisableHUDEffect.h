#pragma once

#include "util/EffectBase.h"

#include "CHud.h"
#include "CMenuSystem.h"
#include "CRadar.h"

class DisableHUDEffect : public EffectBase
{
public:
    DisableHUDEffect ();

    void InitializeHooks () override;

private:
    static void HookedHUDDraw ();
    static void HookedBlipsDraw ();

    static tMenuPanel *HookedCMenuSystemDisplayGridMenu (unsigned __int8 a1,
                                                         unsigned __int8 a2);
    static void HookedCMenuSystemDisplayStandardMenu (unsigned __int8 panelId,
                                                      bool bBrightFont);

    static void HookedCHudDrawAreaName ();
};
