#include "EffectDatabase.h"

#include "Config.h"
#include "DrawHelper.h"
#include "NamedPipe.h"

std::queue<std::function<void()>> EffectDatabase::effectQueue = {};
std::list<EffectBase*> EffectDatabase::effects = {};

void EffectDatabase::Tick() {
	EmptyQueue();

	for (EffectBase* effect : effects) {
		effect->Tick();
	}
	effects.remove_if([](EffectBase* effect) { return !effect->IsRunning(); });
}

void EffectDatabase::EmptyQueue() {
	if (!effectQueue.empty()) {
		effectQueue.front()();
		effectQueue.pop();
	}
}

template<typename _Callable, typename... _Args>
void EffectDatabase::QueueFunction(_Callable&& __f, _Args&& ... __args) {
	effectQueue.push(std::bind(std::forward<_Callable>(__f), std::forward<_Args>(__args)...));
}

void EffectDatabase::QueueEffect(EffectBase* effect, bool executeNow) {
	if (!effect) {
		return;
	}

	if (Config::GetOrDefault("CrowdControl.Enabled", false)) {
		if (GenericUtil::IsMenuActive() || !GameUtil::CanCrowdControlEffectActivate()) {
			NamedPipe::SendCrowdControlResponse(effect->GetCrowdControlID(), 2);
			return;
		}
	}

	auto effectFunction = [effect]() {
		if (!effect->CanActivate() && Config::GetOrDefault("CrowdControl.Enabled", false)) {
			NamedPipe::SendCrowdControlResponse(effect->GetCrowdControlID(), 2);
			return;
		}

		// If an effect with the same type or description is found, disable it, then add the effect and tick it down to start it
		auto it = std::find_if(effects.begin(), effects.end(), [effect](EffectBase* _effect) {
			if (_effect->IsEnabled() || _effect->IsDisabledByOtherEffect() || (!_effect->IsEnabled() && _effect->IsDisabledForMissions())) {
				return effect->IsEqual(_effect);
			}
			return false;
		});

		if (it != effects.end()) {
			EffectBase* effect = *it;
			if (effect) {
				if (Config::GetOrDefault("CrowdControl.Enabled", false)) {
					NamedPipe::SendCrowdControlResponse(effect->GetCrowdControlID(), 2);
					return;
				}
				effect->DisableByOtherEffect();
			}
		}

		NamedPipe::SendCrowdControlResponse(effect->GetCrowdControlID(), 1);

		effects.push_front(effect);
		effect->Tick();
	};

	if (executeNow) {
		effectFunction();
	}
	else {
		QueueFunction(effectFunction);
	}
}

void EffectDatabase::HandleFunction(std::string state, std::string text) {
	char c_function[128] = {};
	int duration = 0;
	char c_description[128] = {};
	char c_voter[64] = {};
	int rapid_fire = 0;
	int crowd_control_id = -1;
	sscanf(text.c_str(), "%[^:]:%d:%[^:]:%[^:]:%d:%d", &c_function, &duration, &c_description, &c_voter, &rapid_fire, &crowd_control_id);

	if (rapid_fire == 1) {
		duration = 1000 * 15; // 15 Seconds
	}

	const std::string function(c_function);
	const std::string description(c_description);
	const std::string voter(c_voter);

	EffectBase* effect = nullptr;
	if (state == "other") {
		if (function == "clear_active_effects") {
			QueueFunction([duration, description, voter, rapid_fire] {
				for (EffectBase* effect : effects) {
					effect->Disable();
				}
			});
			effect = new EffectPlaceholder("clear_active_effects");
		}
	}
	else {
		effect = EffectHandler::Get(state, function);
	}

	if (effect) {
		effect->SetDescription(description);
		if (rapid_fire) {
			effect->SetEffectDuration(min(effect->GetEffectDuration(), duration));
		}
		effect->SetDuration(duration);
		effect->SetTwitchVoter(voter);

		if (crowd_control_id > -1) {
			if (!Config::GetOrDefault("CrowdControl.Enabled", false)) {
				NamedPipe::SendCrowdControlResponse(effect->GetCrowdControlID(), 2);
				return;
			}
			effect->SetCrowdControlID(crowd_control_id);
		}

		QueueEffect(effect);
	}
}

std::list<EffectBase*> EffectDatabase::GetActiveEffects() {
	return effects;
}
