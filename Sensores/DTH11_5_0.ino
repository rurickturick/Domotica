#include "DHT.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definiciones
#define DHTTYPE DHT11
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define HORA_MIN 8
#define HORA_MAX 23
#define DHTPIN 14

// Credenciales WiFi (¡Cuidado con la información sensible!)
const char *ssid = "";
const char *password = "";

// NTP Cliente
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "minuto.roa.es", 7200, 6000); // GMT+1 (España), Horario invierno 3600 ó bien 7200 horario verano

// Pantalla OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Intervalo de lectura del sensor (5 minutos en milisegundos)
const unsigned long SENSOR_INTERVAL = 300000;
unsigned long lastSensorRead = 0;

// Dirección del servidor HTTP
const char *httpServer = "192.168.1.200";
const int httpPort = 80;

// Funciones de utilidad
void mostrarError(const String &mensaje);
bool obtenerDatosServidor(const String &url, String &respuesta);
bool parsearDatosPantalla(const String &data, String &onoff);
bool parsearDatosTodo(const String &data, String &fechah, String &horah, String &temph, String &humh, String &fechas, String &horas, String &temps, String &hums);
void actualizarPantalla(const String &fechah, const String &horah, const String &temph, const String &humh, const String &fechas, const String &horas, const String &temps, const String &hums);

void setup() {
  Serial.begin(115200); // Aumenté la velocidad del puerto serie para una mejor depuración
  Wire.begin();

  // Inicializar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error al inicializar la pantalla OLED"));
    while (true); // Detener si la pantalla no se inicializa
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println(F("Conectando a WiFi..."));
  display.display();

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("\nConectado a WiFi"));
  display.clearDisplay();
  display.println(F("WiFi Conectado"));
  display.display();
  delay(1000);

  dht.begin();
  timeClient.begin();
  Serial.println(F("NTP Cliente iniciado"));
}

void loop() {
  // Leer sensor DHT cada SENSOR_INTERVAL
  if (millis() - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Comprobar si la lectura del sensor fue exitosa
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Error al leer el sensor DHT!"));
    } else {
      timeClient.update();
      String currentDate;
      String currentTime = timeClient.getFormattedTime();

      time_t epochTime = timeClient.getEpochTime();
      struct tm *ptm = gmtime((time_t *)&epochTime);
      int day = ptm->tm_mday;
      int month = ptm->tm_mon + 1;
      int year = ptm->tm_year + 1900;
      currentDate = String(day) + "/" + String(month) + "/" + String(year);

      // Enviar datos de temperatura
      String urlTemp = "/sensores.php?fecha=" + currentDate + "&hora=" + currentTime + "&info=" + String(t) + "&id_sensor=3";
      String respuestaTemp;
      if (obtenerDatosServidor(urlTemp, respuestaTemp)) {
        Serial.print(F("Temperatura enviada. Respuesta: "));
        Serial.println(respuestaTemp);
      } else {
        Serial.println(F("Error al enviar la temperatura."));
      }

      // Enviar datos de humedad
      String urlHum = "/sensores.php?fecha=" + currentDate + "&hora=" + currentTime + "&info=" + String(h) + "&id_sensor=4";
      String respuestaHum;
      if (obtenerDatosServidor(urlHum, respuestaHum)) {
        Serial.print(F("Humedad enviada. Respuesta: "));
        Serial.println(respuestaHum);
      } else {
        Serial.println(F("Error al enviar la humedad."));
      }
    }
  }

  // Obtener estado de la pantalla
  String urlPantalla = "/resJa.php?accion=5";
  String respuestaPantalla;
  String onoff = "";
  if (obtenerDatosServidor(urlPantalla, respuestaPantalla)) {
    if (parsearDatosPantalla(respuestaPantalla, onoff)) {
      Serial.print(F("Estado pantalla (onoff): "));
      Serial.println(onoff);

      timeClient.update();
      int horaActual = timeClient.getHours();
      int horaMin = (timeClient.getHours() == 0 || timeClient.getHours() == 1) ? 8 : HORA_MIN;
      int horaMax = (timeClient.getHours() == 0 || timeClient.getHours() == 1) ? 23 : HORA_MAX;
      int horaAc = (timeClient.getHours() == 0 || timeClient.getHours() == 1) ? 17 : horaActual;

      if (horaMin <= horaAc && horaAc <= horaMax && onoff == "1") {
        // Obtener todos los datos
        String urlTodo = "/resJa.php?accion=6";
        String respuestaTodo;
        String fechah, horah, temph, humh, fechas, horas, temps, hums;

        if (obtenerDatosServidor(urlTodo, respuestaTodo)) {
          if (parsearDatosTodo(respuestaTodo, fechah, horah, temph, humh, fechas, horas, temps, hums)) {
            actualizarPantalla(fechah, horah, temph, humh, fechas, horas, temps, hums);
            display.startscrollleft(0x00, 0x0F);
            delay(60000); // 1 minutos de visualización
          } else {
            mostrarError(F("Error al parsear los datos 'todo'."));
            delay(60000);
          }
        } else {
          mostrarError(F("Error al obtener los datos 'todo' del servidor."));
          delay(60000);
        }
      } else {
        display.clearDisplay();
        display.display();
        display.startscrollleft(0x00, 0x0F);
        delay(60000); // 1 minuto con la pantalla en blanco
      }
    } else {
      mostrarError(F("Error al parsear el estado de la pantalla."));
      delay(60000);
    }
  } else {
    mostrarError(F("Error al obtener el estado de la pantalla del servidor."));
    delay(60000);
  }

  delay(10); // Pequeño delay para no saturar el procesador
}

// Función para mostrar un mensaje de error en el puerto serie y la pantalla
void mostrarError(const String &mensaje) {
  Serial.print(F("ERROR: "));
  Serial.println(mensaje);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Error:"));
  display.println(mensaje);
  display.display();
}

// Función para realizar una petición GET al servidor y obtener la respuesta
bool obtenerDatosServidor(const String &url, String &respuesta) {
  WiFiClient client;
  respuesta = "";
  if (!client.connect(httpServer, httpPort)) {
    Serial.println(F("Error al conectar al servidor HTTP"));
    return false;
  }

  Serial.print(F("Solicitando: "));
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + httpServer + "\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0 /*&& millis() - timeout < 5000*/) { // Esperar hasta 5 segundos por la respuesta
    delay(10);
  }

  if (client.available()) {
    respuesta = client.readString();
    // Leer el resto de la respuesta para evitar problemas con la siguiente conexión
    while (client.available()) {
      client.read();
    }
    client.stop();
    return true;
  } else {
    Serial.println(F("No se recibió respuesta del servidor o se agotó el tiempo de espera."));
    client.stop();
    return false;
  }
}

// Función para parsear la respuesta del estado de la pantalla
bool parsearDatosPantalla(const String &data, String &onoff) {
  int bodyIndex = data.indexOf("BODY:,");
  if (bodyIndex != -1) {
    int startIndex = data.indexOf(":,", bodyIndex);
    if (startIndex != -1) {
      startIndex += 2;
      int endIndex = data.indexOf(",", startIndex);
      if (endIndex != -1) {
        onoff = data.substring(startIndex, endIndex);
        onoff.trim();
        return true;
      } else {
        Serial.println(F("Error al encontrar el delimitador ',' para 'onoff'"));
        return false;
      }
    } else {
      Serial.println(F("Error al encontrar ':, ' después de 'BODY:,'"));
      return false;
    }
  } else {
    Serial.println(F("Error al encontrar 'BODY:,' en la respuesta de la pantalla."));
    return false;
  }
}

// Función para parsear la respuesta de todos los datos
bool parsearDatosTodo(const String &data, String &fechah, String &horah, String &temph, String &humh, String &fechas, String &horas, String &temps, String &hums) {
  int bodyIndex = data.indexOf("BODY:,");
  if (bodyIndex != -1) {
    int startIndex = data.indexOf(":,", bodyIndex);
    if (startIndex != -1) {
      startIndex += 2;
      // Extraer los valores usando indexOf y substring
      int currentIndex = startIndex;
      int nextComma;

      auto extraerValor = [&](String &destino) {
        nextComma = data.indexOf(",", currentIndex);
        if (nextComma != -1) {
          destino = data.substring(currentIndex, nextComma);
          destino.trim();
          currentIndex = nextComma + 1;
          return true;
        } else {
          Serial.println(F("Error al parsear los datos 'todo': delimitador ',' faltante."));
          return false;
        }
      };

      if (extraerValor(fechah) &&
          extraerValor(horah) &&
          extraerValor(temph) &&
          extraerValor(humh) &&
          extraerValor(fechas) &&
          extraerValor(horas) &&
          extraerValor(temps) &&
          extraerValor(hums)) {
        return true;
      }
    } else {
      Serial.println(F("Error al encontrar ':, ' después de 'BODY:,' en los datos 'todo'."));
      return false;
    }
  } else {
    Serial.println(F("Error al encontrar 'BODY:,' en la respuesta de los datos 'todo'."));
    return false;
  }
  return false;
}

// Función para actualizar la pantalla OLED
void actualizarPantalla(const String &fechah, const String &horah, const String &temph, const String &humh, const String &fechas, const String &horas, const String &temps, const String &hums) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.printf(" Hab       %s\n", fechah.c_str());
  display.printf("     %s\n", horah.c_str());
  display.printf("T: %s C  H: %s %% \n", temph.c_str(), humh.c_str());
  display.println();
  display.println();
  display.printf(" Salon     %s\n", fechas.c_str());
  display.printf("     %s\n", horas.c_str());
  display.printf("T: %s C  H: %s %% \n", temps.c_str(), hums.c_str());
  display.display();
}