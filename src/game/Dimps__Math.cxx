#include "Dimps__Math.hxx"

using Dimps::Math::FixedPoint;

float Dimps::Math::FPtoFloat(FixedPoint* fp) {
	return fp->integral + ((float)fp->fractional / (1 << 16));
}