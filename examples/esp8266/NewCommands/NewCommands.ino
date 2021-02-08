#include <ESP8266WiFi.h>
#include <esp8266-google-home-notifier.h>

#define SSID "<YOUR_WIFI_SSID>"
#define PASSWORD "<YOUR_WIFI_PASSWORD>"
#define SPEAKER "<YOUR_GOOGLE_SPEAKER_NAME>"

GoogleHomeNotifier ghn;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("");
  Serial.print("* connecting to Wi-Fi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //Print the local IP
  
  const char displayName[] = SPEAKER;

  Serial.println("connecting to Google Home...");
  if (ghn.device(displayName, "en") != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  Serial.print("found Google Home(");
  Serial.print(ghn.getIPAddress());
  Serial.print(":");
  Serial.print(ghn.getPort());
  Serial.println(")");
  
  // Set volume
  Serial.println("* Set volume");
  if (ghn.setVolume(0.3) != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  delay(2000);
  // // Say hello world
  Serial.println("* Hello, World!");
  if (ghn.notify("Hello, World!") != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  delay(3000);
  // Load and autoplay mp3
  Serial.println("* Load mp3");
  if (ghn.play("https://files.freemusicarchive.org/storage-freemusicarchive-org/music/ccCommunity/The_Kyoto_Connection/Wake_Up/The_Kyoto_Connection_-_09_-_Hachiko_The_Faithtful_Dog.mp3") != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  delay(10000);
  // Set volume
  Serial.println("* Set volume");
  if (ghn.setVolume(0.2) != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  delay(5000);
  // Pause mp3
  Serial.println("* Pause mp3");
  if (ghn.pause() != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  delay(10000);
  // Continue play mp3
  Serial.println("* Continue to play mp3");
  if (ghn.play() != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  delay(5000);
  // Status
  Serial.println("* Status");
  Serial.println(ghn.status());
  delay(2000);
  // Seek
  Serial.println("* Seek");
  if (ghn.seek(ghn.PLAYBACK_START, 56.7f) != true) {
    Serial.println(ghn.getLastError());
    return;
  }
  // Status
  Serial.println("* Status");
  Serial.println(ghn.status());
  delay(10000);
  // Stop
  Serial.println("* Stop");
  if (ghn.stop() != true) {
    Serial.println(ghn.getLastError());
    return;
  }

  ghn.disconnect();
  Serial.println("Done.");
}

void loop() {
  // put your main code here, to run repeatedly:
}
