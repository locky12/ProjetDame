
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "communication.h"
#include "sdl2.h"
// typedef struct Move {
//   int position[2];
//   int newPosition[2];
// }Move;
void write_serveur(int socket, char *buffer)
{
  int taille_envoyee;
  taille_envoyee = send( socket, buffer, strlen(buffer), 0);
  if (taille_envoyee == -1)
  {
    perror("send()");

  }
}

void read_serveur(int socket, char *buffer)
{
  ssize_t taille_recue;
  taille_recue = recv(socket, buffer, strlen(buffer),0);
  if (taille_recue == -1){
    perror("recv()");
  }
}

void send_move (Move * move, int ** arrayCapture, int size, char * buffer  ){
  // char buffer[100] = {0};
  printf("%s\n","je bug" );
 sprintf(buffer,"%d/%d-%d/%d-%d",99,move->position[0],move->position[1],move->newPosition[0],move->newPosition[0]);
  for (int i = 0; i < size; i++){
    printf("%s\n","ta");
    sprintf(buffer,"%s/%d-%d",buffer,arrayCapture[i][0],arrayCapture[i][0]);
  }
  sprintf(buffer,"%s/%d",buffer,size);

}
char ** read_move_query (char * buffer){
  int       i         = 0,
            r         = 0,
            count     = 0,
            ok        = 1;
  const int sizeAlloc = 3;

  char      temp [100];
  char ** result;
  result = malloc((sizeAlloc + atoi(&buffer[strlen(buffer) - 1])) * sizeof(*result));
  if(result == NULL){
    perror("malloc()");
    exit(1);
  }

  // printf("ant decomposition : %s\n",buffer );
  while(ok){
    //printf("char : %c\n",buffer[i] );
    if(buffer[i] != '/' && buffer[i] != '\0' && buffer[i] != '-'){
      temp[r] = buffer[i];
      i++;
      r++;
    }
    else if (buffer[i] == '/') {
      puts("passe dans le else");
      i++;
      result[count] = malloc(3*sizeof(result));
      if(result[count] == NULL){
        perror("malloc()");
        exit(1);
      }
      printf("%s\n",temp );
      strcpy(result[count],temp);
      count++;
      r = 0;
        // memset(temp,'\0',strlen(temp));
    }
    else if (buffer[i] == '-') {
      sprintf(result[count-1],"%c%c",temp[0],buffer[i + 1]);
      i++;
    }
    else if (buffer[i] == '\0') {
        ok = 0;
      }




  }

  //printf("result 3 : %s\n",result[3] );
  return result;
}
// int main(int argc, char const *argv[]) {
// Move * move = malloc(sizeof(Move));
// int ** arrayCapture = malloc(10 *sizeof(arrayCapture));
// for (int i = 0; i< 10; i++){
//   arrayCapture[i] = malloc(2 * sizeof(arrayCapture));
// }
// char ** result;
// char  buffer [100];
// arrayCapture[0][0] = 4;
// arrayCapture[0][1] = 5;
// arrayCapture[1][0] = 1;
// arrayCapture[1][1] = 3;
// move->position[0] = 9 ;
// move->position[0] = 2;
// move->newPosition[0] = 8 ;
// move->newPosition[1] = 7;
// // int ** arrayCapture = malloc(10 * sizeof(arrayCapture));
// send_move (move, arrayCapture,2,buffer);
// result = read_move_query (buffer);
// printf("%s\n",buffer );
// printf("%c\n",result[0][0] );
// printf("%c\n",result[0][1] );
// printf("%c\n",result[1][0] );
// printf("%c\n",result[1][1] );
// printf("%c\n",result[2][0] );
// printf("%c\n",result[2][1] );
//   return 0;
// }
