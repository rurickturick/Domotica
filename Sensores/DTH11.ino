#include "DHT.h"
#define DHTPIN 5
#include <NTPClient.h> 
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h>
#define DHTTYPE DHT11  

const char *ssid     = ""; // la red a la que queremos conectar
const char *password = ""; // la contraseña de la red
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "hora.roa.es",3600,6000);

DHT dht(DHTPIN, DHTTYPE);
void setup() {
Serial.begin(9600);

WiFi.begin(ssid, password); // nos conectamos al wifi

// Esperamos hasta que se establezca la conexión wifi
 while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
  }
dht.begin();
timeClient.begin();
}

void loop() {

delay(2000); //Es un sensor lento, por lo que hay que darle tiempo.
float h = dht.readHumidity();
float t = dht.readTemperature();
WiFiClient client;
const int httpPort = 80; // Puerto HTTP
  if (!client.connect("192.168.1.200", httpPort)) {
    // ¿hay algún error al conectar?

  }
  else {
if (isnan(h) || isnan(t)) {

}
else{
timeClient.update(); //sincronizamos con el server NTP
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int day = ptm->tm_mday;
  int month=ptm->tm_mon+1;
  int year=ptm->tm_year+1900;
  String current_date= String(day)+"/"+String(month)+"/"+String(year);
  String urlt = "/sensores.php?fecha="+current_date+"&hora="+timeClient.getFormattedTime()+"&info="+String(t)+"&id_sensor=3";
  String urlh = "/sensores.php?fecha="+current_date+"&hora="+timeClient.getFormattedTime()+"&info="+String(h)+"&id_sensor=4";
  client.print(String("GET ") + urlt + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n"); 
   delay(1000);
  client.print(String("GET ") + urlh + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
     }
   } 
   delay(300000);
   
   }
  }
}
