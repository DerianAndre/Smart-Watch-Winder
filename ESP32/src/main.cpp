/*
 * Smart Watch Winder
 * Copyright (C) 2021  Derian Castillo
 * Version: 1.0.0
 * Date:    06/12/2021
 */
const char* VERSION = "1.0.0";

// Includes
#include <Arduino.h>
#include <ArduinoJson.h>
#include <analogWrite.h>
#include <WebSocketClient.h>
#include <WiFi.h>
#include <config.h>
//#include <Preferences.h>

WiFiClient client;
WebSocketClient wsClient;
DynamicJsonDocument webSocketJson(256);
String webSocketData;

int motorSpeed = 255;
int turnsPerDay = 640;


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

void wsOnMessage(String &webSocketData) {
  Serial.println("[info][websocket] Data received!");
  serializeJsonPretty(webSocketJson, Serial);
  Serial.println();
}

void motorClockwise() {
  Serial.println("[cmd][motor] Motor rotating clockwise");
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  analogWrite(motorPWM, motorSpeed);
}

void motorCounterClockwise() {
  Serial.println("[cmd][motor] Motor rotating counter-clockwise");
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(motorPWM, motorSpeed);
}

void motorUpdateDirection(String direction) {
  if(direction == "cw") {
    motorClockwise();
  } else if(direction == "ccw") {
    motorCounterClockwise();
  } else if(direction == "both") {
    Serial.print("[cmd][motor] Motor rotating both ");
    motorClockwise();
    delay(turnsPerDay * 1000);
    motorCounterClockwise();
  }
}

void motorToggle(String value) {
  if(value == "true" || value == "on" || value == "1") {
    Serial.print("[cmd][motor] Motor turned on");
    analogWrite(motorPWM, motorSpeed);
  } else {
    Serial.print("[cmd][motor] Motor turned off");
    digitalWrite(motorPWM, LOW);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
  }
}

void motorUpdateSpeed(String motorSpeed) {
  Serial.print("[cmd][motor] Motor speed: ");
  Serial.println(motorSpeed.toInt());
  analogWrite(motorPWM, motorSpeed.toInt());
}

void command(String &command) {
}

void testMotor() {
  Serial.println("[test][motor] Motor testing...");
  // Move the DC motor forward at maximum speed
  Serial.println("[test][motor] Moving CW...");
  motorClockwise();
  delay(5000);
  Serial.println("[test][motor] Moving CCW...");
  motorCounterClockwise();
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
  // Check if we are connected and there is data to read
  if (client.connected()) {
    wsClient.getData(webSocketData);
    if (webSocketData.length() > 0) {
      // Deserialize JSON
      deserializeJson(webSocketJson, webSocketData);
      String type    = webSocketJson["type"];
      String action  = webSocketJson["action"];
      String setting = webSocketJson["setting"];
      String value   = webSocketJson["value"];
      // Print JSON
      wsOnMessage(webSocketData);
      // Check if the type, action and setting
      if(type == "action") {
        if(action == "update") {
          if(setting == "motor" || setting == "status") {
            motorToggle(value);
          } else if(setting == "speed") {
            motorUpdateSpeed(value);
          } else if(setting == "direction") {
            motorUpdateDirection(value);
          } else {
            Serial.println("[error][system] Unknown expression!");
            serializeJsonPretty(webSocketJson, Serial);
          }
        }
      }
    }
    webSocketData = "";
  } else {
    Serial.println("[info][websocket] Device disconnected from server");
    delay(3000);
  }
  delay(500);
}