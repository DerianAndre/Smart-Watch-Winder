/*
 * Smart Watch Winder
 * Copyright (C) 2021  Derian Castillo
 * Version: 1.0.0
 * Date:    06/12/2021
 */
const char* VERSION = "1.0.0";

// Includes
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>
#include <config.h>
//#include <Preferences.h>

WiFiClient client;

WebSocketClient wsClient;

String dataToSend;

void initMotor() {
  Serial.println("[info][motor] Initializing motor...");
  // Sets the pins as outputs:
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPWM, OUTPUT);
  // Default mode off
  digitalWrite(motorPWM, LOW);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  // Serial print
  Serial.println("[info][motor] Motor is ready!");
}

void initWiFi() {
  Serial.println("[info][wifi] Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[info][wifi] Connecting");
  int timeout_s = 30;
  while (WiFi.status() != WL_CONNECTED && timeout_s-- > 0) {
    Serial.print('.');
    delay(1000);
  }
  // Connect
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("[error][wifi] Unable to connect, check your credentials!");
    return;
  } else {
    Serial.println();
    Serial.println("[info][wifi] WiFi is connected");
    Serial.print("[info][wifi] IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("[info][wifi] RRSI: ");
    Serial.println(WiFi.RSSI());
    Serial.println("[info][wifi] WiFi is ready!");
  }
}

void initWebSocket() {
  Serial.println("[info][websocket] Initializing WebSocket...");
  if(!client.connect(WS_HOST, WS_PORT)) {
    Serial.println("[error][websocket] Connection failed!");
    return;
  }
  Serial.println("[info][websocket] Connected.");
  wsClient.path = WS_PATH;
  wsClient.host = WS_HOST;
  if (!wsClient.handshake(client)) {
    Serial.println("[error][websocket] Handshake failed!");
    return;
  }
  Serial.println("[info][websocket] Handshake successful!");
  Serial.println("[info][websocket] WebSocket is ready!");
  // Make sure we are connected
  if (client.connected()) {
    wsClient.sendData("{\"type\":\"connection\",\"client\":\"esp32\",\"id\":\"ESP32XXXXXXX\"}");
    Serial.println("[info][websocket] Device connected to the server");
  } else {
    Serial.println("[info][websocket] Device disconnected from server");
  }
}

void wsOnMessage(String &data) {
  Serial.println("[info][websocket] Data received!");
  Serial.println(data);
}

void motorToggle(bool value) {
  if(value) {
    Serial.print("[test][motor] Motor ON ");
    digitalWrite(motorPWM, HIGH);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
  } else {
    Serial.print("[test][motor] Motor OFF ");
    digitalWrite(motorPWM, LOW);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }
}

void command(String &command) {
}

void testMotor() {
  Serial.println("[test][motor] Motor testing...");
  // Move the DC motor forward at maximum speed
  Serial.println("[test][motor] Moving CW...");
  digitalWrite(motorPWM, HIGH);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  delay(5000);
  Serial.println("[test][motor] Moving CCW...");
  digitalWrite(motorPWM, HIGH);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  Serial.println("[info][system] Initializing all systems...");
  initMotor();
  initWiFi();
  initWebSocket();
  Serial.println("[info][system] All systems are ready!");
  Serial.print("[info][system] Version: ");
  Serial.println(VERSION);
  //testMotor();
}

void loop() {
  String data;
  StaticJsonDocument<124> doc;
  if (client.connected()) {
    wsClient.getData(data);
    if (data.length() > 0) {
      wsOnMessage(data);
      // Deserialize JSON
      deserializeJson(doc, data);
      const char* type = doc["type"]; // "action"
      const char* action = doc["action"]; // "update"
      const char* setting = doc["setting"]; // "status"
      bool value = doc["value"]; // true
      motorToggle(value);
    }
    data = "";
  } else {
    Serial.println("[info][websocket] Device disconnected from server");
    delay(3000);
  }
  delay(500);
}