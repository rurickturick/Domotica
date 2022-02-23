#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
int main () {
  int socket_desc;
  struct sockaddr_in server;
  char *message, server_reply[2000];


  //Create a socket
   socket_desc =socket(AF_INET, SOCK_STREAM, 0);
   if (socket_desc == -1) {
      printf("Couldn't create a socket");
   }

   server.sin_addr.s_addr = inet_addr (" "); // IP ADDRESS
   server.sin_family = AF_INET;
   server.sin_port =htons (80);


  //Connect to remote sever

  if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) <0){
    puts("connect error");
    close(socket_desc);
    return 1;
  }

  puts("Connect \n");

  //Send some Data
   message = "GET / HTTP/1.1\r\nHost: <IP ADDRESS>\r\n\r\n";
  // message = "GET / HTTP/1.0\r\n\r\n";
if (send(socket_desc, message, strlen (message), 0) < 0 ) {
    puts("Send failed");
    close(socket_desc);
    return 1;
  }
  puts ("Data Send\n");


  // Receive a reply from the server
  if (recv(socket_desc, server_reply, 2000, 0) < 0) {
     puts("recv failed");
     close(socket_desc);
     return 1;
  }
  puts("Reply received \n");
  puts(server_reply);
  
  close(socket_desc);
  return 0;
}
