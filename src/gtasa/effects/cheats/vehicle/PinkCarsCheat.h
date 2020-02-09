#pragma once

#include <utility>

#include "util/EffectBase.h"

class PinkCarsCheat : public EffectBase
{
private:
    static std::list<std::pair<RwRGBA *, RwRGBA>> resetMaterialColors;

public:
    PinkCarsCheat ();

    void Enable () override;
    void Disable () override;

private:
    static void SetupRenderEvent (CVehicle *vehicle);
    static void ResetAfterRenderEvent (CVehicle *vehicle);
    static void ModifyCarPaint (CVehicle *vehicle);

    static RpMaterial *MaterialCallback (RpMaterial *material, void *color);
    static RpAtomic *  AtomicCallback (RpAtomic *atomic, void *color);
};
