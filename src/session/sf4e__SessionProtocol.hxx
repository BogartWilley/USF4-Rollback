#pragma once

#include <string>
#include <utility>


#include <GameNetworkingSockets/steam/isteamnetworkingutils.h>
#include <nlohmann/json.hpp>

#include "../Dimps/Dimps__GameEvents.hxx"
#include "../Dimps/Dimps__Math.hxx"

#define MAX_SF4E_PROTOCOL_USERS 4

namespace sf4e {
	namespace SessionProtocol {
		using Dimps::Math::FixedPoint;

		struct MemberData {
			std::string name;
			std::string ip;
			uint16_t port;
		};

		typedef std::vector<MemberData> LobbyData;

		struct MatchData {
			MatchData();
			void Clear();

			int64_t readyMessageNum[2];
			Dimps::GameEvents::VsMode::ConfirmedCharaConditions chara[2];
			int64_t stageID;
			DWORD rngSeed;
		};

		enum MessageType {
			// Server-sent messages.
			MT_DATA_UPDATE,
			MT_JOIN_REJ,

			// Client-sent messages.
			MT_JOIN_REQ,
			MT_SET_CONDITIONS,
			MT_REPORT_RESULTS,

			// Sent by both.
			MT_SNAPSHOT
		};

		NLOHMANN_JSON_SERIALIZE_ENUM(MessageType, {
			{MT_DATA_UPDATE, "data_update"},
			{MT_JOIN_REJ, "join_rej"},

			{MT_JOIN_REQ, "join_req"},
			{MT_SET_CONDITIONS, "set_conditions_req"},
			{MT_REPORT_RESULTS, "report_results"},
			{MT_SNAPSHOT, "snapshot"},
		})

		enum JoinResult {
			JOIN_OK = 0,
			JR_REQUEST_INVALID = 1,
			JR_LOBBY_FULL = 2,
			JR_NAME_TAKEN = 3,
			JR_HASH_INVALID = 4,
		};

		NLOHMANN_JSON_SERIALIZE_ENUM(JoinResult, {
			{JOIN_OK, "ok"},
			{JR_REQUEST_INVALID, "request_invalid"},
			{JR_LOBBY_FULL, "lobby_full"},
			{JR_NAME_TAKEN, "name_taken"},
			{JR_HASH_INVALID, "hash_invalid"}
		})

		struct DataUpdate {
			MessageType type = MT_DATA_UPDATE;
			LobbyData lobbyData;
			MatchData matchData;
		};

		struct JoinReject {
			MessageType type = MT_JOIN_REJ;
			JoinResult result;
		};

		struct JoinRequest {
			MessageType type = MT_JOIN_REQ;
			std::string sidecarHash;
			std::string username;
			uint16_t port;
		};

		struct SetConditionsRequest {
			MessageType type = MT_SET_CONDITIONS;
			Dimps::GameEvents::VsMode::ConfirmedCharaConditions chara;
			int32_t stageID;
			uint32_t rngSeed;
		};

		struct ReportResultsRequest {
			MessageType type = MT_REPORT_RESULTS;
			int32_t loserSide;
		};

		struct StateSnapshot {
			struct CharaStateSnapshot {
				int status;
				float rootPos[4];
				int side;
				FixedPoint vit;
				FixedPoint vitmax;
				FixedPoint revenge;
				FixedPoint revengemax;
				FixedPoint recoverable;
				FixedPoint recoverablemax;
				FixedPoint super;
				FixedPoint supermax;
				FixedPoint sctimeamt;
				FixedPoint sctimemax;
				FixedPoint uctime;
				FixedPoint uctimemax;
				FixedPoint damage;
				FixedPoint combodamage;
			};

			int frameIdx;
			CharaStateSnapshot chara[2];
		};

		struct SnapshotMsg {
			MessageType type = MT_SNAPSHOT;
			StateSnapshot snapshot;
		};

		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MemberData, name, ip, port);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MatchData, readyMessageNum, chara, stageID, rngSeed);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DataUpdate, type, lobbyData, matchData);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(JoinReject, type, result);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(JoinRequest, type, sidecarHash, username, port);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SetConditionsRequest, type, chara, stageID, rngSeed);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ReportResultsRequest, type, loserSide);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StateSnapshot::CharaStateSnapshot, status, rootPos, side, vit, vitmax, revenge, revengemax, recoverable, recoverablemax, super, supermax, sctimeamt, sctimemax, uctime, uctimemax, damage, combodamage);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StateSnapshot, frameIdx, chara);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SnapshotMsg, type, snapshot);
	}
}