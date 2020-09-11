#include "AutosaveEffect.h"

AutosaveEffect::AutosaveEffect (int missionsPassed)
    : EffectPlaceholder ("effect_autosave")
{
    this->missionsPassed = missionsPassed;

    SetDuration (1000 * 10);
    SetDescription ("Autosaving...");
    SetCanTickWhenDisabled ();
}

bool
AutosaveEffect::IsPlaceholder ()
{
    return didSave;
}

void
AutosaveEffect::HandleTick ()
{
    EffectPlaceholder::HandleTick ();

    CPlayerPed *player = FindPlayerPed ();
    if (player && !didSave && !CTheScripts::IsPlayerOnAMission ()
        && !CGangWars::State)
    {
        CRunningScript *script = CTheScripts::pActiveScripts->m_pNext;
        while (script != nullptr)
        {
            if (script->m_bIsMission && script->m_bIsActive)
            {
                return;
            }

            script = script->m_pNext;
        }

        bool wasInVehicle              = player->m_nPedFlags.bInVehicle;
        player->m_nPedFlags.bInVehicle = false;

        if (Config::GetOrDefault ("Chaos.SaveToSlot8", false))
        {
            GameUtil::SaveToFile ("GTASAsf8.b");
        }

        GameUtil::SaveToFile ("chaos_mod\\chaos_autosave.b");

        std::string missionSave;
        missionSave.append ("chaos_mod\\chaos_autosave.mission_")
            .append (std::to_string (missionsPassed))
            .append (".b");
        GameUtil::SaveToFile (missionSave);

        player->m_nPedFlags.bInVehicle = wasInVehicle;

        SetDescription ("Autosave Completed");
        SetEffectDuration (GetEffectDuration ());

        didSave = true;
    }
}
