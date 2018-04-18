#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>

static const char ssid[] = "iPhone de Jason";
static const char password[] = "cacpou1big302";

const char up[]= "up"; 
const char left[]= "left"; 
const char right[]= "right"; 
const char down[]= "down"; 
const char action[]= "action"; 

const int buttonUp = 15;
const int buttonLeft = 13;
const int buttonRight = 12;
const int buttonDown = 14;
const int buttonAction = 16;

int buttonUpState = 0;
int buttonLeftState = 0;
int buttonRightState = 0;
int buttonDownState = 0;
int buttonActionState = 0;

ESP8266WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);

/*
 * This function triggers an action depending on websocket event
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected!");
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    default:
      Serial.printf("Invalid WStype", type);
      break;
  }
}

void setup(){
  Serial.begin(115200);

  pinMode(buttonUp, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonAction, INPUT);

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\r\n", t);
    Serial.flush();
    delay(1000);
  }

  
  // Connection to WiFi
  WiFiMulti.addAP(ssid, password);

  while(WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  // We start the websocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop(){

  // We loop the websocket
  webSocket.loop();

  // We read the state of each button 
  buttonUpState = digitalRead(buttonUp);
  buttonLeftState = digitalRead(buttonLeft);
  buttonRightState = digitalRead(buttonRight);
  buttonDownState = digitalRead(buttonDown);
  buttonActionState = digitalRead(buttonAction);

  // If a button is pressed, a message is triggered
  if (buttonUpState == HIGH) {
     webSocket.broadcastTXT(up,strlen(up));
  } 
  else if(buttonLeftState == HIGH){
     webSocket.broadcastTXT(left,strlen(left));
  }
  else if(buttonRightState == HIGH){
     webSocket.broadcastTXT(right,strlen(right));
  }
  else if(buttonDownState == HIGH){
     webSocket.broadcastTXT(down,strlen(down));
  }
  else if(buttonActionState == HIGH){
     webSocket.broadcastTXT(action,strlen(action));
  }
}
