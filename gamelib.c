#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //per includere sleep()
#include "gamelib.h"

int contatore_stanze=0,quest_rimanenti;
Giocatori *giocatori;
struct Stanza *p_stanza;
struct Stanza *stanza;
struct Stanza *scambio;  //puntatore di appoggio
int n_imp,n_g=0,morti=0;
unsigned short quest_da_finire,contatore_quest=0;

static int tipo_stanza(void){
  int prob;
  prob=rand()%100; //con rand()%100 assegno a prob un valore da 0 a 99 per poi dividere questo intervallo in ulteriori sottointervalli [0-14],[15-39],[40-69],[70-99]
  if(prob<15){  //quest complicata
    if(quest_da_finire-contatore_quest<2) //filtro che non permette di creare una ulteriore quest complicata se manca solo un punto per arrivare all'obiettivo delle quest
    return 1;
    else
    return 2;
  }
  else{
    if(prob>=15 && prob<40){ //botola
      return 3;
    }
    else{
      if(prob>=40 && prob<70){ //quest_semplice
        return 1;
      }
      else
      return 0;   //vuota
    }
  }
}
static char *stampa_tipo_stanza(int toppo){
  switch (toppo) {     //attraverso uno swith faccio ritornare alla funzione una stringa che rappresenza il tipo di stanza
    case 0:
    return "vuota";
    break;
    case 1:
    return  "quest_semplice";
    break;
    case 2:
    return  "quest_complicata";
    break;
    case 3:
    return  "botola";
    break;
  }
  return "NULL";
}
static char *stampa_nome(int nom){
  switch (nom) { //attraverso uno swith faccio ritornare alla funzione una stringa che rappresenza il nome dei giocatori
    case 0:
    return "verde";
    break;
    case 1:
    return "giallo";
    break;
    case 2:
    return "blu";
    break;
    case 3:
    return "rosso";
    break;
    case 4:
    return "viola";
    break;
    case 5:
    return "rosa";
    break;
    case 6:
    return "celeste";
    break;
    case 7:
    return"nero";
    break;
    case 8:
    return "bianco";
    break;
    case 9:
    return "arancione";
    break;
  }
  return "NULL";
}
static char *stampa_stato(int stat){
  switch (stat) {   //attraverso uno swith faccio ritornare alla funzione una stringa che rappresenza lo stato dei giocatori
    case 0:
    return "astronauta";
    break;
    case 1:
    return "impostore";
    break;
    case 2:
    return "assassinato";
    break;
    case 3:
    return "defenestrato";
    break;
  }
  return "NULL";
}
static int avanza(Giocatori *pnt){ //permette al giocatore di spostarsi da una stanza a un altra
  int scelta_avanza;
  do { //controlla  se la scelta è stata effettuata per bene
    printf("(1) destra\n(2) sinistra\n(3) avanti\n");
    if(pnt->posizione->stanza_precedente!=NULL)
    printf("(4) stanza precedente\n");
    if(scanf("%d",&scelta_avanza)!=1)
    {
      scelta_avanza=-1;
    }
    int c;
    while((c=getc(stdin))!='\n' && c!= EOF);
    if(pnt->posizione->stanza_precedente==NULL){
      if(scelta_avanza==4)
      scelta_avanza=10;
    }
  } while(scelta_avanza<0 || scelta_avanza>4);

  switch (scelta_avanza) //distinguo i 4 casi in cui puo avanzare il giocatore
  {
    case 3: //avanti
    if(pnt->posizione->avanti==NULL){  //filtro per vedere se esiste gia una stanza
      contatore_stanze++;
      do {  //alloco stanza fino a che non è stata allocata per bene, quindi escludendo errori di allocamneto
        stanza=(struct Stanza*) malloc(sizeof(struct Stanza));
      } while(stanza==NULL);
      stanza->cronologia=p_stanza; //faccio puntare stanza->cronologia all indirizzo di p_stanza che è l ultima stanza creata
      stanza->avanti=NULL;
      stanza->sinistra=NULL;
      stanza->destra=NULL;
      stanza->tipo=tipo_stanza(); //tipo stanza casuale
      stanza->emergenza=non_effettuata;
      stanza->stanza_precedente=pnt->posizione; //facendo cosi permetto al giocatore in seguito di tornare indietro
      pnt->posizione->avanti=stanza;   //a pnt->posizione->avanti gli faccio puntare all'ultimastanza creata, cosi facendo se altri giocatori faranno lo stesso tragitto, non creerà un ulteriore stanza ma riutilizzerà la stessa
      pnt->posizione=pnt->posizione->avanti;  //cambio la posizione del giocatore
      p_stanza=pnt->posizione;  //faccio puntare a p_stanza l'ultima stanza creata
    }
    else
    pnt->posizione=pnt->posizione->avanti;
    break;


    case 2: //sinistra
    if(pnt->posizione->sinistra==NULL){   //filtro per vedere se esiste gia una stanza
      contatore_stanze++;
      do {   //alloco stanza fino a che non è stata allocata per bene, quindi escludendo errori di allocamneto
        stanza=(struct Stanza*) malloc(sizeof(struct Stanza));
      } while(stanza==NULL);
      stanza->cronologia=p_stanza;  //faccio puntare stanza->cronologia all indirizzo di p_stanza che è l ultima stanza creata
      p_stanza=pnt->posizione;
      stanza->avanti=NULL;
      stanza->sinistra=NULL;
      stanza->destra=NULL;
      stanza->tipo=tipo_stanza();  //tipo stanza casuale
      stanza->emergenza=non_effettuata;
      stanza->stanza_precedente=pnt->posizione;   //facendo cosi permetto al giocatore in seguito di tornare indietro
      pnt->posizione->sinistra=stanza;            //a pnt->posizione->avanti gli faccio puntare all'ultimastanza creata, cosi facendo se altri giocatori faranno lo stesso tragitto, non creerà un ulteriore stanza ma riutilizzerà la stessa
      pnt->posizione=pnt->posizione->sinistra;    //cambio la posizione del giocatore
      p_stanza=pnt->posizione;                    //faccio puntare a p_stanza l'ultima stanza creata
    }
    else
    pnt->posizione=pnt->posizione->sinistra;
    break;


    case 1: //destra
    if(pnt->posizione->destra==NULL){ //filtro per vedere se esiste gia una stanza
      contatore_stanze++;
      do {   //alloco stanza fino a che non è stata allocata per bene, quindi escludendo errori di allocamneto
        stanza=(struct Stanza*) malloc(sizeof(struct Stanza));
      } while(stanza==NULL);
      stanza->cronologia=p_stanza;
      stanza->avanti=NULL;
      stanza->sinistra=NULL;
      stanza->destra=NULL;
      stanza->tipo=tipo_stanza();   //tipo stanza casuale
      stanza->emergenza=non_effettuata;
      stanza->stanza_precedente=pnt->posizione;   //facendo cosi permetto al giocatore in seguito di tornare indietro
      pnt->posizione->destra=stanza;              //a pnt->posizione->avanti gli faccio puntare all'ultimastanza creata, cosi facendo se altri giocatori faranno lo stesso tragitto, non creerà un ulteriore stanza ma riutilizzerà la stessa
      pnt->posizione=pnt->posizione->destra;      //cambio la posizione del giocatore
      p_stanza=pnt->posizione;                    //faccio puntare a p_stanza l'ultima stanza creata
    }
    else
    pnt->posizione=pnt->posizione->destra;
    break;


    case 4: //indietro
    pnt->posizione=pnt->posizione->stanza_precedente;
    break;
  }
  return contatore_stanze;
}
static int chiamata_di_emergenza(Giocatori *pnt){
  int impostori=0,astronauti=0,defenestrato=0;
  for(int i=0;i<n_g;i++){ //controlla il numero diastronauti, il numero di impostori e cambia la posizione degli astronauti assassinati ==NULL
    if(giocatori[i].stato==0 && pnt->posizione==giocatori[i].posizione)
    astronauti++;
    if(giocatori[i].stato==1 && pnt->posizione==giocatori[i].posizione)
    impostori++;
    if(giocatori[i].stato==2 && pnt->posizione==giocatori[i].posizione)
    giocatori[i].posizione=NULL;
  }
  if(astronauti+impostori==1){  //caso un solo astronauta o un solo impostore
    for(int i=0;i<n_g;i++){
      if(pnt->posizione==giocatori[i].posizione && (giocatori[i].stato==1 || giocatori[i].stato==1)){
        if(giocatori[i].stato==1)
        n_imp--;
        giocatori[i].stato=3;
        giocatori[i].posizione=NULL;
        printf("%s sei stato %s automaticamente essendo l'unuco giocatore nella stanza\n",stampa_nome(giocatori[i].nome),stampa_stato(giocatori[i].stato));
      }
    }
  }
  else{  //altri casi
    int prob_imp=0,prob_astro=0;
    prob_imp=-(impostori-1)*30+astronauti*20;
    prob_astro=impostori*20-(astronauti-1)*30;
    if(astronauti+impostori>1 && (astronauti==0 || impostori==0)){  //numero di persone>1 ma sono solo astronauti o impostori
      do {
        for(int i=0;i<n_g;i++){
          if(pnt->posizione==giocatori[i].posizione && (rand()%(astronauti+impostori))==0 && defenestrato==0){
            defenestrato++;
            if(giocatori[i].stato==1)
            n_imp--;
            giocatori[i].stato=3;
            giocatori[i].posizione=NULL;
            printf("%s sei stato %s\n",stampa_nome(giocatori[i].nome),stampa_stato(giocatori[i].stato));
          }
        }
      } while(defenestrato==0);
    }
    if(defenestrato==0){ //controllo che non sia stato defenestrato nessuno, se è cosi allora controllo i rimanenti casi
      do {
        for(int i=0;i<n_g;i++){
          if(giocatori[i].posizione==pnt->posizione && defenestrato==0 && (giocatori[i].stato==1 || giocatori[i].stato==0)){
            if(rand()%100<30+prob_imp && giocatori[i].stato==1){  //filtro con percentuale
              n_imp--;
              defenestrato++;
              giocatori[i].stato=3;
              giocatori[i].posizione=NULL;
              printf("%s sei stato %s\n",stampa_nome(giocatori[i].nome),stampa_stato(giocatori[i].stato));
            }
            if(rand()%100<30+prob_astro && giocatori[i].stato==0){ //filtro con percentuale
              defenestrato++;
              giocatori[i].stato=3;
              giocatori[i].posizione=NULL;
              printf("%s sei stato %s\n",stampa_nome(giocatori[i].nome),stampa_stato(giocatori[i].stato));
            }
          }
        }
      } while(defenestrato==0);  //finchè non ci sarà un defenestrato
    }
  }
  return n_imp;
}
static void spostati_con_botola(Giocatori *pnt){
  int contatore_botole=0,contatore_stanze2=0,giocatori_presenti=0;
  scambio=p_stanza;
  while(scambio!=NULL){   //controllo tutte le stanze aumentando il contatore delle botole se la stanza è di tipo botola
    if(scambio->tipo==3)
    contatore_botole++;
    contatore_stanze2++;
    scambio=scambio->cronologia;
  }
  printf("\n%d n botole\n",contatore_botole);
  if(contatore_botole==1){  //caso solo una botola allora verrà portato su una stanza casuale
    do {
      scambio=p_stanza;
      while(scambio!=NULL){
        if(rand()%contatore_stanze2==0){
          pnt->posizione=scambio;
          contatore_stanze2=0;
        }
        scambio=scambio->cronologia;
      }
    } while(contatore_stanze2!=0);
  }
  else{  //caso più botole
    do {
      scambio=p_stanza;
      while(scambio!=NULL){
        if(scambio->tipo==3 && rand()%contatore_botole==0){
          pnt->posizione=scambio;
          contatore_stanze2=0;
        }
        scambio=scambio->cronologia;
      }
    } while(contatore_stanze2!=0);
  }
  printf("\n%s usando la botola ti sei spostato in questa stanza %p",stampa_nome(pnt->nome),pnt->posizione);
  giocatori_presenti=0;
  for(int i=0;i<n_g;i++){  //conto i giocatori
    if(pnt->posizione==giocatori[i].posizione && pnt->nome!=giocatori[i].nome && (giocatori[i].stato==1 || giocatori[i].stato==0))
    giocatori_presenti++;
  }
  if(giocatori_presenti!=0){
    printf("\nI giocatori oltre a te, presenti in questa stanza sono: ");
    for(int i=0;i<n_g;i++){  //stsmpo i nomi dei giocatori presenti
      if(pnt->posizione==giocatori[i].posizione && pnt->nome!=giocatori[i].nome && (giocatori[i].stato==0 || giocatori[i].stato==1))
      printf("%s ",stampa_nome(giocatori[i].nome));
    }
  }
  else
  printf("\n%s sei solo in questa stanza %p",stampa_nome(pnt->nome),pnt->posizione);
}
static int uccidi(Giocatori *pnt){
  int contatore_possibili_morti[n_g],contatore_stanza_a=0,scelta_uccisione,contatore_stanza_precedente_a=0;
  do {  //controlla se la scelta è stata fatta bene
    printf("\nScegli:\n");
    for(int i=0;i<n_g;i++){ //conta i giocatori nella stanza precedente
      if(pnt->posizione->stanza_precedente==giocatori[i].posizione)
      contatore_stanza_precedente_a++;
      if(pnt->posizione==giocatori[i].posizione && giocatori[i].stato==0)   //filta e fa passare solo gli astronauti presenti nella stanza
      {
      printf("(%d) %s\n",i,stampa_nome(giocatori[i].nome));
      contatore_possibili_morti[i]=1;
      contatore_stanza_a++;
      }
    }
    if(scanf("%d",&scelta_uccisione)!=1)
    {
      scelta_uccisione=-1;
    }
    int c;
    while((c=getc(stdin))!='\n' && c!= EOF);
  } while(contatore_possibili_morti[scelta_uccisione]!=1);
  giocatori[scelta_uccisione].stato=2;   //cambio lo stato al giocatore ucciso
  if(contatore_stanza_a>0)
  contatore_stanza_a--;
  printf("Hai ucciso %s\n",stampa_nome(giocatori[scelta_uccisione].nome));
  if((rand()%100)+1<(50*contatore_stanza_a+20*contatore_stanza_precedente_a)){  //con la percentuale decido se defenestrare l'impostore
    pnt->stato=3;
    n_imp--;
    printf("%s sei stato %s automaticamente senza votazione per via di troppi testimoni sia nella tua stanza sia nella stanza precedente\n",stampa_nome(pnt->nome),stampa_stato(pnt->stato));
  }
  return n_imp;
}
static void turni(int *p_t){
  for(int i=0;i<n_g;i++)    //ciclo for per assegnare ad ogni i-esima cella dell'array il valore i
  p_t[i]=i;
  for(int i=0;i<n_g;i++){  //ciclo che attraverso 3 uguaglianze e un numero randomico, compreso da 0 a numero dei giocatori, permette di creare effettivamente i turni
    int g=0,w=rand()%n_g;
    g=p_t[i];
    p_t[i]=p_t[w];
    p_t[w]=g;
  }
}
static int completa_quest(int contatore_quest,Giocatori *pnt){
  //funzione che cambia il tipo della stanza in cui si trova il giocatore facendola diventare vuota e aggiungendo (1) se quest semplice oppure (2) se quest complicata
  if(pnt->posizione->tipo==1)
  contatore_quest++;
  if(pnt->posizione->tipo==2)
  contatore_quest+=2;
  pnt->posizione->tipo=0;
 return contatore_quest;
}
static void deallocamento(void){
  if(stanza!=NULL){
    do {
      free(p_stanza); //p_stanza è uguale alla prima stanza creata ogni volta che si imposta il gioco e quando il gioco è iniziato rappresenta sempre l ultima stanza creata in ordine cronologico
      p_stanza=p_stanza->cronologia; //con questa uguaglianza si passa dall'ultima stanza creata alla penultima
    } while(p_stanza!=NULL); //condizione che ci permette di terminare il ciclo do while quando è stata deallocata la prima stanza creata per questa partita e quindi termina il ciclo
    free(giocatori); //dealloca la lista dinamica di giocatori
    giocatori=NULL;
  }
}
static void stampa_giocatore(Giocatori *pnt){ //stampa le caratteristiche del giocatore
  printf("Il giocatore %s è un %s e si trova nella stanza %p di tipo %s\n",stampa_nome(pnt->nome),stampa_stato(pnt->stato),pnt->posizione,stampa_tipo_stanza(pnt->posizione->tipo));
}
static void sabotaggio(Giocatori *pnt){  //cambia il tipo di stanza facendola diventare vuota
  pnt->posizione->tipo=0;
  printf("Complimenti %s hai sabotato la stanza facendola diventare %s\n",stampa_stato(pnt->stato),stampa_tipo_stanza(pnt->posizione->tipo));
}
static int menu_giocatore(int stat,int nom,struct Stanza *pos,int comando){
  int caso=0,morto_in_stanza=0,n=0,scelta_a;
  int contatore_a_in_s=0;
  int scelta_i;
  int array_astronauti_in_stanza_con_impostore[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  switch (stat) {
    case 0: //astronauti
    for(int i=0;i<n_g;i++){  //questo ciclo passa tutti i giocatori mettendo un eventuale assassinato
      if(pos==giocatori[i].posizione && giocatori[i].stato==2 && pos->emergenza==0){
            n=i;  //mi segno il giocatore assassinato per poi poterlo stampare, mi basta una sola variabile perchè i giocatori assassinati per turno sono al massimo 1
            morto_in_stanza++; //variabile che mi servirà per decidere quale menù applicare a seconda del giocatore e a seconda della stanza in cui si trova il giocatore
      }
    }
    if(pos->tipo==1 || pos->tipo==2) //serve per controllare se la stanza presenza una quest di qualsiasi tipo per poi aggiungere l'azione "completa quest"
    caso++;
    if(morto_in_stanza!=0)
    caso+=2;
    switch (caso) { //switch per scegliere il menù più adatto al giocatore in base alla stanza in cui si trova
      case 0://avanza
      do { //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s puoi solo (1) avanzare\n",stampa_nome(nom),stampa_stato(stat));
        if(scanf("%d",&scelta_a)!=1)
        {
          scelta_a=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_a!=1);
      comando=1;
      break;
      case 1://quest
      do {  //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n2)completa la %s presente in questa stanza\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_a)!=1)
        {
          scelta_a=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_a<1 || scelta_a>2);
      if(scelta_a==1)
      comando=1;
      if(scelta_a==2)
      comando=2;
      break;
      case 2://chiamata
      do {  //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2)chiama la chiamata d'emergenza perchè giocatore %s è stato assassinato\n",stampa_nome(nom),stampa_stato(stat),stampa_nome(giocatori[n].nome));
        if(scanf("%d",&scelta_a)!=1)
        {
          scelta_a=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_a<1 || scelta_a>2);
      if(scelta_a==1)
      comando=1;
      if(scelta_a==2)
      comando=3;

      break;
      case 3://entrambe
      do {   //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2)completa %s presente in questa stanza\n(3)chiama la chiamata d'emergenza perche giocatore %s è stato assassinato\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo),stampa_nome(giocatori[n].nome));
        if(scanf("%d",&scelta_a)!=1)
        {
          scelta_a=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_a<1 || scelta_a>3);
      if(scelta_a==1)
      comando=1;
      if(scelta_a==2)
      comando=2;
      if(scelta_a==3)
      comando=3;

      break;
    }
    break;






    case 1://impostori
    if(pos->tipo==1 || pos->tipo==2) //controlla se il tipo della stanza in cui si trova l'impostore è quest semplice o complicata
    caso+=8;
    for(int i=0;i<n_g;i++){               //ciclo per vedere se l impostore puo uccidere qualcuno
      if(pos==giocatori[i].posizione && giocatori[i].stato==0){
        contatore_a_in_s++;
        array_astronauti_in_stanza_con_impostore[i]=giocatori[i].nome;
      }
    }
    if(contatore_a_in_s!=0)  //se questa variabile è diversa da zero allora viene implementata la possibilità all'impostore di uccidere un'astronauta
    caso+=2;
    if(pos->tipo==3)              //vede se la stanza ha una botola aggiunge l'opzione "usa botola"
    caso++;
    for(int i=0;i<n_g;i++){         //ciclo per vedere se ci sta un astronauta assassinato
      if(pos==giocatori[i].posizione){
        if(giocatori[i].stato==2){
          //contatore_p_in_s1++;
          n=i;
          morto_in_stanza++;
        }
      }
    }
    if(morto_in_stanza!=0) //per aggiungere l'opzione "chiamats di emergenza"
    caso+=4;

    switch (caso) {
      case 0://avanza
      do {  //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s puoi solo (1) avanzare\n",stampa_nome(nom),stampa_stato(stat));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i!=1);
      comando=1;
      break;

      case 1://avanza botola
      do {  //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2)in questa stanza è presente una %s che ti portera in una stanza casuale gia visitata\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>2);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      break;

      case 2://avanza uccidi
      do {   //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n",stampa_nome(nom),stampa_stato(stat));
        printf("(2) uccidi\n");
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>2);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=4;
      break;

      case 3://avanza botola uccidi
      do {    //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2)in questa stanza è presente una %s che ti portera in una stanza casuale gia visitata\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        printf("(3) uccidi\n");
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>3);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      if(scelta_i==3)
      comando=4;
      break;

      case 4://avanza emergenza
      do {  //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) chiamata d'emergenza\n",stampa_nome(nom),stampa_stato(stat));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>2);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=3;
      break;

      case 5://avanza botola emergenza
      do {    //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2)in questa stanza è presente una %s che ti porterà in una stanza casuale gia visitata\n(3) chiamata d'emergenza\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>3);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      if(scelta_i==3)
      comando=3;
      break;

      case 6://avanza uccidi emergenza
      do {    //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n",stampa_nome(nom),stampa_stato(stat));
        printf("(2) uccidi\n");
        printf("(3) chiamata d'emergenza\n");
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>3);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=4;
      if(scelta_i==3)
      comando=3;
      break;

      case 7://avanta botola uccidi emergenza
      do {   //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) In questa stanza è presente una %s che ti porterà in una stanza casuale gia visitata\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        printf("(3) uccidi\n");
        printf("(4) chiamata d'emergenza\n");
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>4);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      if(scelta_i==3)
      comando=4;
      if(scelta_i==4)
      comando=3;
      break;

      case 8://avanza sabotaggio
      do{     //ciclo che permete di vedere se la scelta è stata fatta bene
      printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) Sabota la %s\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
      if(scanf("%d",&scelta_i)!=1)
      {
        scelta_i=-1;
      }
      int y;
      while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>2);
      if(scelta_i==1)
      comando=1;
      else
      comando=6;
      break;

      case 9://avanza botola sabotaggio
      do {     //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) in questa stanza è presente una %s che ti portera in una stanza casuale gia visitata\n(3) Sabota la %s\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>3);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      if(scelta_i==3)
      comando=6;
      break;

      case 10://avanza uccidi sabotaggio
      do {    //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) uccidi\n(3) Sabota la %s\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>3);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=4;
      if(scelta_i==3)
      comando=6;
      break;

      case 11://avanza botola uccidi sabotaggio
      do {    //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2)in questa stanza è presente una %s che ti portera in una stanza casuale gia visitata\n(3) uccidi\n(4) Sabota la %s\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>4);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      if(scelta_i==3)
      comando=4;
      if(scelta_i==4)
      comando=6;
      break;

      case 12://avanza emergenza sabotaggio
      do {     //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) chiamata d'emergenza\n(3) Sabota la %s\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>3);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=3;
      if(scelta_i==3)
      comando=6;
      break;

      case 13://avanza botola emergenza sabotaggio
      do {   //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2)in questa stanza è presente una %s che ti porterà in una stanza casuale gia visitata\n(3) chiamata d'emergenza\n(4) Sabota la %s\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>4);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      if(scelta_i==3)
      comando=3;
      if(scelta_i==4)
      comando=6;
      break;

      case 14://avanza uccidi emergenza sabotaggio
      do {    //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) uccidi\n(3) chiamata d'emergenza\n(4) Sabota la %s\n",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>4);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=4;
      if(scelta_i==3)
      comando=3;
      if(scelta_i==4)
      comando=6;
      break;

      case 15://tutto
      do {    //ciclo che permete di vedere se la scelta è stata fatta bene
        printf("\n%s essendo un %s le azioni che puoi fare sono:\n(1) avanza\n(2) In questa stanza è presente una %s che ti porterà in una stanza casuale gia visitata\n(3) uccidi\n(4) chiamata d'emergenza\n(5) Sabota la %s",stampa_nome(nom),stampa_stato(stat),stampa_tipo_stanza(pos->tipo),stampa_tipo_stanza(pos->tipo));
        if(scanf("%d",&scelta_i)!=1)
        {
          scelta_i=-1;
        }
        int y;
        while((y=getc(stdin))!='\n' && y!= EOF);
      } while(scelta_i<1 || scelta_i>4);
      if(scelta_i==1)
      comando=1;
      if(scelta_i==2)
      comando=5;
      if(scelta_i==3)
      comando=4;
      if(scelta_i==4)
      comando=3;
      if(scelta_i==5)
      comando=6;
      break;
    }
    break;

     //assassinato
    case 2:
    break;

    //defenestrato
    case 3:
    break;
  }
  return comando;
}
static void scritta_imposta_gioco(void){
  //giusto per abbellire un po lo schermo
  printf(" ██▓ ███▄ ▄███▓ ██▓███   ▒█████    ██████ ▄▄▄█████▓ ▄▄▄           ▄████  ██▓ ▒█████   ▄████▄   ▒█████  \n");
  printf("▓██▒▓██▒▀█▀ ██▒▓██░  ██▒▒██▒  ██▒▒██    ▒ ▓  ██▒ ▓▒▒████▄        ██▒ ▀█▒▓██▒▒██▒  ██▒▒██▀ ▀█  ▒██▒  ██▒\n");
  printf("▒██▒▓██    ▓██░▓██░ ██▓▒▒██░  ██▒░ ▓██▄   ▒ ▓██░ ▒░▒██  ▀█▄     ▒██░▄▄▄░▒██▒▒██░  ██▒▒▓█    ▄ ▒██░  ██▒\n");
  printf("░██░▒██    ▒██ ▒██▄█▓▒ ▒▒██   ██░  ▒   ██▒░ ▓██▓ ░ ░██▄▄▄▄██    ░▓█  ██▓░██░▒██   ██░▒▓▓▄ ▄██▒▒██   ██░\n");
  printf("░██░▒██▒   ░██▒▒██▒ ░  ░░ ████▓▒░▒██████▒▒  ▒██▒ ░  ▓█   ▓██▒   ░▒▓███▀▒░██░░ ████▓▒░▒ ▓███▀ ░░ ████▓▒░\n");
  printf("░▓  ░ ▒░   ░  ░▒▓▒░ ░  ░░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░  ▒ ░░    ▒▒   ▓▒█░    ░▒   ▒ ░▓  ░ ▒░▒░▒░ ░ ░▒ ▒  ░░ ▒░▒░▒░ \n");
  printf(" ▒ ░░  ░      ░░▒ ░       ░ ▒ ▒░ ░ ░▒  ░ ░    ░      ▒   ▒▒ ░     ░   ░  ▒ ░  ░ ▒ ▒░   ░  ▒     ░ ▒ ▒░ \n");
  printf(" ▒ ░░      ░   ░░       ░ ░ ░ ▒  ░  ░  ░    ░        ░   ▒      ░ ░   ░  ▒ ░░ ░ ░ ▒  ░        ░ ░ ░ ▒  \n");
  printf(" ░         ░                ░ ░        ░                 ░  ░         ░  ░      ░ ░  ░ ░          ░ ░  \n");
  printf("                                                                                     ░                 \n");
}

void imposta_gioco(void){
  deallocamento();  //per deallocare la stanza iniziale se il gioco è stato gia impostato una volta
  contatore_stanze=0;
  do                                                     //ciclo che permette di controllare se la scelta è stata effettuta per bene
  {
    scritta_imposta_gioco();
    printf("\nSCEGLI IL NUMERO DEI GIOCATORI [4-10]              \n");
    if(scanf("%d",&n_g)!=1)
    {
      n_g=-1;
    }
    int w;
    while((w=getc(stdin))!='\n' && w!= EOF);
    system("clear");
  }while(n_g<4 || n_g>10);
  switch (n_g) {  //per scegliere il numero di impostori
    case 4:
    case 5:
    case 6:
    n_imp=1;
    break;
    case 7:
    case 8:
    n_imp=2;
    break;
    case 9:
    case 10:
    n_imp=3;
    break;
  }
  do {   //ciclo che alloca stanza fino a che non è stata allocata con successo, escludendo quindi errori di allocamento
    stanza=(struct Stanza*)malloc(sizeof(struct Stanza));
  } while(stanza==NULL);
  do {  //ciclo che alloca giocatori fino a che non sono stati allocati con successo, escludendo quindi errori di allocamento
    giocatori=(Giocatori*) calloc(n_g+1, sizeof(Giocatori));
  } while(giocatori==NULL);
  int contatore_imp=n_imp;
  time_t t;
  int controllo_nome[n_g];
  srand((unsigned)time(&t));
  do {                                                                //ciclo per decidere se impostore o austronauta
    for(int i=0;i<n_g;i++){    //passo tutti i giocatori
      if(contatore_imp!=0 && rand()%2==0){                                         //50% di possibilita di essere un impostore se il contatore di impostori è diverso da 0, perchè se fosse 0 allora gia ci serebbero il numero di impostori stabilito per quella partita
        giocatori[i].stato=impostore;
        if(contatore_imp>0)
        contatore_imp--;
        else
        break;
      }
      else{
        giocatori[i].stato=astronauta;
      }
    }
  } while(contatore_imp!=0);
  for(int i=0;i<n_g;i++){
    giocatori[i].posizione=stanza;
    int controllo;
    do {
      int f;
      f=rand()%10;
      giocatori[i].nome=f;
      controllo_nome[i]=f;
      controllo=0;
      for(int j=0;j<n_g;j++){   //controllo affinche due giocatori non abbiano lo stesso nome
        if(controllo_nome[i]==controllo_nome[j])
        controllo++;
      }
    } while(controllo!=1);
  }
  do                                                     //ciclo che permette di controllare se la scelta è stata effettuta per bene
  {
    scritta_imposta_gioco();
    printf("\nSCEGLI IL NUMERO DI QUEST\n");
    printf("IL NUMERO DEVE ESSERE ALMENO UGUALE A %d\n",n_g);
    if(scanf("%hd",&quest_da_finire)!=1)
    {
      quest_da_finire=-1;
    }
    int t;
    while((t=getc(stdin))!='\n' && t!= EOF);
    system("clear");
  }while(quest_da_finire<n_g);
  //imposto la stanza iniziale
  stanza->avanti=NULL;
  stanza->stanza_precedente=NULL;
  stanza->destra=NULL;
  stanza->sinistra=NULL;
  stanza->emergenza=non_effettuata;
  stanza->tipo=tipo_stanza();
  contatore_stanze++;
  for(int i=0;i<n_g;i++) //stampo i giocatori e la loro posizione
  stampa_giocatore(&giocatori[i]);
  p_stanza=stanza;  //a p_stanza do l'indirizzo della stanza iniziale
  printf("\nLa preghiamo di non scrivere niente\nLa pagina verrà pulita tra:\n");
  for(int i=5;i>0;i--){ //countdown di 5 secondi per poi pulire la pagine
  printf("%d\n",i);
  sleep(1);//pausa prima di usare system("clear")
  }
}
int gioca(void){
  int lista_t[n_g],vittoria=3,contatore_giocatori_stanza=0,controllo_uccisi,comando=0;
  while(vittoria==3){
    turni(lista_t); //ogni volta rifaccio i turni affiche sia tutto piu casuale
    for(int i=0;i<n_g;i++){ //scorro i giocatori
      if((giocatori[lista_t[i]].stato==0 || giocatori[lista_t[i]].stato==1) && n_imp!=0 && contatore_quest<quest_da_finire && n_imp!=n_g-morti){
        //filtro per togliere i giocatori che non possogno piu utilizzare e per vedere se è finita la partita
        contatore_giocatori_stanza=0;
        for(int j=0;j<n_g;j++){
          //filtro per vedere quanti giocatori ci sono nella stanza del giocatore che sta facendo il suo turno
          if(giocatori[j].posizione==giocatori[lista_t[i]].posizione && giocatori[j].nome!=giocatori[lista_t[i]].nome && (giocatori[j].stato==1 || giocatori[j].stato==0))
          contatore_giocatori_stanza++;
        }
        printf("Il tipo dela stanza in cui ti trovi è %s",stampa_tipo_stanza(giocatori[lista_t[i]].posizione->tipo));
        if(contatore_giocatori_stanza!=0){
          printf("\nGli altri giocatori presenti nella stanza numero %p sono:",giocatori[lista_t[i]].posizione);
          for(int j=0;j<n_g;j++){ //stampa i nomi dei giocatori presenti
            if(giocatori[j].posizione==giocatori[lista_t[i]].posizione && giocatori[j].nome!=giocatori[lista_t[i]].nome && (giocatori[j].stato==1 || giocatori[j].stato==0))
            printf(" %s",stampa_nome(giocatori[j].nome));
          }
        }
        if(contatore_giocatori_stanza==0)
        printf("\nSei solo nella stanza %p",giocatori[lista_t[i]].posizione);
        controllo_uccisi=n_imp;
        comando=menu_giocatore(giocatori[lista_t[i]].stato, giocatori[lista_t[i]].nome,giocatori[lista_t[i]].posizione,comando);
        switch (comando) {
          case 1://avanza
          contatore_stanze=avanza(&giocatori[lista_t[i]]);
          break;


          case 2://quest
          contatore_quest=completa_quest(contatore_quest,&giocatori[lista_t[i]]);
          break;


          case 3://emergenza
          n_imp=chiamata_di_emergenza(&giocatori[lista_t[i]]);
          morti++;
          break;

          case 4://uccidi
          n_imp=uccidi(&giocatori[lista_t[i]]);
          if(controllo_uccisi==n_imp+1)
          morti+=2;
          else
          morti++;
          break;


          case 5://botola
          spostati_con_botola(&giocatori[lista_t[i]]);
          break;


          case 6://sabotaggio
          sabotaggio(&giocatori[lista_t[i]]);
          break;
        }
        comando=0;
        if(n_imp==0)
        vittoria=0;
        if(contatore_quest>=quest_da_finire)
        vittoria=0;
        if(n_imp==n_g-morti)
        vittoria=1;
        printf("\nLa preghiamo di non scrivere niente\nLa pagina verrà pulita tra:\n");
        for(int i=3;i>0;i--){ //countdown di 3 secondi
        printf("%d\n",i);
        sleep(1);//pausa prima di usare system("clear")
        }
        system("clear"); //pulisce la pagina
      }
    }
  }
  return vittoria;
}
void termina_gioco(int fine_gioco){
  free(scambio); //dealloco scambio
  scambio=NULL;
  deallocamento();
  switch (fine_gioco) {
    case 0: //vittoria astronauti
    printf("██░ ██  ▄▄▄       ███▄    █  ███▄    █  ▒█████      ██▒   █▓ ██▓ ███▄    █ ▄▄▄█████▓ ▒█████       ▄████  ██▓     ██▓    ▄▄▄        ██████ ▄▄▄█████▓ ██▀███   ▒█████   ███▄    █  ▄▄▄       █    ██ ▄▄▄█████▓ ██▓ \n");
    printf("▓██░ ██▒▒████▄     ██ ▀█   █  ██ ▀█   █ ▒██▒  ██▒   ▓██░   █▒▓██▒ ██ ▀█   █ ▓  ██▒ ▓▒▒██▒  ██▒    ██▒ ▀█▒▓██▒    ▓██▒   ▒████▄    ▒██    ▒ ▓  ██▒ ▓▒▓██ ▒ ██▒▒██▒  ██▒ ██ ▀█   █ ▒████▄     ██  ▓██▒▓  ██▒ ▓▒▓██▒\n");
    printf("▒██▀▀██░▒██  ▀█▄  ▓██  ▀█ ██▒▓██  ▀█ ██▒▒██░  ██▒    ▓██  █▒░▒██▒▓██  ▀█ ██▒▒ ▓██░ ▒░▒██░  ██▒   ▒██░▄▄▄░▒██░    ▒██▒   ▒██  ▀█▄  ░ ▓██▄   ▒ ▓██░ ▒░▓██ ░▄█ ▒▒██░  ██▒▓██  ▀█ ██▒▒██  ▀█▄  ▓██  ▒██░▒ ▓██░ ▒░▒██▒\n");
    printf("░▓█ ░██ ░██▄▄▄▄██ ▓██▒  ▐▌██▒▓██▒  ▐▌██▒▒██   ██░     ▒██ █░░░██░▓██▒  ▐▌██▒░ ▓██▓ ░ ▒██   ██░   ░▓█  ██▓▒██░    ░██░   ░██▄▄▄▄██   ▒   ██▒░ ▓██▓ ░ ▒██▀▀█▄  ▒██   ██░▓██▒  ▐▌██▒░██▄▄▄▄██ ▓▓█  ░██░░ ▓██▓ ░ ░██░\n");
    printf("░▓█▒░██▓ ▓█   ▓██▒▒██░   ▓██░▒██░   ▓██░░ ████▓▒░      ▒▀█░  ░██░▒██░   ▓██░  ▒██▒ ░ ░ ████▓▒░   ░▒▓███▀▒░██████▒░██░    ▓█   ▓██▒▒██████▒▒  ▒██▒ ░ ░██▓ ▒██▒░ ████▓▒░▒██░   ▓██░ ▓█   ▓██▒▒▒█████▓   ▒██▒ ░ ░██░\n");
    printf("▒ ░░▒░▒ ▒▒   ▓▒█░░ ▒░   ▒ ▒ ░ ▒░   ▒ ▒ ░ ▒░▒░▒░       ░ ▐░  ░▓  ░ ▒░   ▒ ▒   ▒ ░░   ░ ▒░▒░▒░     ░▒   ▒ ░ ▒░▓  ░░▓      ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░  ▒ ░░   ░ ▒▓ ░▒▓░░ ▒░▒░▒░ ░ ▒░   ▒ ▒  ▒▒   ▓▒█░░▒▓▒ ▒ ▒   ▒ ░░   ░▓   \n");
    printf("▒ ░▒░ ░  ▒   ▒▒ ░░ ░░   ░ ▒░░ ░░   ░ ▒░  ░ ▒ ▒░       ░ ░░   ▒ ░░ ░░   ░ ▒░    ░      ░ ▒ ▒░      ░   ░ ░ ░ ▒  ░ ▒ ░     ▒   ▒▒ ░░ ░▒  ░ ░    ░      ░▒ ░ ▒░  ░ ▒ ▒░ ░ ░░   ░ ▒░  ▒   ▒▒ ░░░▒░ ░ ░     ░     ▒ ░ \n");
    printf("░  ░░ ░  ░   ▒      ░   ░ ░    ░   ░ ░ ░ ░ ░ ▒          ░░   ▒ ░   ░   ░ ░   ░      ░ ░ ░ ▒     ░ ░   ░   ░ ░    ▒ ░     ░   ▒   ░  ░  ░    ░        ░░   ░ ░ ░ ░ ▒     ░   ░ ░   ░   ▒    ░░░ ░ ░   ░       ▒ ░ \n");
    printf("░  ░  ░      ░  ░         ░          ░     ░ ░           ░   ░           ░              ░ ░           ░     ░  ░ ░           ░  ░      ░              ░         ░ ░           ░       ░  ░   ░               ░   \n");
    printf("                                                        ░                                                                                                                                                        \n");
    break;
    case 1: //vittoria impostori
    printf("██░ ██  ▄▄▄       ███▄    █  ███▄    █  ▒█████      ██▒   █▓ ██▓ ███▄    █ ▄▄▄█████▓ ▒█████       ▄████  ██▓     ██▓    ██▓ ███▄ ▄███▓ ██▓███   ▒█████    ██████ ▄▄▄█████▓ ▒█████   ██▀███   ██▓ \n");
    printf("▓██░ ██▒▒████▄     ██ ▀█   █  ██ ▀█   █ ▒██▒  ██▒   ▓██░   █▒▓██▒ ██ ▀█   █ ▓  ██▒ ▓▒▒██▒  ██▒    ██▒ ▀█▒▓██▒    ▓██▒   ▓██▒▓██▒▀█▀ ██▒▓██░  ██▒▒██▒  ██▒▒██    ▒ ▓  ██▒ ▓▒▒██▒  ██▒▓██ ▒ ██▒▓██▒\n");
    printf("▒██▀▀██░▒██  ▀█▄  ▓██  ▀█ ██▒▓██  ▀█ ██▒▒██░  ██▒    ▓██  █▒░▒██▒▓██  ▀█ ██▒▒ ▓██░ ▒░▒██░  ██▒   ▒██░▄▄▄░▒██░    ▒██▒   ▒██▒▓██    ▓██░▓██░ ██▓▒▒██░  ██▒░ ▓██▄   ▒ ▓██░ ▒░▒██░  ██▒▓██ ░▄█ ▒▒██▒\n");
    printf("░▓█ ░██ ░██▄▄▄▄██ ▓██▒  ▐▌██▒▓██▒  ▐▌██▒▒██   ██░     ▒██ █░░░██░▓██▒  ▐▌██▒░ ▓██▓ ░ ▒██   ██░   ░▓█  ██▓▒██░    ░██░   ░██░▒██    ▒██ ▒██▄█▓▒ ▒▒██   ██░  ▒   ██▒░ ▓██▓ ░ ▒██   ██░▒██▀▀█▄  ░██░\n");
    printf("░▓█▒░██▓ ▓█   ▓██▒▒██░   ▓██░▒██░   ▓██░░ ████▓▒░      ▒▀█░  ░██░▒██░   ▓██░  ▒██▒ ░ ░ ████▓▒░   ░▒▓███▀▒░██████▒░██░   ░██░▒██▒   ░██▒▒██▒ ░  ░░ ████▓▒░▒██████▒▒  ▒██▒ ░ ░ ████▓▒░░██▓ ▒██▒░██░\n");
    printf("▒ ░░▒░▒ ▒▒   ▓▒█░░ ▒░   ▒ ▒ ░ ▒░   ▒ ▒ ░ ▒░▒░▒░       ░ ▐░  ░▓  ░ ▒░   ▒ ▒   ▒ ░░   ░ ▒░▒░▒░     ░▒   ▒ ░ ▒░▓  ░░▓     ░▓  ░ ▒░   ░  ░▒▓▒░ ░  ░░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░  ▒ ░░   ░ ▒░▒░▒░ ░ ▒▓ ░▒▓░░▓   \n");
    printf("▒ ░▒░ ░  ▒   ▒▒ ░░ ░░   ░ ▒░░ ░░   ░ ▒░  ░ ▒ ▒░       ░ ░░   ▒ ░░ ░░   ░ ▒░    ░      ░ ▒ ▒░      ░   ░ ░ ░ ▒  ░ ▒ ░    ▒ ░░  ░      ░░▒ ░       ░ ▒ ▒░ ░ ░▒  ░ ░    ░      ░ ▒ ▒░   ░▒ ░ ▒░ ▒ ░ \n");
    printf("░  ░░ ░  ░   ▒      ░   ░ ░    ░   ░ ░ ░ ░ ░ ▒          ░░   ▒ ░   ░   ░ ░   ░      ░ ░ ░ ▒     ░ ░   ░   ░ ░    ▒ ░    ▒ ░░      ░   ░░       ░ ░ ░ ▒  ░  ░  ░    ░      ░ ░ ░ ▒    ░░   ░  ▒ ░ \n");
    printf("░  ░  ░      ░  ░         ░          ░     ░ ░           ░   ░           ░              ░ ░           ░     ░  ░ ░      ░         ░                ░ ░        ░               ░ ░     ░      ░   \n");
    printf("                                                       ░                                                                                                                                         \n");
    break;
  }
}
void menu(void){
  system("clear");  //pulisco la schermata e poi stampo il menù
  printf("▄▄▄█████▓ ██▀███   ▄▄▄       ███▄    █  ███▄    █  ▒█████   ██▓\n");
  printf("▓  ██▒ ▓▒▓██ ▒ ██▒▒████▄     ██ ▀█   █  ██ ▀█   █ ▒██▒  ██▒▓██▒\n");
  printf("▒ ▓██░ ▒░▓██ ░▄█ ▒▒██  ▀█▄  ▓██  ▀█ ██▒▓██  ▀█ ██▒▒██░  ██▒▒██▒\n");
  printf("░ ▓██▓ ░ ▒██▀▀█▄  ░██▄▄▄▄██ ▓██▒  ▐▌██▒▓██▒  ▐▌██▒▒██   ██░░██░\n");
  printf("  ▒██▒ ░ ░██▓ ▒██▒ ▓█   ▓██▒▒██░   ▓██░▒██░   ▓██░░ ████▓▒░░██░\n");
  printf("  ▒ ░░   ░ ▒▓ ░▒▓░ ▒▒   ▓▒█░░ ▒░   ▒ ▒ ░ ▒░   ▒ ▒ ░ ▒░▒░▒░ ░▓  \n");
  printf("    ░      ░▒ ░ ▒░  ▒   ▒▒ ░░ ░░   ░ ▒░░ ░░   ░ ▒░  ░ ▒ ▒░  ▒ ░\n");
  printf("  ░        ░░   ░   ░   ▒      ░   ░ ░    ░   ░ ░ ░ ░ ░ ▒   ▒ ░\n");
  printf("            ░           ░  ░         ░          ░     ░ ░   ░  \n");
  printf("\nLa preghiamo di ridurre le dimensioni del testo per poter vedere al meglio tutte le possibili scritte\n\n");
  printf("(1) IMPOSTA GIOCO\n");
  printf("(2) GIOCA\n");
  printf("(3) TERMINA GIOCO\n");
}
