#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "Vivo 1902";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "123456789";     // The password of the Wi-Fi network

const int read = A0; //Sensor AO pin to Arduino pin A0
int value;   //Variable to store the incomming data

int threshold = 600;
void setup()
{
  delay(1000);
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to "); 
  Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('...');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
  
}

void loop()
{
  value = analogRead(read);//Read data from analog pin and store it to value variable
  
  if (value > threshold){ 
    Serial.println("Water level: Above Threshold! (> 20 mm)");
    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
      HTTPClient http;  //Declare an object of class HTTPClient
      String Sendvalue, postData;
      Sendvalue = value;
      postData = "value1=" + Sendvalue;
      http.begin("http://maker.ifttt.com/trigger/fill_liquid/with/key/fmGuasvRynyPjLIV60Rs8CKS1VWsB67nbMq5HY_pdpC");  //Specify request destination
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

      int httpCode = http.POST(postData);   //Send the request
      String payload = http.getString();    //Get the response payload

      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload

      http.end();  //Close connection
 
    } else {
        Serial.println("Wifi not connected");
      }
  }
  else { 
    Serial.println("Water level: Ok"); 
  }
  
  delay(3000); // Check for new value every 5 sec
}
