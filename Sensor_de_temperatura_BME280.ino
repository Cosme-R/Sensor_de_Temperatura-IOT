
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define SDA_PIN 0  // GPIO0
#define SCL_PIN 2  // GPIO2

// Defina suas credenciais Wi-Fi e as chaves do canal ThingSpeak
const char* ssid = "teste"; // nome da sua rede  wifi
const char* password = "teste1234"; // senha da sua rede wifi
const char* apiKey = "XXXXXXXX"; //  chave de gravação do ThingSpeak
const unsigned long channelID = 11111111;// Seu ID no ThingSpeak

Adafruit_BME280 bme;
WiFiClient client; // Objeto WiFiClient para a conexão Wi-Fi

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);

  // Inicialize o sensor BME280
  if (!bme.begin(0x76)) {
    Serial.println("Não foi possível encontrar o sensor BME280, verifique a conexão!");
    while (1);
  }

  // Conecta na rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  // Inicialize a conexão com o ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // Convert Pa to hPa

  // Envie os dados para o ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, pressure);

  int httpStatus = ThingSpeak.writeFields(channelID, apiKey);
  if (httpStatus == 200) {
    Serial.println("Dados enviados com sucesso para o ThingSpeak!");
  } else {
    Serial.println("Falha ao enviar dados para o ThingSpeak. Código de status HTTP: " + String(httpStatus));
  }

  delay(15000); // Envia dados a cada 15 segundos
}
