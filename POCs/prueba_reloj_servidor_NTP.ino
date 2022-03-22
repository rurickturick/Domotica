#include <NTPClient.h> //importamos la librería del cliente NTP
#include <ESP8266WiFi.h> //librería necesaria para la conexión wifi
#include <WiFiUdp.h> // importamos librería UDP para comunicar con 
                     // NTP

// datos para conectarnos a nuestra red wifi
const char *ssid     = ""; // la red a la que queremos conectar
const char *password = ""; // la contraseña de la red

//iniciamos el cliente udp para su uso con el server NTP
WiFiUDP ntpUDP;

// cuando creamos el cliente NTP podemos especificar el servidor al // que nos vamos a conectar en este caso
// 0.south-america.pool.ntp.org SudAmerica
// también podemos especificar el offset en segundos para que nos 
// muestre la hora según nuestra zona horaria en este caso
// restamos -10800 segundos ya que estoy en argentina. 
// y por ultimo especificamos el intervalo de actualización en
// mili segundos en este caso 6000

NTPClient timeClient(ntpUDP, "hora.roa.es",3600,6000);

void setup(){
  Serial.begin(9600); // activamos la conexión serial

  WiFi.begin(ssid, password); // nos conectamos al wifi

// Esperamos hasta que se establezca la conexión wifi
 while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "Intentando conectar al WIFI" );
  }

  timeClient.begin(); 
}

void loop() {
  timeClient.update(); //sincronizamos con el server NTP
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int day = ptm->tm_mday;
  int month=ptm->tm_mon+1;
  int year=ptm->tm_year+1900;
  String current_date= String(day)+"/"+String(month)+"/"+String(year)+" "+ timeClient.getFormattedTime();
  Serial.println(current_date);
//Imprimimos por puerto serie la hora actual

  delay(1000);
}
