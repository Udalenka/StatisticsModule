#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ui.h"
#include <memory>
#include "video_room_client.h"
#include "gl_video_renderer.h"
#include "gallery_view.h"
#include <QCloseEvent>
#include "participants_list_view.h"
#include "i_engine_event_handler.h"

namespace vi {
	class Participant;
	class ParticipantsListView;
	class VideoRoomClientInterface;
}

class MediaEventAdapter;
class VideoRoomEventAdapter;
class ParticipantsEventAdapter;

class GUI
	: public QMainWindow
	, public vi::IEngineEventHandler
	, public std::enable_shared_from_this<GUI>
{
	Q_OBJECT

public:
	GUI(QWidget *parent = Q_NULLPTR);

	~GUI();

	void init();

	std::shared_ptr<GLVideoRenderer> _renderer;

private slots:

	// IEngineEventHandler

	void onStatus(vi::EngineStatus status) override;

	void onError(int32_t code) override;

	// IVideoRoomEventHandler

	void onCreateRoom(std::shared_ptr<vi::CreateRoomResult> result, int32_t errorCode);

	void onJoinRoom(std::string roomId, int32_t errorCode);

	void onLeaveRoom(std::string roomId, int32_t errorCode);


	// IMediaControlEventHandler

	void onMediaStatus(bool isActive, const std::string& reason);

	void onCreateVideoTrack(uint64_t pid, rtc::scoped_refptr<webrtc::VideoTrackInterface> track);

	void onRemoveVideoTrack(uint64_t pid, rtc::scoped_refptr<webrtc::VideoTrackInterface> track);

	void onLocalAudioMuted(bool muted);

	void onLocalVideoMuted(bool muted);

	void onRemoteAudioMuted(const std::string& pid, bool muted);

	void onRemoteVideoMuted(const std::string& pid, bool muted);


	// IParticipantsControlEventHandler

	void onCreateParticipant(std::shared_ptr<vi::Participant> participant);

	void onUpdateParticipant(std::shared_ptr<vi::Participant> participant);

	void onRemoveParticipant(std::shared_ptr<vi::Participant> participant);

private slots:

	void closeEvent(QCloseEvent* event);

	void on_actionAttachRoom_triggered(bool checked);

    void on_actionPublishStream_triggered(bool checked);

    void on_actionJanusGateway_triggered();

    void on_actionMyProfile_triggered();

    void on_actionAboutUs_triggered();

    void on_actionStatistics_triggered(bool checked);

    void on_actionConsole_triggered(bool checked);

    void on_actionCreateRoom_triggered();

    void on_actionJoinRoom_triggered(bool checked);

    void on_actionAudio_triggered(bool checked);

    void on_actionVideo_triggered(bool checked);

	void on_actionLeaveRoom_triggered();

private:
	Ui::UIClass ui;

	std::shared_ptr<vi::VideoRoomClientInterface> _vrc;

    //std::shared_ptr<IContentView> _selfContentView;

    //QWidget* _selfView;

	GalleryView* _galleryView;

	std::shared_ptr<MediaEventAdapter> _mediaEventAdapter;
	std::shared_ptr<VideoRoomEventAdapter> _videoRoomEventAdapter;
	std::shared_ptr<ParticipantsEventAdapter> _participantsEventAdapter;

	std::shared_ptr<ParticipantsListView> _participantsListView;

	std::string _displayName;
};
