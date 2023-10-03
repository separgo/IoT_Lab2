#include <WiFi.h>

const char * WIFI_SSID = "AndroidAPF707";
const char * WIFI_PASS = "12345678";
const char * SERVER_ADDRESS = "192.168.43.188";
const int SERVER_PORT = 10000; //YOUR_PORT

// Definición de los pines para el sensor ultrasónico y los LEDs
const int Trigger = 13;       // GPIO 13 para el Trigger del sensor ultrasónico
const int Echo = 12;          // GPIO 12 para el Echo del sensor ultrasónico
const int numLeds = 3;        // Número total de LEDs
int leds[numLeds] = {4, 5, 18};  // Pines GPIO de los LEDs

WiFiClient client;
void setup() {
  Serial.begin(9600);
  pinMode(Trigger, OUTPUT);  // Configuración del pin Trigger como salida
  pinMode(Echo, INPUT);      // Configuración del pin Echo como entrada
  
  // Configuración de los pines GPIO de los LEDs como salidas
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  } 
  digitalWrite(Trigger, LOW);  // Inicialización del pin Trigger en estado bajo
  
  Serial.print("Connecting to: ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to: ");
  Serial.println(SERVER_ADDRESS);

  if (!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    Serial.println("Connection failed");
    return;
  }
   unsigned long previousMillis = millis();
  while (!client.available() && millis() - previousMillis < 1000);
}

void loop() {

  long t;  // Variable para el tiempo de eco
  long d;  // Variable para la distancia medida
  digitalWrite(Trigger, HIGH);         // Generar pulso de activación del sensor
  delayMicroseconds(10);               // Esperar un corto tiempo
  digitalWrite(Trigger, LOW);          // Finalizar el pulso
  t = pulseIn(Echo, HIGH);             // Medir la duración del pulso de eco
  d = t / 59;                          // Convertir el tiempo en distancia en cm
  Serial.print("Distancia: ");
  Serial.print(d);
  Serial.print("cm");
  Serial.println();
  
  // Apagar todos los LEDs
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], LOW);
  }
  
  // Encender el LED correspondiente a la distancia medida
  for (int i = 0; i < numLeds; i++) {
    if (d <= (10 + 10 * i)) {
      digitalWrite(leds[i], HIGH);
      break;  // Romper el ciclo para encender solo un LED
    }
  } 
  delay(100);  // Esperar un breve periodo de tiempo antes de la siguiente medición
  String dat = String(d); //convertir la distancia en string para enviar
  client.print(dat.c_str());
  client.write(dat.c_str());//enviar con socket datos
    
  String line = client.readStringUntil('\n');//leer y mostrar los datos recibidos del servidor
    Serial.println(line);
}
