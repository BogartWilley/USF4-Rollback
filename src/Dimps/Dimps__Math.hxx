#pragma once

#include <windows.h>

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
	}
}