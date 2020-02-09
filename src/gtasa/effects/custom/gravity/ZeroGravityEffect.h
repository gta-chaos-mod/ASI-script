#pragma once

#include "util/EffectBase.h"
#include "util/GameUtil.h"

#include "CCarCtrl.h"

class ZeroGravityEffect : public EffectBase
{
public:
    ZeroGravityEffect ();

    void Disable () override;

    void HandleTick () override;
};
