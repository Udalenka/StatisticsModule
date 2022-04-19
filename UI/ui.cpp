#include "ui.h"
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QToolButton>
#include "Service/rtc_engine.h"
#include "signaling_client_interface.h"
#include "video_room_client.h"
#include "message_models.h"
#include "utils/string_utils.h"
#include "signaling_events.h"
#include "api/media_stream_interface.h"
#include "gl_video_renderer.h"
#include "participant.h"
#include "logger/logger.h"
#include "video_room_event_adapter.h"
#include "video_room_dialog.h"
#include "i_video_room_api.h"
#include "media_event_adapter.h"
#include "participants_event_adapter.h"
#include "media_controller.h"
#include "participants_controller.h"
#include "video_room_client_interface.h"
#include "app_delegate.h"
#include "join_room_dialog.h"
#include "create_room_dialog.h"

GUI::GUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    //ui.mainToolBar->setFixedHeight(64);
    this->setWindowState(Qt::WindowMaximized);

	_videoRoomEventAdapter = std::make_shared<VideoRoomEventAdapter>(this);
	connect(_videoRoomEventAdapter.get(), &VideoRoomEventAdapter::createRoom, this, &GUI::onCreateRoom, Qt::QueuedConnection);
	connect(_videoRoomEventAdapter.get(), &VideoRoomEventAdapter::joinRoom, this, &GUI::onJoinRoom, Qt::QueuedConnection);
	connect(_videoRoomEventAdapter.get(), &VideoRoomEventAdapter::leaveRoom, this, &GUI::onLeaveRoom, Qt::QueuedConnection);

	_mediaEventAdapter = std::make_shared<MediaEventAdapter>(this);
	connect(_mediaEventAdapter.get(), &MediaEventAdapter::mediaStatus, this, &GUI::onMediaStatus, Qt::QueuedConnection);
	connect(_mediaEventAdapter.get(), &MediaEventAdapter::createVideoTrack, this, &GUI::onCreateVideoTrack, Qt::QueuedConnection);
	connect(_mediaEventAdapter.get(), &MediaEventAdapter::removeVideoTrack, this, &GUI::onRemoveVideoTrack, Qt::QueuedConnection);
	connect(_mediaEventAdapter.get(), &MediaEventAdapter::localAudioMuted, this, &GUI::onLocalAudioMuted, Qt::QueuedConnection);
	connect(_mediaEventAdapter.get(), &MediaEventAdapter::localVideoMuted, this, &GUI::onLocalVideoMuted, Qt::QueuedConnection);
	connect(_mediaEventAdapter.get(), &MediaEventAdapter::remoteAudioMuted, this, &GUI::onRemoteAudioMuted, Qt::QueuedConnection);
	connect(_mediaEventAdapter.get(), &MediaEventAdapter::remoteVideoMuted, this, &GUI::onRemoteVideoMuted, Qt::QueuedConnection);

	_participantsEventAdapter = std::make_shared<ParticipantsEventAdapter>(this);
	connect(_participantsEventAdapter.get(), &ParticipantsEventAdapter::createParticipant, this, &GUI::onCreateParticipant, Qt::QueuedConnection);
	connect(_participantsEventAdapter.get(), &ParticipantsEventAdapter::updateParticipant, this, &GUI::onUpdateParticipant, Qt::QueuedConnection);
	connect(_participantsEventAdapter.get(), &ParticipantsEventAdapter::removeParticipant, this, &GUI::onRemoveParticipant, Qt::QueuedConnection);

    ui.actionAudio->setEnabled(false);
    ui.actionVideo->setEnabled(false);
}

GUI::~GUI()
{
	if (_galleryView) {
		_galleryView->removeAll();
	}

	//if (_selfContentView) {
	//	_selfContentView->cleanup();
	//}
}

void GUI::init()
{
	_vrc = appDelegate->getRtcEngine()->createVideoRoomClient();
	_vrc->init();

	_vrc->registerEventHandler(_videoRoomEventAdapter);
	_vrc->mediaContrller()->registerEventHandler(_mediaEventAdapter);
	_vrc->participantsController()->registerEventHandler(_participantsEventAdapter);


	_galleryView = new GalleryView(this);
	setCentralWidget(_galleryView);


    QWidget* dockContentView = new QWidget(this);
    QVBoxLayout* dockContentViewLayout = new QVBoxLayout(dockContentView);
    dockContentView->setLayout(dockContentViewLayout);

    //_selfView = new QWidget(this);
    //QGridLayout* selfViewLayout = new QGridLayout(_selfView);
    //_selfView->setLayout(selfViewLayout);
    //dockContentViewLayout->addWidget(_selfView, 100);

    QToolButton* audioButton = new QToolButton(this);
    audioButton->setDefaultAction(ui.actionAudio);

    QToolButton* videoButton = new QToolButton(this);
    videoButton->setDefaultAction(ui.actionVideo);

    QWidget* buttonsView = new QWidget(this);
    QHBoxLayout* buttonsViewLayout = new QHBoxLayout(buttonsView);
    buttonsView->setLayout(buttonsViewLayout);
    buttonsViewLayout->addWidget(audioButton, 1);
    buttonsViewLayout->addWidget(videoButton, 1);
    dockContentViewLayout->addWidget(buttonsView, 1);

	_participantsListView = std::make_shared<ParticipantsListView>(_vrc, this);
	_participantsListView->setFixedWidth(200);
    dockContentViewLayout->addWidget(_participantsListView.get(), 400);

	QDockWidget* dockWidget = new QDockWidget(this);
    dockWidget->setWindowTitle("Participants List");
    dockWidget->setWidget(dockContentView);

	this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void GUI::onStatus(vi::EngineStatus status)
{

}

void GUI::onError(int32_t code)
{

}

void GUI::onCreateRoom(std::shared_ptr<vi::CreateRoomResult> result, int32_t errorCode)
{
	if (_vrc && errorCode == 0 && result) {
		auto req = std::make_shared<vi::vr::PublisherJoinRequest>();
		req->request = "join";
		req->ptype = "publisher";
		req->room = result->roomId.value_or("");
		req->display = _displayName;
		req->pin = result->pin.value_or("");
		_vrc->join(req);
	}
	else {
		DLOG("create room failed, code = {}", errorCode);
	}
}

void GUI::onJoinRoom(std::string roomId, int32_t errorCode)
{
	DLOG("join room '{}', code = {}", roomId, errorCode);
}

void GUI::onLeaveRoom(std::string roomId, int32_t errorCode)
{
	if (_vrc) {
		_vrc->detach();
	}

	if (_galleryView) {
		_galleryView->removeAll();
	}
}

void GUI::onLocalAudioMuted(bool muted)
{

}

void GUI::onLocalVideoMuted(bool muted)
{

}

void GUI::onRemoteAudioMuted(const std::string& pid, bool muted)
{

}

void GUI::onRemoteVideoMuted(const std::string& pid, bool muted)
{

}

void GUI::onMediaStatus(bool isActive, const std::string& reason)
{
	if (isActive) {
		ui.actionAudio->setEnabled(true);
		ui.actionVideo->setEnabled(true);
		if (_vrc) {
			if (_vrc->mediaContrller()->isLocalAudioMuted()) {
				ui.actionAudio->setChecked(false);
			}
			else {
				ui.actionAudio->setChecked(true);
			}

			if (_vrc->mediaContrller()->isLocalVideoMuted()) {
				ui.actionVideo->setChecked(false);
			}
			else {
				ui.actionVideo->setChecked(true);
			}
		}
	}
}

void GUI::onCreateParticipant(std::shared_ptr<vi::Participant> participant)
{
	_participantsListView->addParticipant(participant);
}

void GUI::onUpdateParticipant(std::shared_ptr<vi::Participant> participant)
{

}

void GUI::onRemoveParticipant(std::shared_ptr<vi::Participant> participant)
{
	if (participant) {
		_participantsListView->removeParticipant(participant);
	}
}

void GUI::onCreateVideoTrack(uint64_t pid, rtc::scoped_refptr<webrtc::VideoTrackInterface> track)
{
	if (!track) {
		return;
	}
	if (track->kind() == webrtc::MediaStreamTrackInterface::kVideoKind) {
        //if (_vrc->getId() != pid) {
			GLVideoRenderer* renderer = new GLVideoRenderer(_galleryView);
            renderer->init();
            renderer->show();

            std::shared_ptr<ContentView> view = std::make_shared<ContentView>(pid, track, renderer);
            view->init();

            _galleryView->insertView(view);
   //     }
   //     else {
			//GLVideoRenderer* renderer = new GLVideoRenderer(this);
   //         renderer->init();
   //         renderer->show();

   //         _selfContentView = std::make_shared<ContentView>(pid, track, renderer);
   //         _selfContentView->init();
   //         _selfView->layout()->addWidget(_selfContentView->view());
   //     }
	}
}

void GUI::onRemoveVideoTrack(uint64_t pid, rtc::scoped_refptr<webrtc::VideoTrackInterface> track)
{
	_galleryView->removeView(pid);
}

void GUI::closeEvent(QCloseEvent* event)
{
	if (_vrc) {
		_vrc->detach();
	}

	if (_galleryView) {
		_galleryView->removeAll();
	}
}

void GUI::on_actionAttachRoom_triggered(bool checked)
{
	if (checked) {
		if (_vrc) {
			_vrc->attach();
		}
	}
}

void GUI::on_actionPublishStream_triggered(bool checked)
{
	if (!_vrc) {
		return;
	}
}

void GUI::on_actionJanusGateway_triggered()
{

}

void GUI::on_actionMyProfile_triggered()
{

}

void GUI::on_actionAboutUs_triggered()
{

}

void GUI::on_actionStatistics_triggered(bool checked)
{

}

void GUI::on_actionConsole_triggered(bool checked)
{

}

void GUI::on_actionCreateRoom_triggered()
{
	if (_vrc) {
		CreateRoomDialog* dlg = new CreateRoomDialog(this);
		if (dlg->exec() == QDialog::Accepted) {
			auto req = std::make_shared<vi::vr::CreateRoomRequest>();
			req->request = "create";
			req->room = dlg->roomId();
			req->description = dlg->description();
			req->secret = dlg->secret();
			req->pin = dlg->pin();
			req->permanent = dlg->permanent();
			req->is_private = dlg->isPrivate();
			_vrc->create(req);

			_displayName = dlg->displayName();
		}
	}
}

void GUI::on_actionJoinRoom_triggered(bool checked)
{
    if (_vrc) {
        JoinRoomDialog* dlg = new JoinRoomDialog(this);
        if (dlg->exec() == QDialog::Accepted) {
            auto req = std::make_shared<vi::vr::PublisherJoinRequest>();
            req->request = "join";
			req->ptype = "publisher";
            req->room = dlg->roomId();
            req->display = dlg->displayName();
			req->pin = dlg->pin();
            _vrc->join(req);
        }
    }
}

void GUI::on_actionAudio_triggered(bool checked)
{
	if (!_vrc) {
		return;
	}

	if (checked) {
		_vrc->mediaContrller()->muteLocalAudio(false);
	}
	else {
		_vrc->mediaContrller()->muteLocalAudio(true);
	}
}

void GUI::on_actionVideo_triggered(bool checked)
{
	if (!_vrc) {
		return;
	}

	if (checked) {
		_vrc->mediaContrller()->muteLocalVideo(false);
	}
	else {
		_vrc->mediaContrller()->muteLocalVideo(true);
	}
}

void GUI::on_actionLeaveRoom_triggered()
{
	if (_vrc) {
		auto req = std::make_shared<vi::vr::LeaveRequest>();
		req->request = "leave";
		_vrc->leave(req);
	}
}

