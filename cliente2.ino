#include <WiFi.h>

const char * WIFI_SSID = "AndroidAPF707";
const char * WIFI_PASS = "12345678"; //estableciendo datos de la red wifi
const char * SERVER_ADDRESS = "192.168.43.188"; //Establecer Ip del servidor
const int SERVER_PORT = 10000; //YOUR_PORT

// Definición de los pines para el sensor ultrasónico y los LEDs
int led = 4;  // Pines GPIO de los LEDs

WiFiClient client;
void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.print("Connecting to: ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);//Conectandose a wifi
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to: ");//conectandose al servidor
  Serial.println(SERVER_ADDRESS);

  if (!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    Serial.println("Connection failed");
    return;
  }
   unsigned long previousMillis = millis();
  while (!client.available() && millis() - previousMillis < 1000);
}

void loop() { 

  
  String line = client.readStringUntil('\n');//leer datos del servidor
  Serial.println(line);
    // Encender el LED correspondiente en funcion al servidor
    if (line == "ON") {
      digitalWrite(led, HIGH);
      digitalWrite(5, LOW);
    }else if(line == "OFF"){
      digitalWrite(led, LOW);
      digitalWrite(5, HIGH);
      }
    
}
