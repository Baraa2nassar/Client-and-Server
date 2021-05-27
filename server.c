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
#include <sys/socket.h>//socket library
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen; //client if the length of the client address
     char buffer[256];//allow 256 charachters to be in the string
     struct sockaddr_in serv_addr, cli_addr; //socket adder in, it doesn't use string or integers
     int n, wannaexit;//wanna exit ==1? then exit if not then don't exit
     if (argc < 2) { /*if there are no arguements in
               the command line, it will exit*/
         fprintf(stderr,"- ERROR, no port provided\n");
         exit(1);
     }
     //printf("%s\n",argv[1] );

     sockfd = socket(AF_INET, SOCK_STREAM, 0);//first directive is the creation of the socket through socket command
     //creates a number, sockfd is int, AF_INET is a reserved word as well as SOCK_STREAM
     //you want to accept the tcp value to connect connection, if not found then it will be a 0
     //strcpy(command,"ps -ef|grep server | grep -v grep");
     //system(command);
     //system("ps -ef|grep server | grep -v grep");


     if (sockfd < 0) 
        error("- ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     //you are zeroing out the address
     portno = atoi(argv[1]);
     if (portno>=1 && portno<=1023){
      error("- sorry these are 'well known' ports\nplease choose a port between 1024 to 49151");
     }
     if (portno>=49152 && portno<=65535){
      error("- sorry these are 'ephemeral' ports.\nplease choose a port between 1024 to 49151");
     }
     //port number is the number in the command line, 
     //it could be a string, so we would convert it into an integer
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
      //two other ways to do the same thing:
//     serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //using the htnol to a format the system understands
//     serv_addr.sin_addr.s_addr = htonl(0x824414ab); //the 0x means it is a hexadecimal number
     
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) //you want to use this socket to communicate back and forth
              error("- ERROR on binding");
     
     printf("---------\n");
     char strD [39];
     snprintf(strD, sizeof strD, "netstat -ant | grep %d", portno);
     system("ps -ef|grep  server.c"); 

     listen(sockfd,5); //you can now start listening, when the program is laugnched it stops here waiting for the client

     clilen = sizeof(cli_addr); //now you are done, you are a server
     printf("\n\nwaiting for a new client...\n");
     for (;;)
     {
       newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
       //will setup new file descripter
       if (newsockfd < 0) 
         error("ERROR on accept");
       int counter =0;
       for (;;) 
       {
         bzero(buffer,256);
         n = read(newsockfd,buffer,255);
         if (n < 0) error("ERROR reading from socket");
         printf("%d: Here is the message: %s\n",counter,buffer);
        
        char str[25];
        //char message[25];
        snprintf(str, sizeof str, "I got your message #%d", counter);

         n = write(newsockfd,str,strlen(str));//18 is for 18 charachters long
        //want to print the counter here #
         //x = write (newsockfd,counter,strlen(counter))
         //printf("%d ",counter);

         if (n < 0) error("ERROR writing to socket");

         wannaexit=strncasecmp(buffer,"exit",4);//4 charachters long
         if (wannaexit == 0){
            //printf("- exiting server");
            break;
         }
          counter ++;
       }
       close(newsockfd);//you just closed the client, it finished, you are still listening for connections
       printf("\n\n- connection closed\n\n- waiting for a new client\n");
     }   
//     return 0; 
}
