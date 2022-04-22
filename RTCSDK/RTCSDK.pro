# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = RTCSDK
DESTDIR = ../x64/Debug
CONFIG += staticlib debug
DEFINES += _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 BUILD_STATIC USE_AURA=1 NO_TCMALLOC FULL_SAFE_BROWSING SAFE_BROWSING_CSD SAFE_BROWSING_DB_LOCAL CHROMIUM_BUILD _HAS_EXCEPTIONS=0 __STD_C _CRT_RAND_S _CRT_SECURE_NO_DEPRECATE _SCL_SECURE_NO_DEPRECATE _ATL_NO_OPENGL _WINDOWS CERT_CHAIN_PARA_HAS_EXTRA_FIELDS PSAPI_VERSION=2 _SECURE_ATL _USING_V110_SDK71_ WINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP WIN32_LEAN_AND_MEAN NOMINMAX NTDDI_VERSION=NTDDI_WIN10_RS2 _WIN32_WINNT=0x0A00 WINVER=0x0A00 DYNAMIC_ANNOTATIONS_ENABLED=1 WTF_USE_DYNAMIC_ANNOTATIONS=1 WEBRTC_ENABLE_PROTOBUF=1 WEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE RTC_ENABLE_VP9 HAVE_SCTP WEBRTC_USE_H264 WEBRTC_NON_STATIC_TRACE_EVENT_HANDLERS=0 WEBRTC_WIN ABSL_ALLOCATOR_NOTHROW=1 HAVE_WEBRTC_VIDEO HAVE_WEBRTC_VOICE ASIO_STANDALONE _WEBSOCKETPP_CPP11_INTERNAL_
INCLUDEPATH += ./GeneratedFiles \
    ./GeneratedFiles/$(ConfigurationName) \
    . \
    ./../3rd \
    ./../3rd/webrtc/include \
    ./../3rd/webrtc/include/third_party/abseil-cpp \
    ./../3rd/webrtc/include/third_party/libyuv/include \
    ./../3rd/asio/asio/include \
    ./../3rd/websocketpp \
    ./../3rd/rapidjson/include \
    ./../3rd/spdlog/include \
    ./../3rd/concurrentqueue

LIBS += ../3rd/webrtc/lib/windows_release_x64/x64 -lwebrtc
LIBS += ../3rd/glew/lib/Release/x64 -lglew32
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(RTCSDK.pri)
