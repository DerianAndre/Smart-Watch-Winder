#include <WiFi.h>
#include <WebSocketClient.h>


#define WIFI_SSID "xxxxxxxxxxx" // change with your own wifi ssid
#define WIFI_PASS "1234" // change with your own wifi password

// Server infos
#define HOST "demos.kaazing.com"
#define PORT 80
#define PATH "/echo"


WebSocketClient webSocketClient;
WiFiClient client;


String dataToSend; // update this with the value you wish to send to the server

void setup()
{
  Serial.begin(115200);

  // try to connect to the wifi
  if(connect() == 0) { return ; }

  // once connected to the wifi, let's reach our server
  if(initWebSocket() == 0) { return ; }

}

void loop() {
  
  String data;
 
  if (client.connected()) {
 
    webSocketClient.sendData("Info to be echoed back");
 
    webSocketClient.getData(data);
    if (data.length() > 0) {
      onDataReceived(data);
    }
    data = "";

    if(dataToSend.length() > 0)
    {
      webSocketClient.sendData(dataToSend);
    }
    dataToSend = "";
 
  } else {
    Serial.println("Client disconnected.");
  }
 
  delay(3000);
 
}

void onDataReceived(String &data)
{
  Serial.println(data);

  if(data == "Info to be echoed back")
  {
    dataToSend = "Info to be echoed back";
  }
}

char initWebSocket()
{
  if(!client.connect(HOST, PORT)) {
    Serial.println("Connection failed.");
    return 0;
  }

  Serial.println("Connected.");
  webSocketClient.path = PATH;
  webSocketClient.host = HOST;

  if (!webSocketClient.handshake(client)) {
    Serial.println("Handshake failed.");
    return 0;
  }
  Serial.println("Handshake successful");
  return 1;
}

/*
  * Connect to the wifi (credential harcoded in the defines)
  */ 
char connect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS); 

  Serial.println("Waiting for wifi");
  int timeout_s = 30;
  while (WiFi.status() != WL_CONNECTED && timeout_s-- > 0) {
      delay(1000);
      Serial.print(".");
  }
  
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("unable to connect, check your credentials");
    return 0;
  }
  else
  {
    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());
    return 1;
  }
}