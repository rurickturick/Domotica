#include <ESP8266WiFi.h> 
const char *ssid     = "Rurick";
const char *password = "MiKadO_= 141";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password); // nos conectamos al wifi

// Esperamos hasta que se establezca la conexión wifi
 while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "Intentando conectar al WIFI" );
  }
}

void loop() {
  
// Creamos el cliente
  WiFiClient client;
  const int httpPort = 80; // Puerto HTTP
  if (!client.connect("192.168.1.200", httpPort)) {
    // ¿hay algún error al conectar?
    Serial.println("Ha fallado la conexión");
    return;
  }
 
  // Creamos la URL para la petición
  String url = "/sensores.php?fecha=16/03/2022&hora=18:21&info=18.22&id_sensor=3";
 

 
  // Enviamos la petición
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: 192.168.1.200\r\n" + 
         "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Superado el tiempo de espera !");
      client.stop();
      return;
    }
  }
 
  // Consutar la memoria libre
  // Quedan un poco más de 40 kB
  Serial.printf("\nMemoria libre en el ESP8266: %d Bytes\n\n",ESP.getFreeHeap());
 
  // Leemos la respuesta y la enviamos al monitor serie
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("Cerrando la conexión");
 
  while(1){
    delay(0); // Siempre que hay un bucle que pueda durar mucho tiempo
              // hay que llamar a la función delay() para atender a los 
              // procesos de la conexión WiFi. Si no se hace el ESP8266
              // generará un error y se reiniciará a los pocos segundos
  }
}
