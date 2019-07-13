#pragma once

#include "common.h"
#include "plugin.h"

#include "CPools.h"
#include "CCarCtrl.h"

#include "TimedEffect.h"

class Gravity : public TimedEffect
{
private:
	float gravity;

public:
	Gravity(float _gravity, int duration, std::string description);

	void Enable();
	void Disable();
};
