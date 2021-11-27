#pragma once

#include <windows.h>

namespace Dimps {
	namespace Math {
		struct FixedPoint {
			unsigned short fractional;
			short integral; // Might be unsigned? Not entirely sure
		};

		float FPtoFloat(FixedPoint* fp);
	}
}