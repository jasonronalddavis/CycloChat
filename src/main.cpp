#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";
const char* serverName = "https://speech.googleapis.com/v1/speech:recognize?key=YOUR_API_KEY";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void sendAudioToGoogleCloud(byte* audioData, int length) {
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(1024);
  String base64Audio = base64::encode(audioData, length);
  doc["audio"]["content"] = base64Audio;
  doc["config"]["encoding"] = "LINEAR16";
  doc["config"]["sampleRateHertz"] = 16000;
  doc["config"]["languageCode"] = "en-US";

  String requestBody;
  serializeJson(doc, requestBody);

  int httpResponseCode = http.POST(requestBody);
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void loop() {
  // Assuming `audioData` and `length` are filled elsewhere in your code after recording
  byte* audioData; // Your audio data
  int length;      // Length of the audio data

  // Call this function when you have audio data ready to send
  sendAudioToGoogleCloud(audioData, length);

  delay(10000); // Delay for demonstration purposes
}
