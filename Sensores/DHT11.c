//#########################################
//# Programa que lee la temperatura y la
//# humedad mediante un sensor DHT11 en
//# una Raspberry
//#
//#Autor: Ángel Luis Ortiz Folgado
//#      10/12/2021
//########################################

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXTIMINGS  85
#define DHTPIN      5
int socket_desc;
struct sockaddr_in server;
char *message, server_reply[2000];
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

int read_dht11_dat()
{
    uint8_t laststate   = HIGH;
    uint8_t counter     = 0;
    uint8_t j       = 0, i;
    float   f; 

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );

    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ( (j >= 40) &&
         (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        return 0;
    }else  {
        return 1;
    }
}

int main( void )
{
    if ( wiringPiSetup() == -1 )
        exit( 1 );
     time_t now =time(NULL);
     struct tm *tm;
     char fecha[100];
     char hora[100];
     char info_temp[5];
     char info_hum[5];
     int  sensor_te = 1;
     int  sensor_h = 2;
     char message_temp[200];
     char message_hum[200];
     

   server.sin_addr.s_addr = inet_addr ("192.168.1.200");
   server.sin_family = AF_INET;
   server.sin_port =htons (80);

    while ( 1 )
    {

    //Create a socket
   socket_desc = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_desc == -1) {
      printf("Couldn't create a socket");
   }
    //Connect to remote sever
         if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) <0){
          puts("Conect Failed");
          close(socket_desc);
          return 1;
         }
        
        if( read_dht11_dat() == 0){
           //generar variables fecha, hora, info, sensor
           now = time(NULL);
           tm=localtime(&now);
           strftime(fecha,100,"%d/%m/%Y",tm);
           strftime(hora,100,"%X",tm);
           sprintf(info_temp,"%d.%d",dht11_dat[2], dht11_dat[3]);
           sprintf(info_hum,"%d.%d",dht11_dat[0], dht11_dat[1]);

           //Send some Data
          sprintf(message_temp, "GET /sensores.php?fecha=%s&hora=%s&info=%s&id_sensor=%d HTTP/1.1\r\nHost: 192.168.1.200\r\n\r\n",fecha, hora, info_temp, sensor_te);
          sprintf(message_hum, "GET /sensores.php?fecha=%s&hora=%s&info=%s&id_sensor=%d HTTP/1.1\r\nHost: 192.168.1.200\r\n\r\n",fecha, hora, info_hum, sensor_h);


          // envio socket temperatura
       if (send(socket_desc, message_temp, strlen (message_temp), 0) < 0 ) {
           puts("Send failed");
           close(socket_desc);
           return 1;
         }
         
           // Receive a reply from the server
          if (recv(socket_desc, server_reply, 2000, 0) < 0) {
            puts("recv failed");
            close(socket_desc);
            return 1;
          }

         
         // envio socket humedad
        if (send(socket_desc, message_hum, strlen (message_hum), 0) < 0 ) {
           puts("Send failed");
           close(socket_desc);
           return 1;
         }
         
           // Receive a reply from the server
          if (recv(socket_desc, server_reply, 2000, 0) < 0) {
            puts("recv failed");
            close(socket_desc);
            return 1;
          }
 
           close(socket_desc);
         return(0);
         }
        else delay(1000);
    }

    return(0);
}
