enum chiamata_emergenza{non_effettuata,effettuata};
enum nome_g{verde,giallo,blu,rosso,viola,rosa,celeste,nero,bianco,arancione};
enum stato_g{astronauta,impostore,assassinato,defenestrato};
enum tipo_stanza{vuota,quest_semplice,quest_complicata,botola};

 struct Stanza{
  enum tipo_stanza tipo;
  enum chiamata_emergenza emergenza;
  struct Stanza *avanti;
  struct Stanza *sinistra;
  struct Stanza *destra;
  struct Stanza *stanza_precedente;
  struct Stanza *cronologia;
};

typedef struct{
  struct Stanza *posizione;
  enum nome_g nome;
  enum stato_g stato;
} Giocatori;

void imposta_gioco(void);
int gioca(void);
void termina_gioco(int fine_gioco);
void menu(void);
