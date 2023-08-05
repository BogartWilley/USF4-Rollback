#pragma once

#include <windows.h>
#include <nlohmann/json.hpp>

namespace Dimps {
	namespace Math {
		struct FixedPoint {
			unsigned short fractional;
			short integral; // Might be unsigned? Not entirely sure
		};

		struct Matrix4x4 {
			float mat[16];
		};

		float FixedToFloat(FixedPoint* fp);

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FixedPoint, fractional, integral);
	}
}