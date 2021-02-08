#ifndef GoogleHomeNotifier_h
#define GoogleHomeNotifier_h

#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <cast_channel.pb.h>

#include <WiFiClientSecure.h>

#ifdef ARDUINO_ARCH_ESP8266
#include <ESP8266mDNS.h>
#include "esp8266sdk_version.h"
#elif defined ARDUINO_ARCH_ESP32
#include <ESPmDNS.h>
#else
#error "ARDUINO_ARCH_ESP8266 or ARDUINO_ARCH_ESP32 has to be defined."
#endif

#include <google-tts.h>

// #define DEBUG 1

#define LIB_NAME "GoogleHomeNotifier for ESP8266"
#define LIB_VERSION "0.1"

#define APP_ID "CC1AD845"

#define SOURCE_ID "sender-0"
#define DESTINATION_ID "receiver-0"

#define CASTV2_NS_CONNECTION "urn:x-cast:com.google.cast.tp.connection"
#define CASTV2_NS_HEARTBEAT "urn:x-cast:com.google.cast.tp.heartbeat"
#define CASTV2_NS_RECEIVER "urn:x-cast:com.google.cast.receiver"
#define CASTV2_NS_MEDIA "urn:x-cast:com.google.cast.media"

#define CASTV2_DATA_CONNECT "{\"type\":\"CONNECT\"}"
#define CASTV2_DATA_PING "{\"type\":\"PING\"}"
#define CASTV2_DATA_LAUNCH "{\"type\":\"LAUNCH\",\"appId\":\"%s\",\"requestId\":1}"
// Cast command reference: https://developers.google.com/cast/docs/reference/messages#MediaComm
#define CASTV2_DATA_LOAD "{\"type\":\"LOAD\",\"autoplay\":true,\"currentTime\":0,\"activeTrackIds\":[],\"repeatMode\":\"REPEAT_OFF\",\"media\":{\"contentId\":\"%s\",\"contentType\":\"audio/mp3\",\"streamType\":\"BUFFERED\"},\"requestId\":1}"
#define CASTV2_DATA_SETVOL  "{\"type\":\"SET_VOLUME\",\"volume\":{\"level\":%.2f},\"requestId\":1}"
#define CASTV2_DATA_PAUSE "{\"type\":\"PAUSE\",\"mediaSessionId\":%s,\"requestId\":1}"
#define CASTV2_DATA_PLAY "{\"type\":\"PLAY\",\"mediaSessionId\":%s,\"requestId\":1}"
#define CASTV2_DATA_SEEK "{\"type\":\"SEEK\",\"mediaSessionId\":%s,%s\"currentTime\":%.3f,\"requestId\":1}"
#define CASTV2_DATA_STOP "{\"type\":\"STOP\",\"requestId\":1}"
#define CASTV2_DATA_STATUS "{\"type\":\"GET_STATUS\",\"requestId\":1}"

typedef class GoogleHomeNotifier {

private:
  char m_transportid[40] = {0};
  char m_clientid[40] = {0};
  char m_mediaSessionId[5] = {0};
  TTS tts;
  WiFiClientSecure* m_client = nullptr;
  boolean m_clientCreated = false;
  IPAddress m_ipaddress;
  uint16_t m_port = 0;
  char m_locale[10] = "en";
  char m_name[128] = "";
  char m_lastError[128] = "";
  bool m_mDNSIsRunning = false;
  static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
  static bool decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg);
  boolean connect();
  boolean sendConnect();
  void setLastError(const char *lastError);
  boolean sendMessage(const char *sourceId, const char *destinationId, const char *ns, const char *data);
  String receiveMessage(const char *keyword, const unsigned long timeout = 5000);
  boolean cast(WiFiClientSecure* pClient = nullptr);

public:
 enum resumeState {
    PLAYBACK_START,
    PLAYBACK_PAUSE
  };
  boolean ip(IPAddress ip, const char *locale = "en", uint16_t port = 8009);
  boolean device(const char *name, const char *locale = "en", long unsigned to = 10000);
  void disconnect();
  boolean notify(const char *phrase, WiFiClientSecure* pClient = nullptr);
  boolean play(const char *mp3Url, WiFiClientSecure* pClient = nullptr);  // aka. load with autoplay
  boolean sendCommand(const char* command, WiFiClientSecure* pClient = nullptr, const char *sourceId = SOURCE_ID, const char *destinationId = DESTINATION_ID, const char *ns = CASTV2_NS_RECEIVER);
  boolean setVolume(const float vol, WiFiClientSecure* pClient = nullptr);
  boolean stop(WiFiClientSecure* pClient = nullptr);
  boolean pause(WiFiClientSecure* pClient = nullptr);
  boolean play(WiFiClientSecure* pClient = nullptr);
  boolean seek(const resumeState state, const float currentTime, WiFiClientSecure* pClient = nullptr);
  String status(WiFiClientSecure* pClient = nullptr);
  const IPAddress getIPAddress();
  const uint16_t getPort();
  const char * getLastError();
 
} GoogleHomeNotifier;

#endif
