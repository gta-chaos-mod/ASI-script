#pragma once

#include "util/EffectBase.h"
#include "util/GameUtil.h"

#include "CCarCtrl.h"

class HalfGravityEffect : public EffectBase
{
private:
    float gravity = 0.004f;

public:
    HalfGravityEffect ();

    void Disable () override;

    void HandleTick () override;
};
