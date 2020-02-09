#include "DoubleGravityEffect.h"

DoubleGravityEffect::DoubleGravityEffect ()
    : EffectBase ("effect_double_gravity")
{
    AddType ("gravity");
}

void
DoubleGravityEffect::Disable ()
{
    injector::WriteMemory (0x863984, 0.008f, true);
    injector::WriteMemory (0x871494, (-0.008f / 2), true);

    EffectBase::Disable ();
}

void
DoubleGravityEffect::HandleTick ()
{
    EffectBase::HandleTick ();

    GameUtil::SetVehiclesToRealPhysics ();

    injector::WriteMemory (0x863984, gravity, true);

    // Potentially fix bikes disappearing with zero / negative gravity
    injector::WriteMemory (0x871494,
                           gravity == 0.0f ? -0.00000001f : (-gravity / 2),
                           true);
}
