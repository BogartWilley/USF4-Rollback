#pragma once

#include "../Dimps/Dimps__Pad.hxx"

namespace sf4e {
	namespace Pad {
		void Install();

		struct System : Dimps::Pad::System
		{
			static const int PLAYBACK_MAX = 60;
			static unsigned int playbackData[2][PLAYBACK_MAX];
			static int playbackFrame;
			static void Install();

			int GetButtons_Mapped(int pindex);
		};
	}
}