#include <string>

#include <nlohmann/json.hpp>

#include "../Dimps/Dimps__GameEvents.hxx"

namespace sf4e {
	namespace SessionProtocol {
		struct SessionRequest {
			std::string type = std::string("request");
			Dimps::GameEvents::VsMode::ConfirmedCharaConditions chara;
		};

		struct SessionResponse {
			std::string type = std::string("response");
			Dimps::GameEvents::VsMode::ConfirmedCharaConditions chara;
			int stageID;
		};


		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SessionRequest, type, chara);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SessionResponse, type, chara, stageID);
	}
}