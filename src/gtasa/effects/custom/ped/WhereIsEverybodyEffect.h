#pragma once

#include "util/EffectBase.h"

#include "CShadows.h"

class WhereIsEverybodyEffect : public EffectBase
{
public:
    WhereIsEverybodyEffect ();

    void InitializeHooks () override;

    void Disable () override;

    void HandleTick () override;

    static void HookedRenderPedShadow (CPed *thisPed, float disp_x,
                                       float disp_y, float front_x,
                                       float front_y, float side_x,
                                       float side_y);
};
