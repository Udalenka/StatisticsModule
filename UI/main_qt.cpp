/**
 * This file is part of janus_client project.
 * Author:    Jackie Ou
 * Created:   2020-10-01
 **/

#include "ui.h"
#include <QtWidgets/QApplication>
#include "rtc_base/checks.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/win32_socket_server.h"
#include <QObject>
#include <memory>
#include "Service/app_instance.h"
#include "signaling_service_interface.h"
#include <QSurfaceFormat>

#include "api/media_stream_interface.h"
#include "api/create_peerconnection_factory.h"
#include "api/video_codecs/builtin_video_decoder_factory.h"
#include "api/video_codecs/builtin_video_encoder_factory.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"
#include "modules/audio_device/include/audio_device.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "modules/video_capture/video_capture_factory.h"
#include "pc/video_track_source.h"
#include "local_video_capture.h"
#include "gl_video_renderer.h"
#include "participant.h"
#include "api/media_stream_interface.h"
#include "janus_connection_dialog.h"
#include "rtc_base/win32_socket_init.h"
#include "rtc_base/physical_socket_server.h"

static void registerMetaTypes()
{
	qRegisterMetaType<std::function<void()>>("std::function<void()>");
	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");

	qRegisterMetaType<uint64_t>("uint64_t");
	qRegisterMetaType<std::shared_ptr<vi::Participant>>("std::shared_ptr<vi::Participant>");
	qRegisterMetaType<rtc::scoped_refptr<webrtc::MediaStreamInterface>>("rtc::scoped_refptr<webrtc::VideoTrackInterface>");
}

static void initOpenGL() {
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(3, 2);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);
}
bool g_showCrashDialog = false;

LONG WINAPI OurCrashHandler(EXCEPTION_POINTERS* exceptionInfo)
{
	std::cout << "Gotcha: " << exceptionInfo->ExceptionRecord->ExceptionInformation << std::endl;

	return g_showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}
int main(int argc, char *argv[])
{
	::SetUnhandledExceptionFilter(OurCrashHandler);

	rtc::WinsockInitializer winsockInit;
	rtc::Win32SocketServer w32ss;
	rtc::Win32Thread w32Thread(&w32ss);
	rtc::ThreadManager::Instance()->SetCurrentThread(&w32Thread);

	rtcApp->init(); 

	registerMetaTypes();

	rtc::InitializeSSL();

	QApplication a(argc, argv);

	initOpenGL();

	int ret = 0;

	auto jcDialog = std::make_shared<JanusConnectionDialog>(nullptr);
	jcDialog->init();
	if (QDialog::Accepted == jcDialog->exec()) {
		jcDialog->cleanup();

		auto ss = rtcApp->getSignalingService();
		std::shared_ptr<UI> w = std::make_shared<UI>();
		ss->registerObserver(w);
		w->show();

		w->init();

		ret = a.exec();
	}

	rtcApp->destroy();

	rtc::CleanupSSL();

	TMgr->stopAll();

	return ret; 
}
