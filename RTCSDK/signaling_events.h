/**
 * This file is part of janus_client project.
 * Author:    Jackie Ou
 * Created:   2020-10-01
 **/

#pragma once

#include <memory>
#include <functional>
#include "api/peer_connection_interface.h"
#include "api/media_stream_interface.h"
#include "absl/types/optional.h"
#include "message_models.h"

namespace vi {
	using SuccessCallback = std::function<void()>;
	using FailureCallback = std::function<void(const std::string& reason)>;
	using EventCallback = std::function<void(bool success, const std::string& response)>;

	class EventBase {
	public:
		virtual ~EventBase() {}
		std::string name;
		std::shared_ptr<EventCallback> callback;
	};

	class MessageEvent : public EventBase {
	public:
		std::string message;
		std::string jsep;
	};

	class TrickleCandidateEvent : public EventBase {
	public:
		CandidateData candidate;
	};

	class ChannelDataEvent : public EventBase {
	public:
		std::string text;
		std::string label;
	};

	class DtmfEvent : public EventBase {
	public:
		std::string tones;
		int duration;
		int interToneGap;
	};

	struct MediaConfig {
		bool audio = true;
		bool video = true;
		bool data = false;

		bool update = false;

		bool addAudio = false;	
		bool addVideo = false;
		bool addData = false;

		bool keepAudio = false;
		bool keepVideo = false;

		bool replaceAudio = false;
		bool replaceVideo = false;

		bool removeAudio = false;
		bool removeVideo = false;

		absl::optional<bool> audioSend;
		absl::optional<bool> videoSend;

		absl::optional<bool> audioRecv;
		absl::optional<bool> videoRecv;

		absl::optional<bool> failIfNoAudio;
		absl::optional<bool> failIfNoVideo;
	};				   

	struct JsepConfig {
		std::string type;
		std::string sdp;
		bool trickle;
	};

	class PrepareWebrtcEvent : public EventBase {
	public:
		absl::optional<JsepConfig> jsep;
		absl::optional<MediaConfig> media;
		absl::optional<bool> trickle;
		absl::optional<bool> simulcast;
		absl::optional<bool> simulcast2;
		absl::optional<bool> iceRestart;
		rtc::scoped_refptr<webrtc::MediaStreamInterface> stream;
	};

	class PrepareWebrtcPeerEvent : public EventBase {
	public:
		absl::optional<JsepConfig> jsep;
	};

	class DetachEvent : public EventBase {
	public:
		bool noRequest;
		std::string jsep;
	};

	class CreateSessionEvent : public EventBase {
	public:
		bool reconnect;
	};

	class DestroySessionEvent : public EventBase {
	public:
		bool notifyDestroyed;
		bool cleanupHandles;
	};
}