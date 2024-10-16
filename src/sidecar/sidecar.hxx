#pragma once

#include <windows.h>

namespace sf4eSidecar {
	const GUID s_guidSidecarPayload = {
		0xf469d43b, 0x6108, 0x4600,
		{0x9c, 0x74, 0x07, 0x71, 0x57, 0x82, 0xc9, 0x40}
	};

	typedef struct Payload {
		HANDLE hSyncEvent;
	} Payload;
}