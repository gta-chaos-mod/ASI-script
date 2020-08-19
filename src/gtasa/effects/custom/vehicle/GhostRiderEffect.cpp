#include "GhostRiderEffect.h"

GhostRiderEffect::GhostRiderEffect () : EffectBase ("effect_ghost_rider") {}

void
GhostRiderEffect::Disable ()
{
    if (Config::GetOrDefault ("CrowdControl.Enabled", false))
    {
        CVehicle *vehicle = FindPlayerVehicle (-1, false);
        if (vehicle)
        {
            vehicle->m_nPhysicalFlags.bExplosionProof = false;
            vehicle->BlowUpCar (NULL, false);
        }
    }

    EffectBase::Disable ();
}

void
GhostRiderEffect::HandleTick ()
{
    EffectBase::HandleTick ();

    int step = CalculateTick ();

    if (vehicleList.size () > 0)
    {
        for (auto it = vehicleList.begin (); it != vehicleList.end (); ++it)
        {
            CVehicle *vehicle = it->first;

            if (vehicle)
            {
                vehicleList[vehicle] -= step;
                if (vehicleList[vehicle] < 0)
                {
                    if (CPools::ms_pVehiclePool->IsObjectValid (vehicle))
                    {
                        vehicle->m_nPhysicalFlags.bExplosionProof = false;
                        vehicle->BlowUpCar (NULL, false);
                    }
                    it = vehicleList.erase (it);
                }
            }
            else
            {
                it = vehicleList.erase (it);
            }
        }
    }

    CVehicle *currentVehicle = FindPlayerVehicle (-1, false);

    if (currentVehicle && currentVehicle->IsDriver (FindPlayerPed ()))
    {
        if (vehicleList.contains (currentVehicle))
        {
            auto it = vehicleList.find (currentVehicle);
            if (it != vehicleList.end ())
            {
                it = vehicleList.erase (it);
            }
        }

        SetBurnTimer (currentVehicle, 0.0f);
    }
    else if (!currentVehicle && lastVehicle
             && !vehicleList.contains (lastVehicle))
    {
        vehicleList[lastVehicle] = 5000;
        lastVehicle->m_fHealth   = 1000.0f;
    }

    lastVehicle = currentVehicle;
}

void
GhostRiderEffect::SetBurnTimer (CVehicle *vehicle, float value)
{
    if (!vehicle || !CPools::ms_pVehiclePool->IsObjectValid (vehicle))
    {
        return;
    }

    CBaseModelInfo *model
        = CModelInfo::ms_modelInfoPtrs[vehicle->m_nModelIndex];
    if (!model)
    {
        return;
    }

    CVehicleModelInfo *vehicleModelInfo
        = reinterpret_cast<CVehicleModelInfo *> (model);
    if (!vehicleModelInfo)
    {
        return;
    }

    switch (vehicleModelInfo->m_nVehicleType)
    {
    case VEHICLE_BIKE:
    {
        CBike *bike     = (CBike *) vehicle;
        bike->m_fHealth = 249.0f;
        // bike->field_7BC = (int)value; // This should also be a float, until
        // then we use the float pointer
        *(float *) ((char *) vehicle + 1980) = value;
        break;
    }
    case VEHICLE_AUTOMOBILE:
    case VEHICLE_MTRUCK:
    case VEHICLE_QUAD:
    case VEHICLE_TRAILER:
    {
        CAutomobile *automobile = (CAutomobile *) vehicle;
        automobile->m_fHealth   = 249.0f;
        // automobile->m_dwBurnTimer = (int)value; // This should be a float,
        // until then we use the float pointer
        *(float *) ((char *) vehicle + 2276) = value;
        break;
    }
    case VEHICLE_BOAT:
    {
        CBoat *boat           = (CBoat *) vehicle;
        boat->m_fHealth       = 249.0f;
        boat->m_fBurningTimer = value;
        break;
    }
    }
}
