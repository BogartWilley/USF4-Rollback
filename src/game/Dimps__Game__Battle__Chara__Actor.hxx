#pragma once

#include <windows.h>

#include "Dimps__Math.hxx"

namespace Dimps {
	namespace Game {
		namespace Battle {
			namespace Chara {
				struct Actor
				{
					enum Status {
						STAND = 0,
						CROUCH = 1,
						JUMP = 2,
						STAND_TO_CROUCH = 3,
						CROUCH_TO_STAND = 4,
						STAND_TO_JUMP = 5,
						JUMP_TO_STAND = 6,
						TURN__STAND = 7,
						TURN__CROUCH = 8,
						TURN__WALK = 9,
						FORWARD = 10,
						BACKWARD = 11,
						FRONTDASH = 12,
						BACKDASH = 13,
						GUARD__STAND = 14,
						GUARD__CROUCH = 15,
						SKILL = 16,
						STUN = 17,
						BOUND = 18,
						DOWN = 19,
						RISE = 20,
						DAMAGE = 21,
						DAMAGE__GUARD = 22,
						DAMAGE__BLOW = 23,
						SEQUENCE = 24
					};

					typedef struct __privateMethods {
						// TODO
					} __privateMethods;

					typedef struct __publicMethods {
						unsigned int (Actor::* GetActorID)();
						int (Actor::* GetComboDamage)(Dimps::Math::FixedPoint* out);
						int (Actor::* GetDamage)(Dimps::Math::FixedPoint* out);
						unsigned int (Actor::* GetStatus)();
					} __publicMethods;

					typedef struct __staticMethods {
						// TODO
					} __staticMethods;

					static void Locate(HMODULE peRoot);
					static __privateMethods privateMethods;
					static __publicMethods publicMethods;
					static __staticMethods staticMethods;

					// Instance values here
				};
			}
		}
	}
}