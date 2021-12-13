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
#include <WebSocketClient.h>
#include <WiFi.h>
#include <Preferences.h>
#include <config.h>
#include <libSys.h>

// Libraries variables
DynamicJsonDocument jsonConnection(128);
DynamicJsonDocument jsonSettings(1024);
DynamicJsonDocument jsonWebSocket(512);
WebSocketClient webSocketClient;
WiFiClient wifiClient;
Preferences preferences;

// Variables
String webSocketData;
String controlMode;
String motorDirection;
bool   motorStatus;
int    motorSpeed;
int    motorSpeedMin = pwmSpeedMin;
int    motorSpeedMax = pwmSpeedMax;
int    turnsCycles;
int    turnsPerDay;
int    turnsPerHour;

// Functions
void wsOnMessage(String &webSocketData) {
  Serial.print("[info][websocket] Data received!: ");
  serializeJson(jsonWebSocket, Serial);
  //serializeJsonPretty(jsonWebSocket, Serial);
  Serial.println();
}

void wsSendMessage(String message) {
  Serial.print("[info][websocket] Sending data: ");
  Serial.println(message);
  webSocketClient.sendData(message);
}

void wsSendConnection() {
  String jsonConnectionString;
  jsonSettings["type"]           = "connection";
  jsonSettings["client"]         = "esp32";
  jsonSettings["id"]             = WS_ID;
  serializeJson(jsonSettings, jsonConnectionString);
  // Print
  Serial.println("[info][websocket] Sending connection...");
  // Send
  wsSendMessage(jsonConnectionString);
}

void wsSendSettings() {
  String jsonSettingsString;
  jsonSettings["type"]           = "settings";
  jsonSettings["id"]             = WS_ID;
  jsonSettings["controlMode"]    = controlMode;
  jsonSettings["motorDirection"] = motorDirection;
  jsonSettings["motorSpeed"]     = motorSpeed;
  jsonSettings["motorSpeedMin"]  = motorSpeedMin;
  jsonSettings["motorSpeedMax"]  = motorSpeedMax;
  jsonSettings["motorStatus"]    = motorStatus;
  jsonSettings["turnsCycles"]    = turnsCycles;
  jsonSettings["turnsPerDay"]    = turnsPerDay;
  jsonSettings["turnsPerHour"]   = turnsPerHour;
  serializeJson(jsonSettings, jsonSettingsString);
  // Print
  Serial.println("[info][websocket] Sending settings...");
  // Send
  wsSendMessage(jsonSettingsString);
}

void initMotor() {
  Serial.println("[info][motor] Initializing motor...");
  // Sets the pins as outputs:
  pinMode(motorIN1A, OUTPUT);
  pinMode(motorIN1B, OUTPUT);
  pinMode(motorEN1,  OUTPUT);
  // Configure PWM functionalitites and
  // Attach the channel to the GPIO to be controlled
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(motorEN1, pwmChannel);
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
  if(!wifiClient.connect(WS_HOST, WS_PORT)) {
    Serial.println("[error][websocket] Connection failed!");
    return;
  }
  Serial.println("[info][websocket] Connected.");
  webSocketClient.path = WS_PATH;
  webSocketClient.host = WS_HOST;
  if (!webSocketClient.handshake(wifiClient)) {
    Serial.println("[error][websocket] Handshake failed!");
    return;
  }
  // Make sure we are connected
  if (wifiClient.connected()) {
    // Print
    Serial.println("[info][websocket] Handshake successful!");
    Serial.println("[info][websocket] Device connected to the WebSocket server");
    Serial.println("[info][websocket] WebSocket is ready!");
    // Send a message to the server
    wsSendConnection();
  } else {
    // Print error
    Serial.println("[error][websocket] WebSocket connection faild!");
    // Reconnect
    Serial.println("[info][websocket] Reconnecting...");
    initWebSocket();
    // Wait
    delay(3000);
  }
}

void initDone() {
  Serial.println("[info][system] All systems are ready!");
  Serial.print("[info][system] Program version: ");
  Serial.println(VERSION);
}

void motorDirectionCW() {
  Serial.println("[cmd][motor] Motor rotating clockwise");
  digitalWrite(motorIN1A, LOW);
  digitalWrite(motorIN1B, HIGH);
}

void motorDirectionCCW() {
  Serial.println("[cmd][motor] Motor rotating counter-clockwise");
  digitalWrite(motorIN1A, HIGH);
  digitalWrite(motorIN1B, LOW);
}

void motorDirectionBOTH() {
  Serial.println("[cmd][motor] Motor rotating both ");
  motorDirectionCW();
  delay(turnsPerDay * 1000);
  motorDirectionCCW();
}

void motorUpdateDirection(String newMotorDirection) {
  // Clockwise
  if(newMotorDirection == "cw") {
    motorDirectionCW();
  }
  // Counter clockwise
  else if(newMotorDirection == "ccw") {
    motorDirectionCCW();
  }
  // Both
  else if(newMotorDirection == "both") {
    motorDirectionBOTH();
  }
}

void motorUpdateSpeed(int newMotorSpeed) {
  Serial.print("[cmd][motor] Motor speed is ");
  Serial.println(newMotorSpeed);
  // Add preferences update to save state to memory
  if(!motorStatus) return;
  // Fix when the newMotorSpeed is higher/lower than the pwmSpeedMax/pwmSpeedMin
  if(newMotorSpeed > pwmSpeedMax) newMotorSpeed = pwmSpeedMax;
  if(newMotorSpeed < pwmSpeedMin) newMotorSpeed = pwmSpeedMin;
  // This fixes the issue where the motor is not spinning
  // when starting off from a low speed
  ledcWrite(pwmChannel, 200);
  delay(10);
  // Mapped the range of the speed to the range of the PWM resolution and Duty Cycle
  int newMotorSpeedMapped = map(newMotorSpeed, pwmSpeedMin, pwmSpeedMax, 170, 255);
  // Write the new speed to the PWM channel
  ledcWrite(pwmChannel, newMotorSpeedMapped);
}

void motorUpdateStatus(bool newMotorStatus) {
  if(newMotorStatus) {
    Serial.println("[cmd][motor] Motor turned on");
    digitalWrite(motorEN1, HIGH);
    motorUpdateSpeed(motorSpeed);
  } else {
    Serial.println("[cmd][motor] Motor turned off");
    digitalWrite(motorEN1, LOW);
    ledcWrite(pwmChannel, 0);
  }
}

void command(String &command) {

}

void testMotor() {
  Serial.println("[test][motor] Motor testing...");
  // Move the DC motor forward at maximum speed
  Serial.println("[test][motor] Moving CW...");
  motorDirectionCW();
  delay(5000);
  Serial.println("[test][motor] Moving CCW...");
  motorDirectionCCW();
  delay(5000);
}

void updateSetting(String setting, String value) {
  preferences.begin("settings", false);
  Serial.print("[info][settings] Updating setting: ");
  Serial.print(setting);
  Serial.print(" to ");
  Serial.println(value);
  // Control Mode
  if(setting == "controlMode") {
    String valueString = value;
    controlMode = valueString;
    preferences.putString("controlMode", valueString);
  }
  // Motor status
  else if(setting == "motorStatus") {
    bool valueBoolean = value == "true" ? true : false;
    motorStatus = valueBoolean;
    preferences.putBool("motorStatus", valueBoolean);
    motorUpdateStatus(valueBoolean);
  }
  // Motor speed
  else if(setting == "motorSpeed") {
    int valueInt = value.toInt();
    motorSpeed = valueInt;
    preferences.putInt("motorSpeed", valueInt);
    motorUpdateSpeed(valueInt);
  }
  // Motor direction
  else if(setting == "motorDirection") {
    String valueString = value;
    motorDirection = valueString;
    preferences.putString("motorDirection", valueString);
    motorUpdateDirection(value);
  }
  // Turns per day
  else if(setting == "turnsPerDay") {
    int valueInt = value.toInt();
    turnsPerDay = valueInt;
    preferences.putInt("turnsPerDay", valueInt);
  }
  // Turns per hour
  else if(setting == "turnsPerHour") {
    int valueInt = value.toInt();
    turnsPerHour = valueInt;
    preferences.putInt("turnsPerHour", valueInt);
  }
  // Turns Cycles
  else if(setting == "turnsCycles") {
    int valueInt = value.toInt();
    turnsCycles = valueInt;
    preferences.putInt("turnsCycles", valueInt);
  }
  // Unknown setting
  else {
    Serial.print("[error][system] Unknown setting: ");
    Serial.println(setting);
  }
  // End
  preferences.end();
  wsSendSettings();
}

void loadSettings() {
  preferences.begin("settings", false);
  Serial.println("[info][settings] Loading settings...");
  controlMode=    preferences.getString("controlMode", defaultControlMode);
  motorDirection= preferences.getString("motorDirection", defaultMotorDirection);
  motorStatus=    preferences.getBool("motorStatus", defaultMotorStatus);
  motorSpeed=     preferences.getInt("motorSpeed", defaultMotorSpeed);
  turnsCycles=    preferences.getInt("turnsCycles", defaultTurnsCycles);
  turnsPerDay=    preferences.getInt("turnsPerDay", defaultTurnsPerDay);
  turnsPerHour=   preferences.getInt("turnsPerHour", defaultTurnsPerHour);
  preferences.end();

  Serial.print("[info][settings] Control mode: ");
  Serial.println(controlMode);
  Serial.print("[info][settings] Motor direction: ");
  Serial.println(motorDirection);
  Serial.print("[info][settings] Motor status: ");
  Serial.println(motorStatus);
  Serial.print("[info][settings] Motor speed: ");
  Serial.println(motorSpeed);
  Serial.print("[info][settings] Turns cycles: ");
  Serial.println(turnsCycles);
  Serial.print("[info][settings] Turns per day: ");
  Serial.println(turnsPerDay);
  Serial.print("[info][settings] Turns per hour: ");
  Serial.println(turnsPerHour);

  motorUpdateStatus(motorStatus);
  motorUpdateDirection(motorDirection);
  motorUpdateSpeed(motorSpeed);
}


void setup() {
  Serial.begin(115200);
  Serial.println("[info][system] Initializing all systems...");
  initMotor();
  loadSettings();
  initWiFi();
  initWebSocket();
  initDone();
  wsSendSettings();
}

void loop() {
  // Check if we are connected and there is data to read
  if (wifiClient.connected()) {
    webSocketClient.getData(webSocketData);
    if (webSocketData.length() > 0) {
      // Deserialize JSON
      deserializeJson(jsonWebSocket, webSocketData);
      String type    = jsonWebSocket["type"];
      String id      = jsonWebSocket["id"];
      String request = jsonWebSocket["request"];
      String action  = jsonWebSocket["action"];
      String setting = jsonWebSocket["setting"];
      String value   = jsonWebSocket["value"];
      // Print JSON
      if(type =="actiion") {
        wsOnMessage(webSocketData);
      }
      // Check if the type, action and setting
      if(type == "action") {
        // Update
        if(action == "update") {
          updateSetting(setting, value);
        }
        // Restart
        if(action == "restart" || action == "reboot") {
          sysRestart();
        }
        // Sleep
        if(action == "sleep") {
          sysSleep();
        }
      }
      else if(type == "request") {
        // Send settings
        if(request == "settings") {
          wsSendSettings();
        }
      }
    }
    webSocketData = "";
  } else {
    Serial.println("[error][websocket] Device disconnected from the WebSocket server!");
    delay(1000);
    Serial.println("[info][websocket] Reconnecting...");
    initWebSocket();
    delay(5000);
  }
  delay(500);
}