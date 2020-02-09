#include "HalfGravityEffect.h"

HalfGravityEffect::HalfGravityEffect () : EffectBase ("effect_half_gravity")
{
    AddType ("gravity");
}

void
HalfGravityEffect::Disable ()
{
    injector::WriteMemory (0x863984, 0.008f, true);
    injector::WriteMemory (0x871494, (-0.008f / 2), true);

    EffectBase::Disable ();
}

void
HalfGravityEffect::HandleTick ()
{
    EffectBase::HandleTick ();

    GameUtil::SetVehiclesToRealPhysics ();

    injector::WriteMemory (0x863984, gravity, true);

    // Potentially fix bikes disappearing with zero / negative gravity
    injector::WriteMemory (0x871494,
                           gravity == 0.0f ? -0.00000001f : (-gravity / 2),
                           true);
}
