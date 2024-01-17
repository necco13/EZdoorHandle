#include <WiFi.h>
#include <HTTPClient.h>

#include <Keypad.h>

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


const char* ssid = "Prou";
const char* password = "Geslo1234567890";

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.1.106:1880/update-sensor";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

bool check(String pass){
if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = "https://ezdoor.azurewebsites.net/api/token/"+pass;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        String payload = http.getString();
        payload.replace("}","");
        payload = payload.substring(payload.indexOf(":")+1,payload.length());
        if(payload.equals("true"))
          return true;
        else
          return false;
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    return false;
}

void setup() {
  Serial.begin(115200); 
  pinMode(12, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

String input = "";

void loop() {
  char key = keypad.getKey();

  if (key) {
    if(key=='#'){
      if(check(input)){
        digitalWrite(12, HIGH);
        delay(5000);
        digitalWrite(12, LOW);
      }
      input="";
    }
    else
      input+=key;
  }
}
