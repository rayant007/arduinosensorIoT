#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Configuración WiFi
const char* ssid = "iPhone de Bryant";
const char* password = "123456711";

// Configuración del bot de Telegram
const char* botToken = "6737245222:AAH8S5uR1JhXhOzO_XiFynQZdc5n5bBc8qM"; // Coloca aquí el token de tu bot

// Configuración del cliente WiFi y el bot
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

// Definir pines para el sensor PIR y el LED
const int pirPin = 13; // Pin GPIO para el sensor PIR
const int ledPin = 2;  // Pin GPIO para el LED

void setup() {
  // Iniciar comunicación serial
  Serial.begin(115200);

  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  // Configurar el pin del sensor PIR como entrada
  pinMode(pirPin, INPUT);

  // Conexión a la red WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configuración del cliente WiFi seguro
  client.setInsecure();
}

void loop() {
  // Leer el estado del sensor PIR
  int pirState = digitalRead(pirPin);

  if (pirState == HIGH) {
    // Movimiento detectado
    digitalWrite(ledPin, HIGH); // Encender LED
    Serial.println("Movimiento detectado!");

    // Enviar notificación a Telegram
    String chat_id = "7307070603"; // Coloca aquí tu ID de chat
    String message = "¡ALERTA MOVIMIENTO DETECTADO!";
    if (bot.sendMessage(chat_id, message, "")) {
      Serial.println("Mensaje enviado con éxito");
    } else {
      Serial.println("Error al enviar el mensaje");
    }

    // Esperar mientras el movimiento sigue siendo detectado
    while (digitalRead(pirPin) == HIGH) {
      delay(100);
    }
    // Apagar el LED cuando el movimiento cesa
    digitalWrite(ledPin, LOW);
  }

  delay(100); // Pequeña espera antes de la próxima lectura del sensor
}
