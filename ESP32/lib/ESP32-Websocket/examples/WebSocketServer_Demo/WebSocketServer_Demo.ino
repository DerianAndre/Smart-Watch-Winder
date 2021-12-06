#include <WiFi.h>
#include <WebSocketServer.h>


#define WIFI_SSID "xxxxxxxxxxx" // change with your own wifi ssid
#define WIFI_PASS "1234" // change with your own wifi password

#define PORT 80

WiFiServer server(PORT);
WebSocketServer webSocketServer;

String dataToSend; // update this with the value you wish to send to your client

void setup()
{
  Serial.begin(115200);

  // try to connect
  if(connect() == 0) { return ; }

}

void loop() {
 webSocketLoop(); 
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

void webSocketLoop()
{
  Serial.println("Starting the server");
  server.begin();
  delay(100);

  Serial.println("server running");
  while(true)
  {
    WiFiClient client = server.available();
    
    if (client.connected() && webSocketServer.handshake(client)) 
    {
      String data;      
      while (client.connected()) 
      {
        data = webSocketServer.getData();
        if (data.length() > 0) 
        {
          onDataReceived(data);
        }
        
        delay(10); // Delay needed for receiving the data correctly
        data = "";
        
        if(dataToSend.length() > 0)
        {
          webSocketServer.sendData(dataToSend);
         
        }
        delay(10); // Delay needed for sending the data correctly
        dataToSend = "";
      }
 
      Serial.println("The client disconnected");
      delay(100);
    }
    
    delay(100);
  }
}

void onDataReceived(String &data)
{
  Serial.println(data);

  if(data == "HELLO")
  {
    dataToSend = "Hello to you too...";
  }
  else if(data == "BYE")
  {
    dataToSend = "I am sad to see you leave";
  }
  else if(data == "WAZZUP")
  {
    dataToSend = "Life socks!";
  }
}
