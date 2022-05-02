#include <stdio.h>
#include <stdlib.h>
#include "gamelib.h"
int main(){
  menu();
  int scelta,no_primo=0,fine_gioco;
  do                                                     //ciclo che permette di controllare se la scelta è stata effettuta per bene
  {
    if(scanf("%d",&scelta)!=1)
    {
      scelta=-1;
    }
    int c;
    while((c=getc(stdin))!='\n' && c!= EOF);
    system("clear");
    if(no_primo==0){
      if(scelta==2){
        scelta=1;
      }
    }
    switch (scelta) {  //switch per i 3 casi: imposta, gioca e termina gioco
      case 1:
      imposta_gioco();
      no_primo++;
      scelta=-2;  //cambio il valore di scelta affichè si possa ritornare al menù e decidere se riimpostare il gioco, giocare o terminare il gioco
      menu();
      break;
      case 2:
      system("clear");
      printf("▄████▄   ██░ ██ ▓█████     ██▓ ██▓         ▄████  ██▓ ▒█████   ▄████▄   ▒█████      ██▓ ███▄    █  ██▓▒███████▒ ██▓ ▐██▌  \n");
      printf("▒██▀ ▀█  ▓██░ ██▒▓█   ▀    ▓██▒▓██▒        ██▒ ▀█▒▓██▒▒██▒  ██▒▒██▀ ▀█  ▒██▒  ██▒   ▓██▒ ██ ▀█   █ ▓██▒▒ ▒ ▒ ▄▀░▓██▒ ▐██▌ \n");
      printf("▒▓█    ▄ ▒██▀▀██░▒███      ▒██▒▒██░       ▒██░▄▄▄░▒██▒▒██░  ██▒▒▓█    ▄ ▒██░  ██▒   ▒██▒▓██  ▀█ ██▒▒██▒░ ▒ ▄▀▒░ ▒██▒ ▐██▌ \n");
      printf("▒▓▓▄ ▄██▒░▓█ ░██ ▒▓█  ▄    ░██░▒██░       ░▓█  ██▓░██░▒██   ██░▒▓▓▄ ▄██▒▒██   ██░   ░██░▓██▒  ▐▌██▒░██░  ▄▀▒   ░░██░ ▓██▒ \n");
      printf("▒ ▓███▀ ░░▓█▒░██▓░▒████▒   ░██░░██████▒   ░▒▓███▀▒░██░░ ████▓▒░▒ ▓███▀ ░░ ████▓▒░   ░██░▒██░   ▓██░░██░▒███████▒░██░ ▒▄▄  \n");
      printf("░ ░▒ ▒  ░ ▒ ░░▒░▒░░ ▒░ ░   ░▓  ░ ▒░▓  ░    ░▒   ▒ ░▓  ░ ▒░▒░▒░ ░ ░▒ ▒  ░░ ▒░▒░▒░    ░▓  ░ ▒░   ▒ ▒ ░▓  ░▒▒ ▓░▒░▒░▓   ░▀▀▒ \n");
      printf("░  ▒    ▒ ░▒░ ░ ░ ░  ░    ▒ ░░ ░ ▒  ░     ░   ░  ▒ ░  ░ ▒ ▒░   ░  ▒     ░ ▒ ▒░     ▒ ░░ ░░   ░ ▒░ ▒ ░░░▒ ▒ ░ ▒ ▒ ░ ░  ░   \n");
      printf("░         ░  ░░ ░   ░       ▒ ░  ░ ░      ░ ░   ░  ▒ ░░ ░ ░ ▒  ░        ░ ░ ░ ▒      ▒ ░   ░   ░ ░  ▒ ░░ ░ ░ ░ ░ ▒ ░    ░ \n");
      printf("░ ░       ░  ░  ░   ░  ░    ░      ░  ░         ░  ░      ░ ░  ░ ░          ░ ░      ░           ░  ░    ░ ░     ░   ░    \n");
      printf("░                                                              ░                                       ░                  \n");
      fine_gioco=gioca();
      no_primo=0;
      case 3:
      termina_gioco(fine_gioco);
      break;
      default:
      printf("\n              IL COMANDO E' SBAGLIATO                         \n\n");
      menu();
    }
  }while(scelta<1 || scelta>3);
}
