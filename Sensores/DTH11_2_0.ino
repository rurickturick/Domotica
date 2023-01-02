#include "DHT.h"
#define DHTPIN 14
#include <NTPClient.h> 
#include <ESP8266WiFi.h> 
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define DHTTYPE DHT11 
#define SCREEN_WIDTH 128 // ancho pantalla OLED
#define SCREEN_HEIGHT 64 // alto pantalla OLED
#define hora_min 8
#define hora_max 23 

const char *ssid     = "";
const char *password = "";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "hora.roa.es",3600,6000);//3600 ó bien 7200
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);
unsigned long sensor = 0;
void setup() {
Serial.begin(9600);
Wire.begin(); 
WiFi.begin(ssid, password); // nos conectamos al wifi
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
// Esperamos hasta que se establezca la conexión wifi
 while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
  }
dht.begin();
timeClient.begin();
}

void loop() {
char  *buffer, *fechas, *horas, *fechah, *horah, *temps, *temph, *hums, *humh, *onoff  ;
 String urlth = "/resJa.php?accion=0";
 String urlhh = "/resJa.php?accion=1";
 String urlts = "/resJa.php?accion=2";
 String urlhs = "/resJa.php?accion=3";
 String pantalla = "/resJa.php?accion=5";
 String lineth;
 String linehh;
 String linets;
 String linehs;
 String linepantalla;
 int hora_mn, hora_mx, hora_ac; 
 
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
  if (sensor == 0 || millis() - sensor >= 300000 ) {
   sensor=millis(); 
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
   }

    client.print(String("GET ") + pantalla + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n"); 
   delay(1000);

unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
     }
  } 

 // Leemos la respuesta y la enviamos al monitor serie 
while(client.available()){ 
  // Mientras quedan datos por leer 
  linepantalla = client.readString();
}

// Recuperar los valores 
// onoff
buffer =new char [linepantalla.length()];
strcpy(buffer,linepantalla.c_str());
if (strstr(buffer, "BODY:,")) {
  buffer=strstr(buffer, "BODY:,");
  buffer=strstr(buffer, ":,");
  buffer[strcspn(buffer, "\n")]=0;
  strtok(buffer,",");
  onoff = strtok(NULL, ",");
}
timeClient.update(); //sincronizamos con el server NTP     
if(timeClient.getHours() == 0 || timeClient.getHours() == 1 ) {
  hora_mn = 8;
  hora_mx = 23;
  hora_ac =17;  
}
else {
  hora_mn = hora_min;
  hora_mx = hora_max;
  hora_ac = timeClient.getHours();   
}
if (hora_mn <= hora_ac && hora_ac <= hora_mx && strcmp("1",onoff)== 0){
// if (strcmp("1", onoff) == 0){
 client.print(String("GET ") + urlth + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n"); 
   delay(1000);

  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
     }
  } 

 // Leemos la respuesta y la enviamos al monitor serie 
while(client.available()){ 
  // Mientras quedan datos por leer 
  lineth = client.readString();
}

  
  client.print(String("GET ") + urlhh + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n");
      delay(1000); 

timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
     }
  } 

 // Leemos la respuesta y la enviamos al monitor serie 
while(client.available()){ 
  // Mientras quedan datos por leer 
  linehh = client.readString();
}
             
   client.print(String("GET ") + urlts + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n");         
     delay(1000);

timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
     }
  } 

 // Leemos la respuesta y la enviamos al monitor serie 
while(client.available()){ 
  // Mientras quedan datos por leer 
  linets = client.readString();
}

   client.print(String("GET ") + urlhs + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n"); 
   delay(1000);

timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
     }
  } 

 // Leemos la respuesta y la enviamos al monitor serie 
while(client.available()){ 
  // Mientras quedan datos por leer 
  linehs = client.readString(); 
}
   
   client.print(String("GET ") + pantalla + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n\r\n"); 
   delay(1000);
     
timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
     }
  } 

 // Leemos la respuesta y la enviamos al monitor serie 
while(client.available()){ 
  // Mientras quedan datos por leer 
  linepantalla = client.readString();
}

// Recuperar los valores
// datos habitacion 
// humedad
buffer =new char [linehh.length()];
strcpy(buffer,linehh.c_str());
if (strstr(buffer, "BODY:,")) {
  buffer=strstr(buffer, "BODY:,");
  buffer=strstr(buffer, ":,");
  buffer[strcspn(buffer, "\n")]=0;
  strtok(buffer,",");
  fechah = strtok(NULL, ",");
  horah = strtok(NULL, ",");
  humh = strtok(NULL, ",");
}
 // temperatura 
buffer =new char [lineth.length()];
strcpy(buffer,lineth.c_str());
if (strstr(buffer, "BODY:,")) {
  buffer=strstr(buffer, "BODY:,");
  buffer=strstr(buffer, ":,");
  buffer[strcspn(buffer, "\n")]=0;
  strtok(buffer,",");
  fechah = strtok(NULL, ",");
  horah = strtok(NULL, ",");
  temph = strtok(NULL, ",");
}
  // datos salon 
// humedad
buffer =new char [linehs.length()];
strcpy(buffer,linehs.c_str());
if (strstr(buffer, "BODY:,")) {
  buffer=strstr(buffer, "BODY:,");
  buffer=strstr(buffer, ":,");
  buffer[strcspn(buffer, "\n")]=0;
  strtok(buffer,",");
  fechas = strtok(NULL, ",");
  horas = strtok(NULL, ",");
  hums = strtok(NULL, ",");
}
 // temperatura 
buffer =new char [linets.length()];
strcpy(buffer,linets.c_str());
if (strstr(buffer, "BODY:,")) {
  buffer=strstr(buffer, "BODY:,");
  buffer=strstr(buffer, ":,");
  buffer[strcspn(buffer, "\n")]=0;
  strtok(buffer,",");
  fechas = strtok(NULL, ",");
  horas = strtok(NULL, ",");
  temps = strtok(NULL, ","); 
}
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.printf("Hab        %s\n",fechah);
display.printf("     %s\n",horah);
display.printf("T: %s C  H: %s %% \n",temph,humh);
display.printf("\n");
display.printf("\n");
display.printf("Salon      %s\n",fechas);
display.printf("     %s\n",horas);
display.printf("T: %s C  H: %s %% \n",temps,hums);
display.display();
delay (39000);
}
else {
 display.clearDisplay();
 display.display();
 delay (60000);
 }   
  } 
 }
}
