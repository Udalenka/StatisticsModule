/**
 * This file is part of janus_client project.
 * Author:    Jackie Ou
 * Created:   2020-10-01
 **/

#pragma once

#include <memory>
#include <string>
#include "i_plugin_client.h"
#include "i_webrtc_event_handler.h"

namespace vi {
	class WebRTCServiceInterface;
	class TaskScheduler;

	class PluginClient
		: public IPluginClient
		, public IWebRTCEventHandler
		, public std::enable_shared_from_this<PluginClient>
	{
	public:
		PluginClient(std::shared_ptr<WebRTCServiceInterface> wrs);

		~PluginClient();

		uint64_t getId() { return _id; }

		// IPluginClient
		void setHandleId(int64_t handleId) override;

		const std::shared_ptr<PluginContext>& pluginContext() const override;

		void attach() override;

		int32_t remoteVolume(const std::string& mid) override;

		int32_t localVolume(const std::string& mid) override;

		bool isAudioMuted(const std::string& mid) override;

		bool isVideoMuted(const std::string& mid) override;

		bool muteAudio(const std::string& mid) override;

		bool muteVideo(const std::string& mid) override;

		bool unmuteAudio(const std::string& mid) override;

		bool unmuteVideo(const std::string& mid) override;

		std::string getBitrate(const std::string& mid) override;

		void sendMessage(std::shared_ptr<SendMessageEvent> event) override;

		void sendData(std::shared_ptr<SendDataEvent> event) override;

		void sendDtmf(std::shared_ptr<SendDtmfEvent> event) override;

		void createOffer(std::shared_ptr<PrepareWebRTCEvent> event) override;

		void createAnswer(std::shared_ptr<PrepareWebRTCEvent> event) override;

		void handleRemoteJsep(std::shared_ptr<PrepareWebRTCPeerEvent> event) override;

		void hangup(bool sendRequest) override;

		void detach(std::shared_ptr<DetachEvent> event) override;

		void startStatsReport() override;

		void stopStatsReport() override;

	protected:
		uint64_t _id = 0;
		uint64_t _privateId = 0;
		std::shared_ptr<PluginContext> _pluginContext;

		std::shared_ptr<TaskScheduler> _rtcStatsTaskScheduler;
		uint64_t _rtcStatsTaskId = 0;
	};
}

