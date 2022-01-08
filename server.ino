#include <WiFi.h>

const char* ssid     = "Virus1392001";
const char* password = "Anukul1392001";


const char* resource = "/trigger/Attendance_Monitoring_System/with/key/e92YJD2HHNHlhtg6MvQ-90Ysv8yDjrPrcomUYwLZ5tT";

const char* server = "maker.ifttt.com";

int x = 0;
int y = 0;

void setup() {
  Serial.begin(115200); 
  pinMode(4,INPUT);
  pinMode(2,INPUT);
 
  initWifi();  
}

void loop() {
   Serial.println(digitalRead(2));
    
   x = digitalRead(4);
   y = digitalRead(2);
   
   if(x == 1) {
    makeIFTTTRequest("Prashant","1DS19ET059","Present");
    delay(1000);
   }

  else if(y == 1) {
    makeIFTTTRequest("Shiv","1DS19ET079","Present");
    delay(1000);
   }   
}


void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4;
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}


void makeIFTTTRequest(String d_name, String usn, String comment) {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);


  String jsonObject = String("{\"value1\":\"") + d_name + "\",\"value2\":\"" + usn + "\",\"value3\":\"" + comment + "\"}";
                      

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}
}
