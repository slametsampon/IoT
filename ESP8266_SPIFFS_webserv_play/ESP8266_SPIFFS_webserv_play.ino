#define onboard_led 2 

const char* ssid     = "Your-SSID"; 
const char* password = "Your-Password"; 

#include <ESP8266WebServer.h>
ESP8266WebServer server (80);

#include "FS.h" //file system

#define TABLE_WIDTH 3
#define TABLE_HEIGHT 10
long data[TABLE_WIDTH][TABLE_HEIGHT] =  {};

void UpdateRandomData()
{
  for (byte i=0; i<TABLE_HEIGHT; i++) 
  for (byte j=0; j<TABLE_WIDTH;  j++) 
    {
     switch (j)
          {
          case 0:  data[j][i] = i+1; break; 
          case 1:  data[j][i] = random(-100, 100); break;
          default: data[j][i] = random(100, 999); break;
          }
    }
}

void setup() 
{
Serial.begin(115200);
delay(10);
//
pinMode(onboard_led, OUTPUT ); //led
digitalWrite(onboard_led, HIGH); //off
//wifi
WiFi.begin(ssid, password);
Serial.println();
while (WiFi.status() != WL_CONNECTED) 
  {
  delay(100);
  Serial.print(".");
  digitalWrite(onboard_led, !digitalRead(onboard_led) ); //led invert
  }
Serial.print("\nWiFi connected with ip: ");  
Serial.println(WiFi.localIP());
//web-server
server.on ("/", handleRoot);
server.onNotFound (handleNotFound);
server.on("/CombineStringInCode", handleCombineStringInCode);
server.on("/ReadSPIFFSServerReplace", handleReadSPIFFSServerReplace);
server.on("/ReadSPIFFSClientReplaceJS", handleReadSPIFFSClientReplaceJS);
server.on("/favicon.ico", handle_favicon);
server.begin();
Serial.println ("HTTP server started");
SPIFFS.begin();
}

void loop() 
{
  server.handleClient(); //check httpserver
}

void handleRoot()
{
  Serial.print("handleRoot() ");
  digitalWrite(onboard_led, LOW); //on
  long _a = millis();  

  if (SPIFFS.exists("/index.html")) 
    {// If the file exists
      File file = SPIFFS.open("/index.html", "r");
      size_t sent = server.streamFile(file, "text/html");
      file.close();
    }
     else server.send(404, "text/plain", "404: File NOT FOUND!");
  
  long _b = millis() - _a;
  digitalWrite(onboard_led, HIGH); //off
  Serial.printf("\tTakes %dmS\n", _b);
}

void handleNotFound()
{
  Serial.print("handleNotFound() ");
  Serial.print( server.uri() );
  digitalWrite(onboard_led, LOW); //on
 
  long _a = millis();  
  server.send(200, "text/plain", "handleNotFound");
  long _b = millis() - _a;  
  digitalWrite(onboard_led, HIGH); //off
  Serial.printf("\tTakes %dmS\n", _b);
}

void handleCombineStringInCode()
{
  Serial.print("handleCombineStringInCode() ");
  UpdateRandomData();
  digitalWrite(onboard_led, LOW); //on
  long _a = millis();  

  String http_resp = "<html>\n <head>\n <title> ESP8266 #2 </title>\n</head>\n <body>\n";
  http_resp = http_resp + "Combine page in code as String.\n";
  http_resp = http_resp + "<table border='1'>\n <tr>\n <th BGCOLOR='#e0e0e0'>Row#</th>\n"; 
  http_resp = http_resp + " <th BGCOLOR='#e0e0e0'>Header 2</th>\n"; 
  http_resp = http_resp + " <th BGCOLOR='#e0e0e0'>Header 3</th>\n </tr>\n";
  ///////////////////
  for (byte i=0; i<TABLE_HEIGHT; i++) 
    {
    http_resp=http_resp+ "<tr>\n";
    for (byte j=0; j<TABLE_WIDTH; j++) 
      {
      http_resp=http_resp+ " <td align='center'>" + String(data[j][i]) + "</td>\n"; 
      }
    http_resp=http_resp+ "</tr>\n"; 
    };
  http_resp=http_resp+ "</table>\n";
  http_resp=http_resp+ "</body>\n</html>\n";
  
  server.send(200, "text/html", http_resp ); 
  
  long _b = millis() - _a; 
  digitalWrite(onboard_led, HIGH); //off 
  Serial.printf("\tTakes %dmS\n", _b);
}


void handleReadSPIFFSServerReplace()
{
  Serial.print("handleReadSPIFFSServerReplace() ");
  UpdateRandomData();
  digitalWrite(onboard_led, LOW); //on
  long _a = millis();  
  
  if (SPIFFS.exists("/page1.html")) 
    {// If the file exists
      File file = SPIFFS.open("/page1.html", "r");   
      //size_t sent = server.streamFile(file, "text/html");

      String Str1;
      String s;
      Str1 = file.readString();
      
      //  replacements
      for (byte i=0; i<TABLE_HEIGHT; i++) 
      for (byte j=0; j<TABLE_WIDTH; j++) 
       {
        s=String( "%DATA" + String(i+1) + "_" + String(j+1) + "%" ); //combine template
        Str1.replace(s, String( data[j][i] ) );  //replace
       }
      
      server.send(200, "text/html", Str1 ); 
      file.close();
    }
     else server.send(404, "text/plain", "404: File NOT FOUND!");
  
  long _b = millis() - _a;  
  digitalWrite(onboard_led, HIGH); //off
  Serial.printf("\tTakes %dmS\n", _b);
}

void handleReadSPIFFSClientReplaceJS()
{
  Serial.print("handleReadSPIFFSClientReplaceJS() ");
  UpdateRandomData();
  digitalWrite(onboard_led, LOW); //on
  long _a = millis();  
  
  if (SPIFFS.exists("/page2-JS.html")) 
    {// If the file exists
      File file = SPIFFS.open("/page2-JS.html", "r"); 
      String Str1 = file.readString();
      // prepare data string  for replacement
      String s = "[";
      for (byte i=0; i<TABLE_HEIGHT; i++) 
        {
        s = s + "[";
        for (byte j=0; j<TABLE_WIDTH; j++) 
         {
         s = s + String(data[j][i]) + ",";   //combine data string
         }
        s = s + "],";
        } 
      s = s + "]";  
      Str1.replace("%DATA%", s );  //replace
      server.send(200, "text/html", Str1 ); 
      file.close();
    }
     else server.send(404, "text/plain", "404: File NOT FOUND!");
  
  long _b = millis() - _a;
  digitalWrite(onboard_led, HIGH); //off
  Serial.printf("\tTakes %dmS\n", _b);
}

void handle_favicon()
{
  /*  takes long time, removed for demo.
  Serial.print("handle favicon.ico");
  long _a = millis();  
  
  if (SPIFFS.exists("/favicon.ico")) 
    {// If the file exists
      File file = SPIFFS.open("/favicon.ico", "r");                 // Open it
      size_t sent = server.streamFile(file, "image/x-icon"); // And send it to the client
      file.close();                                       // Then close the file again

    }
     else server.send(404, "text/plain", "404: File NOT FOUND!"); // otherwise, respond with a 404 (Not Found) error
    
  long _b = millis() - _a;  
  Serial.printf("\tTakes %dmS\n", _b);
  */
}

  


    
