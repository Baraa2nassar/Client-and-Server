/*
Author: Baraa Nassar and Brandon
Application Name: lab11.c
Functionality Purpose:  A simple server in the internet domain using TCP
   The port number is passed as an argument 
Version: 1.0
5-19-2021
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}//you are going to be listening here, 

int main(int argc, char *argv[])
{
    int sockfd, portno, n, wannaexit;

    struct sockaddr_in serv_addr;
    struct hostent *server;



    char buffer[256];
    if (argc < 3) { //error message if less than 3 arguements
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //serever flags
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) { //if you do not get a response back, then you get a null back
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; 
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting"); //instead of bind, it is doing a connect

//if the host has been provided then it will go through this loop
    char strD [39];
    snprintf(strD, sizeof strD, "netstat -ant | grep %d", portno);
    system(strD);
    printf("------------------\n");

  for (;;){ 
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");

    wannaexit=strncasecmp(buffer,"exit",4); // checks
    if (wannaexit == 0){ 
      printf("exiting client\n");
      break;
      //exit(0);
      //break;
    }

  
    bzero(buffer,256);
    n = read(sockfd,buffer,255); //read from the otherside what the server has sent to me
    
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);


  }
  close(sockfd);
  printf("\n\nconnection closed\n");
  //exit(0);
//    return 0;
}
