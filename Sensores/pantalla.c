//#########################################
//# Programa que muestra en una pantalla
//# en una pantalla Oled ssd1306,conectada 
//# a una Raspberry, los datos de temperatura
//# y humedad obtenidos de unos sensores DHT11
//# y almacenados en una base de datos
//#Autor: Ángel Luis Ortiz Folgado
//#      10/06/2022
//########################################
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "ssd1306_i2c.h"
#include <time.h>
#define  hora_min 8
#define  hora_max 23
int recuperar_datos(int accion, char *server_reply  ) {
  int socket_desc;
  struct sockaddr_in server;
  char message [100];

  //Create a socket
   socket_desc =socket(AF_INET, SOCK_STREAM, 0);
   if (socket_desc == -1) {
      printf("Couldn't create a socket");
   }

   server.sin_addr.s_addr = inet_addr ("192.168.1.200");
   server.sin_family = AF_INET;
   server.sin_port =htons (80);


  //Connect to remote sever

  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) <0){
    puts("connect error");
    close(socket_desc);
    return 1;
  }



  //Send some Data
  sprintf(message, "GET /resJa.php?accion=%i HTTP/1.8\r\nHost: 192.168.1.200\r\n\r\n", accion); 
if (send(socket_desc, message, strlen (message), 0) < 0 ) {
    puts("Send failed");
    close(socket_desc);
    return 1;
  }


  read(socket_desc, server_reply, 2000);
  close(socket_desc);
  return 0; 
}


int main () {
  int hora_mx, hora_mn, hora_ac, resultado;
  char *punto, *fecha_h, fecha_hab[10], *fecha_s, fecha_sal[10],  *hora_h, hora_hab[8], *hora_s, hora_sal[8],*temp_h, temp_hab[4], *temp_s, temp_sal[4], *hum_h, hum_hab[4], *hum_s, hum_sal[4], text[302],  server_reply[2000], *onoff;
  time_t now =time(NULL);
  struct tm *tm;
  now = time(NULL);
  // Inicializar la pantalla;
  ssd1306_clearDisplay();
  ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
 
  //Recuperar parámetro onoff pantalla
  recuperar_datos(4,server_reply); 

  punto=strstr(server_reply, "BODY:");
  punto=strstr(punto,":"); 
  punto[strcspn(punto,"\n")] =0;  
  strtok(punto,",");
  onoff=strtok(NULL,","); 
  //Sacamos la hora actual  
  tm=localtime(&now);
  if( tm->tm_hour == 0 || tm->tm_hour == 1 ) {
    hora_mn = 8;
    hora_mx = 23;
    hora_ac = 17;
  }
  else {
    hora_mn = hora_min;
    hora_mx = hora_max;
    hora_ac = tm->tm_hour;
  }
  
  if (hora_mn <= hora_ac && hora_ac <= hora_mx && strcmp("1", onoff) == 0) {
  
  // obtener datos temp habitacion
  recuperar_datos(0,server_reply);
  
  punto=strstr(server_reply, "BODY:"); 
  punto=strstr(punto,":"); 
  punto[strcspn(punto,"\n")] =0;
  strtok(punto,","); 
  fecha_h=strtok(NULL,","); 
  hora_h=strtok(NULL,","); 
  temp_h=strtok(NULL,",");
  sprintf(temp_hab,"%s",temp_h);
  sprintf(fecha_hab,"%s",fecha_h);
  sprintf(hora_hab,"%s",hora_h);   
  // obtener datos humedad habitacion
  recuperar_datos(1,server_reply);
  punto=strstr(server_reply, "BODY:"); 
  punto=strstr(punto,":"); 
  punto[strcspn(punto,"\n")] =0;
  strtok(punto,","); 
  fecha_h=strtok(NULL,","); 
  hora_h=strtok(NULL,","); 
  hum_h=strtok(NULL,",");
  sprintf(hum_hab,"%s",hum_h);  
  sprintf(fecha_hab,"%s",fecha_h);
  sprintf(hora_hab,"%s",hora_h); 
  sprintf(text, "Hab       %s\n    %s\nT: %s C  H: %s % \n\n", fecha_hab, hora_hab, temp_hab, hum_hab); 
  // obtener datos temp salon
  recuperar_datos(2,server_reply);
  punto=strstr(server_reply, "BODY:"); 
  punto=strstr(punto,":"); 
  punto[strcspn(punto,"\n")] =0;
 // printf("RESPUESTA 2: %s\n",punto); 
  strtok(punto,",");
 // printf("RESPUESTA 3:%s\n",punto); 
  fecha_s=strtok(NULL,","); 
  hora_s=strtok(NULL,","); 
  temp_s=strtok(NULL,",");
  sprintf(temp_sal,"%s",temp_s);
  sprintf(fecha_sal,"%s",fecha_s);
  sprintf(hora_sal,"%s",hora_s); 
   
  // obtener datos humedad salon
  recuperar_datos(3,server_reply);
  punto=strstr(server_reply, "BODY:"); 
  punto=strstr(punto,":"); 
  punto[strcspn(punto,"\n")] =0;
  strtok(punto,","); 
  fecha_s=strtok(NULL,","); 
  hora_s=strtok(NULL,","); 
  hum_s=strtok(NULL,",");
  sprintf(hum_sal,"%s",hum_s);  
  sprintf(fecha_sal,"%s",fecha_s);
  sprintf(hora_sal,"%s",hora_s);
  
  // PRINT SCREEN
  sprintf(text, "%sSalon     %s\n    %s\nT: %s C  H: %s %% ",text,fecha_sal, hora_sal, temp_sal, hum_sal);
  ssd1306_drawString(text);
	ssd1306_display();
	ssd1306_clearDisplay();
	//ssd1306_command(SSD1306_DISPLAYOFF);
  return 0;
 }
 else {
   resultado=strcmp("1", onoff);
   printf("HORA EXCEDIDA boton = %s resultado= %i \n",onoff,resultado);
   
  }
 ssd1306_clearDisplay();
 ssd1306_command(SSD1306_DISPLAYOFF);
 return 0;
}
