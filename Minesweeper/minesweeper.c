/*
   FUNDAMENTOS DE PROGRAMACIÓN
   NOMBRE: RANDY ALEXANDER GERMOSEN UREÑA
   MATRÍCULA: 1013-4707

   - ASIGNACIÓN BUSCAMINAS -
*/

// Librerías Estandarizadas
#include <stdio.h>
#include <stdlib.h>
#include <conio.c>
#include <string.h>
#include <time.h>

// Parámetros de Juego //
#define DEFAULT_MINES      10
#define MAX_MINES          20

// Parámetros de Pantalla
#define WIDTH              80
#define HEIGHT             24

// Parámetros de Campo de Juego //
#define M_WIDTH            32
#define M_HEIGHT           32

// Dimensiones del Tablero //
#define DIM                8

//Parámetros de Teclas y Símbolos //
#define ARROW              -32
#define ARRIBA             72
#define ABAJO              80
#define DERECHA            77
#define IZQUIERDA          75
#define ESC                27
#define ENTER              13
#define BKSP               8
#define PUNTO              '.'
#define ESP                ' '
#define GUION              '-'
#define FLAG               17
#define INTERROGATION      63
#define MINEICON           15
#define DISCOVERED         250

// Máximo tamaño de texto //
#define LEN_ID             10
#define LEN_NAME           40
#define LEN_PASSWORD       40
#define LEN_RECORD         6
#define LEN_LINE           80
#define MIN_ID             3
#define MIN_NAME           3
#define MIN_PASSWORD       8

// Parámetros del Índice del Menú //
#define START_MENU         00
#define MAIN_MENU          01
#define GAME_MENU          02
#define HELP_MENU          03
#define MINE_MENU          04
#define PASSWORD_MENU      05
#define PSWINPUT_MENU      06
#define CONFIRM_MENU       07
#define PLAYER_LIST        10
#define RECORD_LIST        11

// Parámetros de Constantes del Menú //
#define MAX_SHOWED_ITEMS   5
#define MAX_FIELD_ITEMS    5
#define MAX_RECORD_P       20
#define LIST_ITEMS         3

// Parámetros de Posición //
#define POSX_MENU          40
#define POSY_MENU          10
#define POSX_LIST          50
#define POSY_LIST          10
#define POSX_MF            40
#define POSY_MF            5

// Parámetros de Diseño de Texto //
#define LUC                218
#define RUC                191
#define LDC                192
#define RDC                217
#define HL                 196
#define VL                 179

// Parámetros de Color //
#define BACKGROUND         DARKGRAY
#define TXC_TITLE          BLACK
#define BGC_TITLE          WHITE
#define TXC_MENU           LIGHTGRAY
#define BGC_MENU           BLUE
#define TXC_MSELEC         BLACK
#define BGC_MSELEC         YELLOW
#define TXC_FIELD          BLUE
#define BGC_FIELD          LIGHTGRAY
#define BGC_FSELEC         YELLOW
#define TXC_HEADER         LIGHTGRAY
#define BGC_HEADER         BROWN
#define TXC_INFO           WHITE
#define BGC_INFO           GREEN
#define TXC_OFF            DARKGRAY
#define TXC_ERROR          WHITE
#define BGC_ERROR          RED
#define TXC_MF             BLACK
#define BGC_MF             LIGHTGRAY
#define TXC_MFS            BLUE
#define BGC_MFD            DARKGRAY
#define TXC_WIN            BLACK
#define BGC_WIN            CYAN

// Parámetros de Búsqueda de Archivos //
#define FILEPATH_P         "players.dat"
#define FILEPATH_R         "records.dat"
#define PFILE              0
#define RFILE              1

typedef struct
{
// Parámetros Base //
   char id[LEN_ID];
   char name[LEN_NAME];
   char password[LEN_PASSWORD];
   int record;
// Parámetros Juego //
   int activeGame;
   char playerfield[DIM][DIM];
   char minefield[DIM][DIM];
   int steps;

}PLAYER;

typedef struct node
{
   PLAYER player;
   struct node* previous;
   struct node* next;
}NODE;

  //=====================//
 // Funciones del Juego //
//=====================//
char game(PLAYER*);
char checkMinefield(char[][DIM], char[][DIM], int, int);
char checkWin(char[][DIM]);
int checkBestRecord(NODE*, PLAYER);
void setupMinefield(char[][DIM], int);
void modifyPlayer(PLAYER, NODE*);
void returnPlayer(PLAYER*, NODE*);
char convertMinefield(char);
int minesInMinefield(char[][DIM],char[][DIM]);

  //====================//
 // Funciones de Menús //
//====================//
int standardMenu(char[][LEN_LINE], char[][LEN_LINE], int, int*);
int listMenu(NODE*, int, int*);

  //==================================//
 // Funciones de Manejo de Jugadores //
//==================================//
void scanPlayer(NODE**, NODE**, char[][LEN_LINE], int*);
PLAYER searchPlayer(NODE*, int);
int validatePlayer(NODE*, char[][LEN_LINE]);
int validatePassword(char*);

   //=========================================//
  // Funciones de Manejo de Archivos y Nodos //
 //=========================================//
void loadFile(NODE**, NODE**, FILE*, int*, int);
void saveFile(NODE*,FILE*,int);
void insertNode(NODE**, NODE**, PLAYER);
void deleteNode(NODE**, int);
void sortNode(NODE*);
void swap(NODE*, NODE*);
long getfsize(FILE*);

  //====================================//
 // Funciones de Impresión en Pantalla //
//====================================//
void showMenu(char[][LEN_LINE], char[][LEN_LINE], int, int, int, int);
void showList(char[][LEN_LINE], int, int, int, int, int, int, int);
void showField(char*, int, int, int, int, int);
void showMinefield(char[][DIM], int, int, int, int, char);

  //==============================//
 // Funciones de Manejo de Menús //
//==============================//
void captureField(char*, int, char, int*, int, int, int);
void initializeGameMenu(PLAYER, char[][LEN_LINE]);
int bounds(int, int);
int isSpaced(char*);
int isDotted(char*);

  //==========================//
 // Funciones para Coloreado //
//==========================//
void setcolor(int, int);
void colordefault();

  //=======================//
 // Funciones Misceláneas //
//=======================//
void resetField(char[][LEN_LINE], char[][LEN_LINE]);
void defaultMinefield(char[][DIM]);
int randrange(int, int);
void waitEnter();


//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\MAIN//\\//\\//\\//\\//\\//\\//\\//
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
int main()
{
   // Declaraciones de Nodos
   NODE* playerFirst = NULL; NODE* playerLast = NULL;
   NODE* recordFirst = NULL; NODE* recordLast = NULL;
   // Declaraciones de Archivo
   FILE* data = NULL;
   // Cantidad de Elementos en los Nodos
   int nplayer = 0, nrecord = 0;
   // Declaración del Jugador Conectado
   PLAYER onlinePlayer;
   //Declaración de Variables del Menú
   int menu = 0; int loop = 1; int update; int nitems;
   // Declaración de Variable Aleatoria
   srand(time(NULL));
   // Menús predefinidos dentro del Juego

   char startMenu[][LEN_LINE] =
   {
      "   ID   ",
      " Nombre ",
      "                    Terminar                    ",
      "               Seleccionar Perfil               "
   };
   char mainMenu[][LEN_LINE] =
   {
      "                      Juego                     ",
      "                      Ayuda                     ",
      "                      Salir                     "
   };
   char gameMenu[][LEN_LINE] =
   {
      "                  Nueva Partida                 ",
      "                    Continuar                   ",
      "                   Estadisticas                 ",
      "                  Guardar Juego                 ",
      "                      Atras                     "
   };
   char helpMenu[][LEN_LINE] =
   {
      "                 Reglas del Juego               ",
      "                 Acerca del Juego               ",
      "                      Atras                     "
   };
   char mineMenu[][LEN_LINE] =
   {
      " Cantidad de Minas ",
      "                     Terminar                   ",
      "               Por Defecto (10 Minas)           "
   };
   char passwordMenu[][LEN_LINE] =
   {
      " Clave  ",
      "                     Terminar                   ",
      "                Permanecer Anterior             "
   };
   char passwordInputMenu[][LEN_LINE] =
   {
      " Clave  ",
      "                     Terminar                   "
   };
   char confirmMenu[][LEN_LINE] =
   {
      "                     Confirmar                  ",
      "                     Cancelar                   "
   };
   // Campos predefinidos dentro del Juego
   char defaultfield[MAX_FIELD_ITEMS][LEN_LINE] = {0};
   char field[MAX_FIELD_ITEMS][LEN_LINE];

   // Cargar los datos del archivo
   loadFile(&playerFirst,&playerLast,data,&nplayer,PFILE);
   loadFile(&recordFirst,&recordLast,data,&nrecord,RFILE);

   do{
      system("cls");
      resetField(field,defaultfield);
      switch(menu)
      {
         case START_MENU:
            nitems = sizeof(startMenu) / sizeof(startMenu[0]);
            update = standardMenu(startMenu,field,nitems,&menu);
            if(update == FALSE) loop = FALSE;
            if(update == 1)
            {
               if(nplayer)
               {
                  menu = PLAYER_LIST;
               }
               else
               {
                  clrscr();
                  gotoxy(POSX_MENU,POSY_MENU+2);
                  setcolor(TXC_TITLE,BGC_TITLE);
                  printf("                    Aviso                    ");
                  gotoxy(POSX_MENU,POSY_MENU+3);
                  setcolor(TXC_MSELEC,BGC_HEADER);
                  printf("   No se encontraron jugadores registrados   ");
                  colordefault();
                  waitEnter();
               }
            }
            if(update == 2)
            {
               if(validatePlayer(playerFirst,field) == 0)
               {
                  scanPlayer(&playerFirst,&playerLast,field,&nplayer);
                  saveFile(playerFirst,data,PFILE);
                  onlinePlayer = playerLast->player;
                  menu = MAIN_MENU;
               }
               else
               {
                  gotoxy(POSX_MENU,POSY_MENU+5);
                  setcolor(TXC_ERROR,BGC_ERROR);
                  printf("   El ID del jugador ya existe o es muy corto   ");
                  gotoxy(POSX_MENU,POSY_MENU+6);
                  printf("      Elija la opci%cn 'Seleccionar Perfil'      ",162);
                  colordefault();
                  waitEnter();
               }
            }
            break;
         case MAIN_MENU:
            nitems = sizeof(mainMenu) / sizeof(mainMenu[0]);
            update = standardMenu(mainMenu,NULL,nitems,&menu);
            if(update == FALSE) menu = START_MENU;

            break;
         case GAME_MENU:
            nitems = sizeof(gameMenu) / sizeof(gameMenu[0]);
            initializeGameMenu(onlinePlayer,field);
            update = standardMenu(gameMenu,field,nitems,&menu);
            if(update == 1)
            {
               system("cls");
               menu = MINE_MENU;
               nitems = sizeof(mineMenu) / sizeof(mineMenu[0]);
               resetField(field,defaultfield);
               do{
               update = standardMenu(mineMenu,field,nitems,&menu);
                  if(update == -1)
                  {
                     if( atoi(field[0]) > 0 && atoi(field[0]) <= MAX_MINES)
                     {
                        update = atoi(field[0]);
                     }
                     else
                     {
                        gotoxy(POSX_MENU,POSY_MENU+4);
                        setcolor(TXC_ERROR,BGC_ERROR);
                        printf("       El valor debe estar entre '1' y '%d'      ",DIM);
                        colordefault();
                        waitEnter();
                        gotoxy(POSX_MENU,POSY_MENU+4);
                        printf("                                                ");
                        resetField(field,defaultfield);
                     }
                  }
                  if(update == -2)
                  {
                     update = DEFAULT_MINES;
                  }
               }while(update == -1);

               if(update != FALSE)
               {
                  clrscr();
                  onlinePlayer.steps = 0;
                  defaultMinefield(onlinePlayer.minefield);
                  defaultMinefield(onlinePlayer.playerfield);
                  setupMinefield(onlinePlayer.minefield,update);

                  update = game(&onlinePlayer);

                  if(update == 'N')
                  {
                     menu = GAME_MENU;
                  }
                  if(update == 'W')
                  {
                     system("cls");
                     resetField(field,defaultfield);
                     modifyPlayer(onlinePlayer,playerFirst);
                     saveFile(playerFirst,data,PFILE);
                     nitems = sizeof(passwordMenu) / sizeof(passwordMenu[0]);
                     menu = PASSWORD_MENU;

                     if(nrecord == MAX_RECORD_P)
                     {
                        if(checkBestRecord(recordLast,onlinePlayer))
                        {
                           do
                           {
                              update = standardMenu(passwordMenu,field,nitems,&menu);

                              if(update == -1)
                              {
                                 if(validatePassword(field[0]) == FALSE)
                                 {
                                    strcpy(onlinePlayer.password,field[0]);
                                    modifyPlayer(onlinePlayer,playerFirst);
                                    deleteNode(&recordFirst,MAX_RECORD_P-1);
                                    insertNode(&recordFirst,&recordLast,onlinePlayer);
                                    sortNode(recordFirst);
                                    saveFile(playerFirst,data,PFILE);
                                    saveFile(recordFirst,data,RFILE);
                                 }
                                 else
                                 {
                                    gotoxy(POSX_MENU,POSY_MENU+5);
                                    setcolor(TXC_ERROR,BGC_ERROR);
                                    printf("        La clave ingresada es muy corta       ");
                                    colordefault();
                                    update = FALSE;
                                 }
                              }
                              if(update == -2)
                              {
                                 deleteNode(&recordFirst,MAX_RECORD_P-1);
                                 insertNode(&recordFirst,&recordLast,onlinePlayer);
                                 sortNode(recordFirst);
                                 saveFile(recordFirst,data,RFILE);
                              }
                           }while(update == FALSE);
                        }
                     }
                     else
                     {
                        nrecord++;
                        do
                        {
                           update = standardMenu(passwordMenu,field,nitems,&menu);

                           if(update == -1)
                           {
                              if(validatePassword(field[0]) == FALSE)
                              {
                                 strcpy(onlinePlayer.password,field[0]);
                                 modifyPlayer(onlinePlayer,playerFirst);
                                 insertNode(&recordFirst,&recordLast,onlinePlayer);
                                 sortNode(recordFirst);
                                 saveFile(playerFirst,data,PFILE);
                                 saveFile(recordFirst,data,RFILE);
                              }
                              else
                              {
                                 gotoxy(POSX_MENU,POSY_MENU+5);
                                 setcolor(TXC_ERROR,BGC_ERROR);
                                 printf("        La clave ingresada es muy corta       ");
                                 colordefault();
                                 update = FALSE;
                              }
                           }
                           if(update == -2)
                           {
                              insertNode(&recordFirst,&recordLast,onlinePlayer);
                              sortNode(recordFirst);
                              saveFile(recordFirst,data,RFILE);
                           }
                        }while(update == FALSE);
                     }
                     resetField(field,defaultfield);
                     menu = START_MENU;
                  }
                  if(update == 'L')
                  {
                     modifyPlayer(onlinePlayer,playerFirst);
                     saveFile(playerFirst,data,PFILE);
                     resetField(field,defaultfield);
                     menu = GAME_MENU;
                  }
               }
               else
               {
                  menu = GAME_MENU;
                  update = 1;
               }
            }
            if(update == 2)
            {
               clrscr();

               update = game(&onlinePlayer);

               if(update == 'N')
               {
                  menu = GAME_MENU;
               }
               if(update == 'W')
               {
                  system("cls");
                  resetField(field,defaultfield);
                  modifyPlayer(onlinePlayer,playerFirst);
                  saveFile(playerFirst,data,PFILE);
                  nitems = sizeof(passwordMenu) / sizeof(passwordMenu[0]);
                  menu = PASSWORD_MENU;

                  if(nrecord == MAX_RECORD_P)
                  {
                     if(checkBestRecord(recordLast,onlinePlayer))
                     {
                        do
                        {
                           update = standardMenu(passwordMenu,field,nitems,&menu);

                           if(update == -1)
                           {
                              if(validatePassword(field[0]) == FALSE)
                              {
                                 strcpy(onlinePlayer.password,field[0]);
                                 modifyPlayer(onlinePlayer,playerFirst);
                                 deleteNode(&recordFirst,MAX_RECORD_P-1);
                                 insertNode(&recordFirst,&recordLast,onlinePlayer);
                                 sortNode(recordFirst);
                                 saveFile(playerFirst,data,PFILE);
                                 saveFile(recordFirst,data,RFILE);
                              }
                              else
                              {
                                 gotoxy(POSX_MENU,POSY_MENU+5);
                                 setcolor(TXC_ERROR,BGC_ERROR);
                                 printf("        La clave ingresada es muy corta       ");
                                 colordefault();
                                 update = FALSE;
                              }
                           }
                           if(update == -2)
                           {
                              deleteNode(&recordFirst,MAX_RECORD_P-1);
                              insertNode(&recordFirst,&recordLast,onlinePlayer);
                              sortNode(recordFirst);
                              saveFile(recordFirst,data,RFILE);
                           }
                        }while(update == FALSE);
                     }
                  }
                  else
                  {
                     nrecord++;
                     do
                     {
                        update = standardMenu(passwordMenu,field,nitems,&menu);

                        if(update == -1)
                        {
                           if(validatePassword(field[0]) == FALSE)
                           {
                              strcpy(onlinePlayer.password,field[0]);
                              modifyPlayer(onlinePlayer,playerFirst);
                              insertNode(&recordFirst,&recordLast,onlinePlayer);
                              sortNode(recordFirst);
                              saveFile(playerFirst,data,PFILE);
                              saveFile(recordFirst,data,RFILE);
                           }
                           else
                           {
                              gotoxy(POSX_MENU,POSY_MENU+5);
                              setcolor(TXC_ERROR,BGC_ERROR);
                              printf("        La clave ingresada es muy corta       ");
                              colordefault();
                              update = FALSE;
                           }
                        }
                        if(update == -2)
                        {
                           insertNode(&recordFirst,&recordLast,onlinePlayer);
                           sortNode(recordFirst);
                           saveFile(recordFirst,data,RFILE);
                        }
                     }while(update == FALSE);
                  }
                  resetField(field,defaultfield);
                  menu = START_MENU;
               }
               if(update == 'L')
               {
                  modifyPlayer(onlinePlayer,playerFirst);
                  saveFile(playerFirst,data,PFILE);
                  resetField(field,defaultfield);
                  menu = GAME_MENU;
               }
            }
            if(update == 3)
            {
               if(nrecord)
               {
                  menu = RECORD_LIST;
               }
               else
               {
                  clrscr();
                  gotoxy(POSX_MENU,POSY_MENU+2);
                  setcolor(TXC_TITLE,BGC_TITLE);
                  printf("                    Aviso                    ");
                  gotoxy(POSX_MENU,POSY_MENU+3);
                  setcolor(TXC_MSELEC,BGC_HEADER);
                  printf("    No se encontraron records registrados    ");
                  colordefault();
                  waitEnter();
               }
            }
            if(update == 4)
            {
               modifyPlayer(onlinePlayer,playerFirst);
               saveFile(playerFirst,data,PFILE);
               gotoxy(POSX_MENU,POSY_MENU+6);
               setcolor(TXC_INFO,BGC_INFO);
               printf("             Se ha guardado la partida          ");
               gotoxy(POSX_MENU,POSY_MENU+7);
               printf("           Presiona ENTER para continuar        ");
               waitEnter();
               colordefault();

            }
            if(update == FALSE)
            {
               if(onlinePlayer.activeGame == TRUE)
               {
                  system("cls");
                  nitems = sizeof(confirmMenu) / sizeof(confirmMenu[0]);
                  menu = CONFIRM_MENU;
                  update = standardMenu(confirmMenu,NULL,nitems,&menu);

                  if(update == -1)
                  {
                     modifyPlayer(onlinePlayer,playerFirst);
                     saveFile(playerFirst,data,PFILE);
                     gotoxy(POSX_MENU,POSY_MENU+3);
                     setcolor(TXC_INFO,BGC_INFO);
                     printf("             Se ha guardado la partida          ");
                     gotoxy(POSX_MENU,POSY_MENU+4);
                     printf("           Presiona ENTER para continuar        ");
                     waitEnter();
                     colordefault();
                     menu = MAIN_MENU;
                  }
                  else
                  {
                     returnPlayer(&onlinePlayer,playerFirst);
                     menu = MAIN_MENU;
                  }
               }
               else
               {
                  resetField(field,defaultfield);
                  menu = MAIN_MENU;
               }

            }
            break;
         case HELP_MENU:
            nitems = sizeof(helpMenu) / sizeof(helpMenu[0]);
            update = standardMenu(helpMenu,NULL,nitems,&menu);
            if(update == FALSE) menu = MAIN_MENU;
            if(update == 1)
            {
               clrscr();
               gotoxy(POSX_MENU,POSY_MENU);
               setcolor(TXC_TITLE,BGC_TITLE);
               printf("                Reglas del Juego                ");
               gotoxy(POSX_MENU,POSY_MENU+1);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf("              Movimiento del Jugador            ");
               gotoxy(POSX_MENU,POSY_MENU+2);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf("              'Flechas del Teclado'             ");
               gotoxy(POSX_MENU,POSY_MENU+3);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf("             Interactuar con Casilla            ");
               gotoxy(POSX_MENU,POSY_MENU+4);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf("          'Barra Espaciadora' o 'ENTER'         ");
               gotoxy(POSX_MENU,POSY_MENU+5);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf("                  Marcar Casilla                ");
               gotoxy(POSX_MENU,POSY_MENU+6);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf("                    Tecla 'M'                   ");
               gotoxy(POSX_MENU,POSY_MENU+7);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf("                  Pausar Partida                ");
               gotoxy(POSX_MENU,POSY_MENU+8);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf("                     'ESCAPE'                   ");

               colordefault();
               waitEnter();
               system("cls");

               gotoxy(POSX_MENU,POSY_MENU);
               setcolor(TXC_TITLE,BGC_TITLE);
               printf("                Reglas del Juego                ");
               gotoxy(POSX_MENU,POSY_MENU+1);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf(" Interact%ce  con  las   casillas  sin  caer  en ",163);
               gotoxy(POSX_MENU,POSY_MENU+2);
               printf(" ninguna  mina. Los n%cmeros indican  minas  que ",163);
               gotoxy(POSX_MENU,POSY_MENU+3);
               printf(" se encuentran alrededor de una casilla.        ");
               gotoxy(POSX_MENU,POSY_MENU+4);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf(" Se ganar%c  al  descubrir  el  campo  de  minas ",160);
               gotoxy(POSX_MENU,POSY_MENU+5);
               printf(" completo sin caer en ninguna mina.             ");
               gotoxy(POSX_MENU,POSY_MENU+6);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf(" Puede marcar casillas donde crea que hay minas ");
               gotoxy(POSX_MENU,POSY_MENU+7);
               printf(" y descarmarlas con la misma  tecla  cuando  lo ");
               gotoxy(POSX_MENU,POSY_MENU+8);
               printf(" desee.                                         ");
               colordefault();
               waitEnter();

            }
            if(update == 2)
            {
               clrscr();
               gotoxy(POSX_MENU,POSY_MENU);
               setcolor(TXC_TITLE,BGC_TITLE);
               printf("                Acerca del Juego                ");
               gotoxy(POSX_MENU,POSY_MENU+1);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf("                Creador del Juego               ");
               gotoxy(POSX_MENU,POSY_MENU+2);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf("                Randy A. Germos%cn               ",130);
               gotoxy(POSX_MENU,POSY_MENU+3);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf("                   Matr%ccula                    ",161);
               gotoxy(POSX_MENU,POSY_MENU+4);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf("                   1013-4707                    ");
               gotoxy(POSX_MENU,POSY_MENU+5);
               setcolor(TXC_MSELEC,BGC_HEADER);
               printf("           Originalmente Dise%cado por           ",164);
               gotoxy(POSX_MENU,POSY_MENU+6);
               setcolor(TXC_MSELEC,BGC_MSELEC);
               printf("                  Robert Donner                 ");
               colordefault();
               waitEnter();
            }
            break;
         case PLAYER_LIST:
            update = listMenu(playerFirst,nplayer,&menu);
            if(update != -1)
            {
               onlinePlayer = searchPlayer(playerFirst,update);
               if(strcmp(onlinePlayer.password,"0") == 0)
               {
                  menu = MAIN_MENU;
               }
               else
               {
                  system("cls");
                  menu = PSWINPUT_MENU;
                  resetField(field,defaultfield);
                  nitems = sizeof(passwordInputMenu) / sizeof(passwordInputMenu[0]);
                  update = standardMenu(passwordInputMenu,field,nitems,&menu);
                  if(update == -1)
                  {
                     if(strcmp(onlinePlayer.password,field[0]) == 0)
                     {
                        menu = MAIN_MENU;
                     }
                     else
                     {
                        gotoxy(POSX_MENU,POSY_MENU+3);
                        setcolor(TXC_ERROR,BGC_ERROR);
                        printf("        La clave introducida no es v%clida       ",160);
                        colordefault();
                        waitEnter();
                        menu = PLAYER_LIST;
                        resetField(field,defaultfield);
                     }
                  }
                  if(update == FALSE)
                  {
                     menu = PLAYER_LIST;
                     resetField(field,defaultfield);
                  }
               }
            }
            else
            {
               menu = START_MENU;
            }
            break;
         case RECORD_LIST:
            update = listMenu(recordFirst,nrecord,&menu);
            menu = GAME_MENU;
            break;
      }
   }while(loop);

   return 0;
}

/*=============================================================================*/
// Función: game
// Argumentos: (PLAYER* p) = Jugador que entró a la partida
// Objetivo: Dado un jugador por parámetro, permitir jugar su partida
// Retorno: (char) Estado del que se descontinuó la partida, donde:
//          'N' = No terminada | 'W' = Victoria | 'L' = Derrota
/*=============================================================================*/
char game(PLAYER* p)
{
   // Variable de Jugador Activo
   PLAYER player = *p; player.activeGame = TRUE;
   // Variable de Loop
   int loop = TRUE;
   // Variable de Tecla y Posición
   char key; int isArrow; int selectedX = 0; int selectedY = 0;
   // Variable que Indica la condición del jugador
   char status = 'N';
   // Variable de cantidad de Minas en el juego y pasos dados
   int mines = minesInMinefield(player.minefield,player.playerfield) ; int steps = player.steps;
   // Añade las minas al campo

   do{
      showMinefield(player.playerfield,selectedX,selectedY,mines,steps,status);

      isArrow = FALSE;

      if((key = getch()) == ARROW) isArrow = TRUE;
      if(isArrow == TRUE) key = getch();

      if(isArrow == TRUE)
      {
         if(key == ARRIBA)
         {
            if(selectedY)
            {
               selectedY--;
            }
            else
            {
               selectedY = DIM-1;
            }
         }
         if(key == ABAJO)
         {
            if(selectedY != DIM-1)
            {
               selectedY++;
            }
            else
            {
               selectedY = 0;
            }
         }
         if(key == DERECHA)
         {
            if(selectedX != DIM-1)
            {
               selectedX++;
            }
            else
            {
               selectedX = 0;
            }
         }
         if(key == IZQUIERDA)
         {
            if(selectedX)
            {
               selectedX--;
            }
            else
            {
               selectedX = DIM-1;
            }
         }
      }
      else
      {
         if(key == ESP || key == ENTER)
         {
            if(player.playerfield[selectedY][selectedX] != 'F' && player.minefield[selectedY][selectedX] != 'D')
            {
               status = checkMinefield(player.playerfield,player.minefield,selectedX,selectedY);
               if(status == 'L') loop = FALSE;
               else status = checkWin(player.minefield);
               if(status == 'W') loop = FALSE;
               steps++;
            }
         }
         if(key == 'M' || key == 'm')
         {

            switch(player.playerfield[selectedY][selectedX])
            {
               case 'N':
                  player.playerfield[selectedY][selectedX] = 'F';
                  mines--;
                  break;
               case 'F':
                  player.playerfield[selectedY][selectedX] = 'I';
                  mines++;
                  break;
               case 'I':
                  player.playerfield[selectedY][selectedX] = 'N';
                  break;
            }
         }
         if(key == ESC)
         {
            loop = FALSE;
         }
      }

   }while(loop);

   if(status == 'L')
   {
      showMinefield(player.minefield,selectedX,selectedY,mines,steps,status);
      player.activeGame = FALSE;
      waitEnter();
   }
   if(status == 'W')
   {
      showMinefield(player.playerfield,selectedX,selectedY,mines,steps,status);
      player.activeGame = FALSE;
      if(player.record == 0)
      {
         player.record = steps;
      }
      else
      {
         if(steps < player.record)
         player.record = steps;
      }

      waitEnter();
   }

   player.steps = steps;
   *p = player;

   return status;
}

/*=============================================================================*/
// Función: showMinefield
// Argumentos: char minefield[][DIM] = Matriz del campo de minas.
//             int selectedX = Posición en X de la celda seleccionada.
//             int selectedY = Posición en Y de la celda seleccionada.
//             int mines = Cantidad de Minas que se muestran en pantalla.
//             int steps = Cantidad de Pasos que se muestran en pantalla.
//             char status = Condición de la Partida
// Objetivo: Imprimir el pantalla todos los elementos de la partida.
// Retorno: Sin retorno
/*=============================================================================*/
void showMinefield(char minefield[][DIM], int selectedX, int selectedY, int mines, int steps, char status)
{
   // Variable para convertir a simbolos
   char converter;

   gotoxy(POSX_MF,POSY_MF-3);
   setcolor(TXC_TITLE,BGC_TITLE);
   printf("                BUSCAMINAS              ");
   if(status == 'N')
   {
      gotoxy(POSX_MF,POSY_MF-2);
      setcolor(TXC_TITLE,BGC_HEADER);
      printf("                 Pasos: %2d              ",steps);
   }
   if(status == 'L')
   {
      gotoxy(POSX_MF,POSY_MF-2);
      setcolor(TXC_TITLE,BGC_HEADER);
      printf("        Se ha perdido la partida        ");
   }

   if(status != 'W')
   {
      gotoxy(POSX_MF,POSY_MF-1);
      setcolor(TXC_ERROR,BGC_ERROR);
      printf("                 Minas: %3d             ",mines);
   }
   if(status == 'W')
   {
      gotoxy(POSX_MF,POSY_MF-1);
      setcolor(TXC_WIN,BGC_WIN);
      printf("          Has ganado la partida         ");
   }

   colordefault();

   for(int row = 0; row < DIM; row++)
   {
      for(int col = 0; col < DIM; col++)
      {
         converter = convertMinefield(minefield[row][col]);

         if((minefield[row][col] >= '0' && minefield[row][col] <= '9'))
         {
            if(selectedX == col && selectedY == row)
            setcolor(TXC_MFS,BGC_MFD);
            else
            setcolor(TXC_MF,BGC_MFD);
         }
         else
         {
            if(selectedX == col && selectedY == row)
            setcolor(TXC_MFS,BGC_MF);
            else
            setcolor(TXC_MF,BGC_MF);
         }

         if(status == 'L')
         {
            setcolor(BGC_ERROR,BGC_MF);
            if(minefield[row][col] == 'M')
            {
               gotoxy(POSX_MF+col*5,(POSY_MF+row*3)+0);
               printf("%c%c%c%c%c",LUC,HL,HL,HL,RUC);
               gotoxy(POSX_MF+col*5,(POSY_MF+row*3)+1);
               printf("%c %c %c",VL,converter,VL);
               gotoxy(POSX_MF+col*5,(POSY_MF+row*3)+2);
               printf("%c%c%c%c%c",LDC,HL,HL,HL,RDC);
            }
         }
         else
         {
            gotoxy(POSX_MF+col*5,(POSY_MF+row*3)+0);
            printf("%c%c%c%c%c",LUC,HL,HL,HL,RUC);
            gotoxy(POSX_MF+col*5,(POSY_MF+row*3)+1);
            printf("%c %c %c",VL,converter,VL);
            gotoxy(POSX_MF+col*5,(POSY_MF+row*3)+2);
            printf("%c%c%c%c%c",LDC,HL,HL,HL,RDC);

         }
         colordefault();
      }
   }
}

/*=============================================================================*/
// Función: modifyPlayer
// Argumentos: PLAYER player = Jugador a ser actualizado.
//             NODE* first = Cabeza de la lista doblemente enlazada.
// Objetivo: Actualizar los datos del jugador pasado por parámetro dentro de la
//           la lista doblemente enlazada.
// Retorno: Sin retorno
/*=============================================================================*/
void modifyPlayer(PLAYER player, NODE* first)
{
   NODE* index;

   for(index = first; index; index = index->next)
   {
      if(strcmp(player.id,index->player.id) == 0)
      {
         index->player = player;
      }
   }

   return;
}

/*=============================================================================*/
// Función: returnPlayer
// Argumentos: PLAYER* player = Jugador a ser actualizado.
//             NODE* first = Cabeza de la lista doblemente enlazada.
// Objetivo: Devolver los datos del jugador pasado por parámetro a el valor que
//           tenía la última vez que se guardó.
// Retorno: Sin retorno
/*=============================================================================*/
void returnPlayer(PLAYER* player, NODE* first)
{
   NODE* index;

   for(index = first; index; index = index->next)
   {
      if(strcmp(player->id,index->player.id) == 0)
      {
         *player = index->player;
      }
   }

   return;
}

/*=============================================================================*/
// Función: standardMenu
// Argumentos: char menu[][LEN_LINE] = Menú con el que se trabajará.
//             char field[][LEN_LINE] = Campo donde se escribirá el texto.
//             int nitems = Cantidad de elementos del menú.
//             int* menuindex = Índice del menú.
// Objetivo: Función base para el funcionamiento de un menú estandar.
// Retorno: (int) Entero que indica el elemento que se seleccionó en el menú.
/*=============================================================================*/
int standardMenu(char menu[][LEN_LINE], char field[][LEN_LINE], int nitems,int* menuindex)
{
   // Variable de Menú
   int index = *menuindex;
   // Variables de Ciclo y Retorno
   int update = 0; int loop = TRUE;
   // Variables de Tecla y Posición de Cursor
   char key; int isArrow; int cursor = 0; int selected = 0;

   _setcursortype(0);

   do{
      showMenu(menu,field,nitems,selected,index,cursor);

      if(bounds(selected,index) > 0) _setcursortype(100); else _setcursortype(0);

      isArrow = FALSE;

      if((key = getch()) == ARROW) isArrow = TRUE;
      if(isArrow == TRUE) key = getch();

      if(isArrow == TRUE)
      {
         if(key != ARRIBA && key != ABAJO && key != ENTER && key != ESC)
         {
            if(field != NULL) captureField(field[selected],bounds(selected,index),key,&cursor,selected,isArrow,index);
         }
      }
      else
      {
         if(key != ENTER && key != ESC)
         {
            if(field != NULL)
            captureField(field[selected],bounds(selected,index),key,&cursor,selected,isArrow,index);
         }
      }


      if((isArrow == TRUE && (key == ARRIBA || key == ABAJO)) || key == ESC || key == ENTER)
      {
         switch(key)
         {
            case ARRIBA:
               if(isArrow == TRUE)
               {
                  if(selected)
                  {
                     selected--;
                  }
                  else
                  {
                     selected = nitems - 1;
                  }
                  if(index == GAME_MENU)
                  {
                        if(strcmp(field[selected],"F") == 0) selected--;
                  }

                  if(field != NULL) {cursor = (int)strlen(field[selected]);}
               }
            break;

            case ABAJO:
               if(isArrow == TRUE)
               {
                  if(selected < nitems - 1)
                     selected++;
                  else
                     selected = 0;
                  if(index == GAME_MENU)
                  {
                        if(strcmp(field[selected],"F") == 0) selected++;
                  }
                  if(field != NULL) {cursor = (int)strlen(field[selected]);}
               }
            break;

            case ENTER:
               switch(index)
               {
                  case START_MENU:
                     if(selected == 2)
                     {
                        update = 2;
                        loop = FALSE;
                     }
                     if(selected == 3)
                     {
                        update = 1;
                        loop = FALSE;
                     }
                     break;
                  case MAIN_MENU:
                     if(selected == 0)
                     {
                        index = GAME_MENU;
                        update = TRUE;
                        loop = FALSE;
                     }
                     if(selected == 1)
                     {
                        index = HELP_MENU;
                        update = TRUE;
                        loop = FALSE;
                     }
                     if(selected == 2)
                     {
                        update = FALSE;
                        loop = FALSE;
                     }
                     break;
                  case GAME_MENU:
                     if(selected == 0)
                     {
                        update = 1;
                        loop = FALSE;
                     }
                     if(selected == 1)
                     {
                        update = 2;
                        loop = FALSE;
                     }
                     if(selected == 2)
                     {
                        update = 3;
                        loop = FALSE;
                     }
                     if(selected == 3)
                     {
                        update = 4;
                        loop = FALSE;
                     }
                     if(selected == 4)
                     {
                        update = FALSE;
                        loop = FALSE;
                     }
                     break;
                  case HELP_MENU:
                     if(selected == 0)
                     {
                        update = 1;
                        loop = FALSE;
                     }
                     if(selected == 1)
                     {
                        update = 2;
                        loop = FALSE;
                     }
                     if(selected == 2)
                     {
                        update = FALSE;
                        loop = FALSE;
                     }
                     break;
                  case MINE_MENU:
                     if(selected == 1)
                     {
                        update = -1;
                        loop = FALSE;
                     }
                     if(selected == 2)
                     {
                        update = -2;
                        loop = FALSE;
                     }
                     break;
                  case PASSWORD_MENU:
                     if(selected == 1)
                     {
                        update = -1;
                        loop = FALSE;
                     }
                     if(selected == 2)
                     {
                        update = -2;
                        loop = FALSE;
                     }
                     break;
                  case PSWINPUT_MENU:
                     if(selected == 1)
                     {
                        update = -1;
                        loop = FALSE;
                     }
                     break;
                  case CONFIRM_MENU:
                     if(selected == 0)
                     {
                        update = -1;
                        loop  = FALSE;
                     }
                     if(selected == 1)
                     {
                        update = -2;
                        loop = FALSE;
                     }
                     break;
               }
            break;

            case ESC:
               update = FALSE;
               loop = FALSE;
            break;

         }
      }

   }while(loop);

   *menuindex = index;

   return update;
}

/*=============================================================================*/
// Función: listMenu
// Argumentos: NODE* first = Cabeza de la lista doblemente enlazada.
//             int nitems = Cantidad de elementos dentro de la lista.
//             int* menuindex = Índice del menú.
// Objetivo: Función base para el funcionamiento de un menú lista.
// Retorno: (int) Entero que indica el elemento que se seleccionó en el menú.
/*=============================================================================*/
int listMenu(NODE* first, int nitems, int* menuindex)
{
   // Variables de Menú
   int index = *menuindex;
   // Variables de Ciclo y Retorno
   int update = -1; int loop = TRUE;
   // Variables de Tecla y Posición del Cursor
   char key; int counter = 0; int selected = 0;
   // Variables de Scroll
   int mmin = 0; int mmax = MAX_SHOWED_ITEMS;
   // Variables de Formato
   int maxid = 0; int maxname = 0; int idlen; int namelen;
   // Declaración de la Lista (Menú)
   char menu[nitems*LIST_ITEMS][LEN_LINE];

   _setcursortype(0);

   for(NODE* ind = first; ind; ind = ind->next)
   {
      sprintf(menu[counter+0],"%s",ind->player.id);
      sprintf(menu[counter+1],"%s",ind->player.name);
      if(ind->player.record)
         sprintf(menu[counter+2],"%d",ind->player.record);
      else
         sprintf(menu[counter+2],"-");

      idlen = strlen(ind->player.id);
      namelen = strlen(ind->player.name);

      if(idlen > maxid)
         maxid = idlen;
      if(namelen > maxname)
         maxname = namelen;

      counter = counter+LIST_ITEMS;
   }

   do{
      showList(menu,nitems,selected,index,mmin,mmax,maxid,maxname);

      key = getch();

         switch(key)
         {
            case ARRIBA:
               if(selected)
               {
                  selected--;
                  if(nitems > MAX_SHOWED_ITEMS)
                  if(selected == mmin && mmin != 0){mmin--; mmax--; }
               }
               else
               {
                  selected = nitems - 1;
                  if(nitems > MAX_SHOWED_ITEMS)
                  {
                     mmax = nitems;
                     mmin = mmax-MAX_SHOWED_ITEMS;
                  }
               }
            break;

            case ABAJO:
               if(selected < nitems - 1)
               {
                  selected++;
                  if(nitems > MAX_SHOWED_ITEMS)
                  if(selected == mmax && mmax < nitems){mmax++; mmin++;}
               }
               else
               {
                  selected = 0;
                  if(nitems > MAX_SHOWED_ITEMS)
                  {
                     mmin = 0;
                     mmax = MAX_SHOWED_ITEMS;
                  }
               }
            break;

            case ENTER:
               switch(index)
               {
                  case PLAYER_LIST:
                     update = selected;
                     loop = FALSE;
                     break;
                  case RECORD_LIST:
                     loop = FALSE;
                     break;
               }
            break;

            case ESC:
               loop = FALSE;
            break;

         }
   }while(loop);

   *menuindex = index;

   return update;
}

/*=============================================================================*/
// Función: captureField
// Argumentos: char* str = Campo en el que se está realizando la captura.
//             int bounds = Máximo de caracteres permitidos.
//             char key = Tecla presionada.
//             int* cursor = Posición del cursor.
//             int selected = Posición del campo seleccionada.
//             int isArrow = Indicador de si la tecla es una flecha.
//             int menu = Índice del menú.
// Objetivo: Capturar la tecla presionada en el campo pasado por parámetro.
// Retorno: Sin retorno.
/*=============================================================================*/
void captureField(char* str, int bounds, char key, int* cursor, int selected, int isArrow, int menu)
{
   int ind;
   ind = *cursor;

   if( ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9') || key == GUION || key == ESP || key == PUNTO) && ind < bounds && isArrow == FALSE)
   {
      if((menu == START_MENU && (selected == 0 || selected == 1)) || (menu == PASSWORD_MENU && (selected == 0))
         || (menu == PSWINPUT_MENU && (selected == 0)))
      {
         if( (key >= 'a' && key <= 'z') ||  (key >= 'A' && key <= 'Z') || key == GUION || key == ESP)
         {
            if(key == ESP)
            {
               if(isSpaced(str))
               {
                  *(str+ind) = key;
                  ind++;
               }
            }
            else
            {
               *(str+ind) = key;
               ind++;
            }
         }
      }

      if((menu == START_MENU && (selected == 0)) || (menu == MINE_MENU && (selected == 0)) ||
         (menu == PASSWORD_MENU && (selected == 0)) || (menu == PSWINPUT_MENU && (selected == 0)))
      {
         if( (key >= '0' && key <= '9') || key == PUNTO )
         {
            if(key == PUNTO && selected == 5)
            {
               if(isDotted(str))
               {
                  *(str+ind) = key;
                  ind++;
               }
            }
            else
            {
               if(key != PUNTO)
               {
                  *(str+ind) = key;
                  ind++;
               }
            }
         }
      }
   }

   if(key == DERECHA && isArrow == TRUE)
   {
      if ( ind < bounds-1 ) ind++;
   }

   if(key == IZQUIERDA && isArrow == TRUE)
   {
      if ( ind > 0 ) ind--;
   }

   if(key == BKSP)
   {
      if(ind)
      {
         ind--;
         if(selected != 3 && selected != 4)
            strcpy(str+ind,str+ind+1);
         else
         {
            if(selected == 3 || selected == 4)
            {
               if(ind == 2 || ind == 5)
               {
                  ind--;
                  *(str+ind) = ' ';
               }else *(str+ind) = ' ';
            }

         }
      }
   }

   *cursor = ind;
   return;
}

/*=============================================================================*/
// Función: showMenu
// Argumentos: char menu[][LEN_LINE] = Menú con el que se trabajará.
//             char field[][LEN_LINE] = Campo donde se escribirá el texto.
//             int nitems = Cantidad de elementos del menú.
//             int selected = Posición seleccionada por el usuario.
//             int menuindex = Índice del menú.
//             Posición del cursor en un campo de texto.
// Objetivo: Imprimir todo lo que representa el menú estándar en pantalla.
// Retorno: Sin retorno
/*=============================================================================*/
void showMenu(char menu[][LEN_LINE], char field[][LEN_LINE], int nitems, int selected,int menuindex,int cursor)
{
   int ind;
   gotoxy(POSX_MENU,POSY_MENU);
   setcolor(TXC_TITLE,BGC_TITLE);
   switch(menuindex)
   {
      case START_MENU: printf("               Perfil del Jugador               "); break;
      case MAIN_MENU: printf("                  Men%c Principal                ",163); break;
      case GAME_MENU: printf("                  Men%c de Juego                 ",163); break;
      case HELP_MENU: printf("                  Men%c de Ayuda                 ",163); break;
      case MINE_MENU: printf("         Selecciona la Cantidad de Minas        "); break;
      case PASSWORD_MENU: printf("                Clave de Jugador                "); break;
      case PSWINPUT_MENU: printf("           Ingresar Clave de Jugador            "); break;
      case CONFIRM_MENU: printf("   Hay una partida en curso, %cQuiere guardar?   ",168); break;
   }

   for ( ind = 0; ind < nitems; ind ++ )
   {
      setcolor(TXC_MENU,BGC_MENU);
      gotoxy(POSX_MENU,POSY_MENU+ind+1);
      if(selected == ind)
         setcolor(TXC_MSELEC,BGC_MSELEC);
      if(menuindex == GAME_MENU && (strcmp(field[ind],"F") == 0))
         setcolor(TXC_OFF,BGC_MENU);
      printf("%s",menu[ind]);
      if(field != NULL)
      {
         showField(field[ind], bounds(ind,menuindex), POSX_MENU+strlen(menu[ind]), POSY_MENU+ind+1, selected, menuindex);
      }
   }
   colordefault();
   gotoxy(POSX_MENU+strlen(menu[selected])+cursor+1,POSY_MENU+selected+1);

   return;
}

/*=============================================================================*/
// Función: showList
// Argumentos: char menu[][LEN_LINE] = Menú con el que se trabajará.
//             int nitems = Cantidad de elementos del menú.
//             int selected = Posición seleccionada por el usuario.
//             int menuindex = Índice del menú.
//             int mmin = Posición mínima que se mostrará en pantalla.
//             int mmax = Posición máxima que se mostrará en pantalla.
// Objetivo: Imprimir todo lo que representa un menú lista en pantalla.
// Retorno: Sin retorno.
/*=============================================================================*/
void showList(char menu[][LEN_LINE], int nitems, int selected, int menuindex, int mmin, int mmax, int maxid, int maxname)
{
   int ind = 0; int count = 0; int selcount = mmin;
   int space1, space2;
   if(maxname < 7) maxname = 7;
   if(maxid < 2) maxid = 2;
   gotoxy(POSX_LIST,POSY_LIST);
   setcolor(TXC_TITLE,BGC_TITLE);
   space1 = ((8+maxname+maxid+LEN_RECORD)-18)/2;
   if(space1 % 2 == 0)
      space2 = space1;
   else
      space2 = space1+1;
   switch(menuindex)
   {
      case PLAYER_LIST: printf("%-*sLista de Jugadores%-*s",space1," ",space2," "); break;
   }

   setcolor(TXC_HEADER,BGC_HEADER);
   gotoxy(POSX_LIST,POSY_LIST+1);

   printf(" %-*s | %-*s | %-*s ",maxid,"ID",maxname,"Jugador",LEN_RECORD,"Record");
   if(nitems < MAX_SHOWED_ITEMS)
   {
      for ( ind = 0; ind < (nitems*LIST_ITEMS); ind+=LIST_ITEMS )
      {
         setcolor(TXC_MENU,BGC_MENU);
         gotoxy(POSX_LIST,POSY_LIST+count+2);
         if(selected == selcount) setcolor(TXC_MSELEC,BGC_MSELEC);
         printf(" %-*s | %-*s | %*s ",maxid,menu[ind+0],maxname,menu[ind+1],LEN_RECORD,menu[ind+2]);
         count++;
         selcount++;
      }
   }
   else
   {
      for ( ind = mmin*LIST_ITEMS; ind < (mmax*LIST_ITEMS); ind+=LIST_ITEMS )
      {
         setcolor(TXC_MENU,BGC_MENU);
         gotoxy(POSX_LIST,POSY_LIST+count+2);
         if(selected == selcount) setcolor(TXC_MSELEC,BGC_MSELEC);
         printf("%-*s | %-*s | %-*s ",maxid,menu[ind+0],maxname,menu[ind+1],LEN_RECORD,menu[ind+2]);
         count++;
         selcount++;
      }
   }

   colordefault();
   gotoxy(POSX_LIST+strlen(menu[selected])+1,POSY_LIST+selected+1);

   return;
}

/*=============================================================================*/
// Función: showField
// Argumentos: char* str = Campo de texto.
//             int bounds = Cantidad máxima de caracteres del campo.
//             int posX = Posición en X del campo.
//             int posY = Posición en Y del campo.
//             int selected = Índice del campo seleccionado actual.
//             int menu = Índice del menú.
// Objetivo: Mostrar el campo pasado por parámetro en pantalla.
// Retorno: Sin retorno.
/*=============================================================================*/
void showField(char* str, int bounds, int posX, int posY, int selected, int menu)
{
   int ind;
   if(selected == posY-POSY_MENU-1) setcolor(TXC_FIELD, BGC_FSELEC);
   else setcolor(TXC_FIELD, BGC_FIELD);
   for (ind = 0; ind < bounds; ind ++)
   {
      if(menu != PSWINPUT_MENU)
      {
         gotoxy(posX+ind+1,posY);
         if (*(str+ind))
            printf("%c",*(str+ind));
         else
            printf(" ");
      }
      else
      {
         gotoxy(posX+ind+1,posY);
         if (*(str+ind))
            printf("*");
         else
            printf(" ");
      }
   }
   colordefault();
   return;
}

/*=============================================================================*/
// Función: scanPlayer
// Argumentos: NODE** first = Cabeza de la lista doblemente enlazada.
//             NODE** last = Cola de la lista doblemente enlazada.
//             char field[][LEN_LINE] = Campo con información.
//             int* nplayer = Cantidad de jugadores registrados.
// Objetivo: Escanear los datos del jugador del campo pasado por parámetro e
//           insertarlo dentro de la lista doblemente enlazada.
// Retorno: Sin retorno.
/*=============================================================================*/
void scanPlayer(NODE** first, NODE** last, char field[][LEN_LINE], int* nplayer)
{
   int n = *nplayer;
   PLAYER player;

   strcpy(player.id,field[0]);
   strcpy(player.name,field[1]);
   player.record = 0;
   player.steps = 0;
   player.activeGame = FALSE;
   strcpy(player.password,"0");
   defaultMinefield(player.minefield);

   insertNode(first,last,player);
   n++;
   *nplayer = n;
   return;
}

/*=============================================================================*/
// Función: searchPlayer
// Argumentos: NODE* first = Cabeza de la lista doblemente enlazada.
//             int ind = Índice del jugador a buscar.
// Objetivo: Buscar a un jugador dado su índice.
// Retorno: (PLAYER) Retorna al jugador buscado.
/*=============================================================================*/
PLAYER searchPlayer(NODE* first, int ind)
{
   PLAYER player;
   NODE* index;
   int counter = 0;

   for(index = first; index; index = index->next)
   {
      if(counter == ind)
      {
         player = index->player;
      }
      counter++;
   }

   return player;
}

/*=============================================================================*/
// Función: validatePlayer
// Argumentos: NODE* first = Cabeza de la lista doblemente enlazada.
//             char field[][LEN_LINE] = Campo con información a ser verificada.
// Objetivo: Verificar si la ID de un jugador ya existe en los jugadores dentro
//           de la lista doblemente enlazada o si el ID o el Nombre muy cortos.
// Retorno: (int) Entero que indica si la información es válida o no es válida
//           Válida = FALSE (0) | No Válida = TRUE (1)
/*=============================================================================*/
int validatePlayer(NODE* first, char field[][LEN_LINE])
{
   int err = FALSE; NODE* index;

   if(strlen(field[0]) < MIN_ID)
      err = TRUE;

   if(strlen(field[1]) < MIN_NAME)
      err = TRUE;

   for(index = first; index; index = index->next)
   {
      if(strcmp(index->player.id,field[0]) == 0)
      {
         err = TRUE;
      }
   }

   return err;
}

/*=============================================================================*/
// Función: validatePassword
// Argumentos: char* password = Contraseña a validar.
// Objetivo: Validar la contraseña dada por parámetro.
// Retorno: (int) Entero que indica si la contraseña es válida o no es válida
//           Válida = FALSE (0) | No Válida = TRUE (1)
/*=============================================================================*/
int validatePassword(char* password)
{
   int err = FALSE;

   if(strlen(password) < MIN_PASSWORD)
      err = TRUE;

   return err;
}

/*=============================================================================*/
// Función: loadFile
// Argumentos: NODE** first = Cabeza de la lista doblemente enlazada.
//             NODE** last = Cola de la lista doblemente enlazada.
//             FILE* data = Archivo desde donde se cargará la información.
//             int* n = Cantidad de elementos.
//             int option = Opción para saber que archivo se cargará.
//                          PFILE = Jugadores | RFILE = Records.
// Objetivo: Cargar de un archivo la información.
// Retorno: Sin retorno.
/*=============================================================================*/
void loadFile(NODE** first, NODE** last, FILE* data, int* n, int option)
{
   int total = *n;
   PLAYER* players;

   if(option)
   {
      data = fopen(FILEPATH_R,"rb");
   }
   else
   {
      data = fopen(FILEPATH_P,"rb");
   }

   if(data != NULL)
   {
      total = getfsize(data) / sizeof(PLAYER);
      players = (PLAYER*) calloc(total, sizeof(PLAYER));
      fread(players,sizeof(PLAYER),total,data);

      for(int index = 0; index < total; index++)
         insertNode(first,last,players[index]);
   }


   fclose(data);
   *n = total;

   return;
}

/*=============================================================================*/
// Función: saveFile
// Argumentos: NODE* first = Cabeza de la lista doblemente enlazada.
//             FILE* data = Archivo donde se guardará la información.
//             int option = Opción para saber en que archivo se guardará.
//                          PFILE = Jugadores | RFILE = Records.
// Objetivo: Guardar en un archivo la información.
// Retorno: Sin retorno.
/*=============================================================================*/
void saveFile(NODE* first,FILE* data, int option)
{
   NODE* index;

   if(option == PFILE)
   {
      if((data = fopen(FILEPATH_P,"wb")) != NULL)
      {
         for(index = first; index; index = index->next)
         {
            fwrite(&index->player,sizeof(PLAYER),1,data);
         }
      }
   }
   if(option == RFILE)
   {
      if((data = fopen(FILEPATH_R,"wb")) != NULL)
      {
         for(index = first; index; index = index->next)
         {
            fwrite(&index->player,sizeof(PLAYER),1,data);
         }
      }
   }

   fclose(data);

   return;
}

/*=============================================================================*/
// Función: insertNode
// Argumentos: NODE** first = Cabeza de la lista doblemente enlazada.
//             NODE** last = Cola de la lista doblemente enlazada.
//             PLAYER player = Jugador a ser insertado en la lista.
// Objetivo: Insertar un nodo nuevo que contiene al jugador pasado por parámetro
//           al final de la lista doblemente enlazada.
// Retorno: Sin retorno.
/*=============================================================================*/
void insertNode(NODE** first, NODE** last, PLAYER player)
{
   NODE* newNode;

   newNode = (NODE *) malloc(sizeof(NODE));
   newNode->player = player;

   newNode->next = NULL;
   newNode->previous = *last;

   if (*first ==  NULL)
      *first = newNode;
   if (*last)
      (*last)->next = newNode;

   *last = newNode;

   return;
}

/*=============================================================================*/
// Función: deleteNode
// Argumentos: NODE** first = Cabeza de la lista doblemente enlazada.
//             int selected = Índice del nodo a eliminar.
// Objetivo: Dado un índice eliminar el nodo en esa posición.
// Retorno: Sin retorno.
/*=============================================================================*/
void deleteNode(NODE** first, int selected)
{
   NODE *indice; int count = 0;

   for ( indice = *first; indice; indice = indice->next)
   {
      if (count == selected)
      {
         if ( *first == indice )
            *first = (*first)->next;
         else
         {
            indice->previous->next = indice->next;
            indice->next->previous = indice->previous;
         }
         free(indice);
      }
      count++;
   }

   return;
}

/*=============================================================================*/
// Función: sortNode
// Argumentos: NODE* first = Cabeza de la lista doblemente enlazada.
// Objetivo: Ordenar la lista de menor a mayor según el record de el jugador
//           que contengan.
// Retorno: Sin retorno.
/*=============================================================================*/
void sortNode(NODE* first)
{
   NODE *current = NULL, *index = NULL;

   if(first == NULL)
   {
      return;
   }
   else
   {
      for(current = first; current->next != NULL; current = current->next)
      {
         for(index = current->next; index != NULL; index = index->next)
         {
            if(current->player.record > index->player.record)
            {
               swap(current,index);
            }
         }
      }
   }
}

/*=============================================================================*/
// Función: swap
// Argumentos: NODE* a = Nodo 'a' de una lista.
//             NODE* b = Nodo 'b' de una lista.
// Objetivo: Intercambiar la información del nodo 'a' con la del nodo 'b'.
// Retorno: Sin retorno.
/*=============================================================================*/
void swap(NODE* a, NODE* b)
{
   PLAYER temp = a->player;
   a->player = b->player;
   b->player = temp;

   return;
}

/*=============================================================================*/
// Función: bounds
// Argumentos: int index = Posición del menú.
//             int menu = Índice del menú.
// Objetivo: Establecer el máximo de carácteres para el campo de texto.
// Retorno: (int) Entero que indica el máximo de caracteres.
/*=============================================================================*/
int bounds(int index, int menu)
{
   int result = 0;

   switch(menu)
   {
      case START_MENU:
         if(index == 0) result = LEN_ID-1;
         if(index == 1) result = LEN_NAME-1;
         break;

      case MINE_MENU:
         if(index == 0) result = 4;
         break;
      case PASSWORD_MENU:
         if(index == 0) result = LEN_PASSWORD-1;
         break;
      case PSWINPUT_MENU:
         if(index == 0) result = LEN_PASSWORD-1;
         break;
   }

   return result;
}

/*=============================================================================*/
// Función: initializeGameMenu
// Argumentos: PLAYER player = Jugador conectado.
//             char field[][LEN_LINE] = Campo que sirve para indicar que items
//                                      del menú serán seleccionables.
// Objetivo: Inicializar los items seleccionables dada las condiciones que posea
//           el jugador.
// Retorno: Sin retorno.
/*=============================================================================*/
void initializeGameMenu(PLAYER player, char field[][LEN_LINE])
{
   strcpy(field[0],"T");
   strcpy(field[1],"F");
   strcpy(field[2],"T");
   strcpy(field[3],"F");
   strcpy(field[4],"T");

   if(player.activeGame == TRUE)
   {
      strcpy(field[1],"T");
      strcpy(field[3],"T");
   }
}

/*=============================================================================*/
// Función: resetField
// Argumentos: char fielddes[][LEN_LENLINE] = Campo destino.
//             char fieldsource[][LEN_LINE] = Campo fuente.
// Objetivo: Copiar las cadenas de caracter del campo fuente al campo destino.
// Retorno: Sin retorno.
/*=============================================================================*/
void resetField(char fielddes[][LEN_LINE], char fieldsource[][LEN_LINE])
{
   for(int ind = 0; ind < MAX_FIELD_ITEMS; ind++)
   {
      for(int len = 0; len < LEN_LINE; len++)
      {
         fielddes[ind][len] = fieldsource[ind][len];
      }
   }
}

/*=============================================================================*/
// Función: checkMinefield
// Argumentos: char temp[][DIM] = Campo del jugador.
//             char mined[][DIM] = Campo minado.
//             int pX = Posición en X de la celda seleccionada.
//             int pY = Posición en Y de la celda seleccionada.
// Objetivo: Realizar todos los procedimientos que proceden tras interactuar
//           con una celda.
// Retorno: (char) Estado que indica si el jugador a perdido o no.
/*=============================================================================*/
char checkMinefield(char temp[][DIM], char mined[][DIM], int pX, int pY)
{
   // Variables de Estado y Contador de Minas Alrededor
   char status = 'N'; int mymines = 0;

   if(mined[pY][pX] == 'M')
   {
      status = 'L';
   }
   else
   {
      mined[pY][pX] = 'D';

      if(pX != 0)
      {
         if(mined[pY][pX-1] == 'M')
            mymines++;
      }
      if(pX != DIM-1)
      {
         if(mined[pY][pX+1] == 'M')
            mymines++;
      }
      if(pY != 0)
      {
         if(mined[pY-1][pX] == 'M')
            mymines++;
      }
      if(pY != DIM-1)
      {
         if(mined[pY+1][pX] == 'M')
            mymines++;
      }
      if(pX != 0 && pY != 0)
      {
         if(mined[pY-1][pX-1] == 'M')
            mymines++;
      }
      if(pX != DIM-1 && pY != 0)
      {
         if(mined[pY-1][pX+1] == 'M')
            mymines++;
      }
      if(pX != 0 && pY != DIM-1)
      {
         if(mined[pY+1][pX-1] == 'M')
            mymines++;
      }
      if(pX != DIM-1 && pY != DIM-1)
      {
         if(mined[pY+1][pX+1] == 'M')
            mymines++;
      }

      temp[pY][pX] = '0' + mymines;

      if(mymines == 0)
      {
         if(pX != 0)
         {
            if(temp[pY][pX-1] != '0')
               checkMinefield(temp,mined,pX-1,pY);
         }
         if(pX != DIM-1 && temp[pY][pX+1] != '0')
         {
            checkMinefield(temp,mined,pX+1,pY);
         }
         if(pY != 0)
         {
            if(temp[pY-1][pX] != '0')
               checkMinefield(temp,mined,pX,pY-1);
         }
         if(pY != DIM-1)
         {
            if(temp[pY+1][pX] != '0')
               checkMinefield(temp,mined,pX,pY+1);
         }
         if(pX != 0 && pY != 0)
         {
            if(temp[pY-1][pX-1] != '0')
               checkMinefield(temp,mined,pX-1,pY-1);
         }
         if(pX != DIM-1 && pY != 0)
         {
            if(temp[pY-1][pX+1] != '0')
               checkMinefield(temp,mined,pX+1,pY-1);
         }
         if(pX != 0 && pY != DIM-1)
         {
            if(temp[pY+1][pX-1] != '0')
               checkMinefield(temp,mined,pX-1,pY+1);
         }
         if(pX != DIM-1 && pY != DIM-1)
         {
            if(temp[pY+1][pX+1] != '0')
               checkMinefield(temp,mined,pX+1,pY+1);
         }
      }
   }

   return status;
}

/*=============================================================================*/
// Función: checkWin
// Argumentos: char minefield[][DIM] = Campo minado.
// Objetivo: Verificar sin el jugador a terminado el juego y ganado.
// Retorno: (char) Estado que indica si el jugador a ganado o no.
/*=============================================================================*/
char checkWin(char minefield[][DIM])
{
   char status = 'W';

   for(int row = 0; row < DIM; row++)
   {
      for(int col = 0; col < DIM; col++)
      {
         if(minefield[row][col] == 'N')
            status = 'N';
      }
   }

   return status;
}

/*=============================================================================*/
// Función: checkBestRecord
// Argumentos: NODE* last = Cola de la lista doblemente enlazada.
//             PLAYER player = Jugador conectado.
// Objetivo: Verificar si el record del jugador es mayor que el último en la
//           lista doblemente enlazada de records.
// Retorno: (int) Entero que indica si es mayor que el último o no.
/*=============================================================================*/
int checkBestRecord(NODE* last, PLAYER player)
{
   int isRecord = FALSE;

   if(last->player.record < player.record)
      isRecord = TRUE;

   return isRecord;
}

/*=============================================================================*/
// Función: setupMinefield
// Argumentos: char minefield[][DIM] = Campo minado.
//             int maxmines = Cantidad máxima de minas.
// Objetivo: Rellena del campo de minas en posiciones aleatorias hasta que la
//           cantidad de  minas en el campo sea 'maxmines'.
// Retorno: Sin retorno.
/*=============================================================================*/
void setupMinefield(char minefield[][DIM], int maxmines)
{

   int randRow;
   int randCol;
   int minecount = 0;

   while(minecount < maxmines)
   {

      randRow = randrange(0,DIM-1);
      randCol = randrange(0,DIM-1);

      if(minefield[randRow][randCol]  == 'N')
      {
         minecount++;
         minefield[randRow][randCol] = 'M';
      }
   }

}

/*=============================================================================*/
// Función: convertMinefield
// Argumentos: char field = Caracter a ser convertido.
// Objetivo: Convertir el caracter en un simbolo más acertado para ser
//           representado en pantalla.
// Retorno: (char) Símbolo que representará al caracter dado por parámetro.
/*=============================================================================*/
char convertMinefield(char field)
{
   switch(field)
   {
      case 'N': case '0':
         return ESP;
         break;
      case 'F':
         return FLAG;
         break;
      case 'I':
         return INTERROGATION;
         break;
      case 'M':
         return MINEICON;
      case 'D':
         return DISCOVERED;
         break;
   }

   return field;
}

/*=============================================================================*/
// Función: minesInMinefield
// Argumentos: char minefield[][DIM] = Matriz del campo minado.
//             char playerfield[][DIM] = Matriz del campo del jugador.
// Objetivo: Verificar cuantas minas hay en el tablero y cuantas banderas ha
//           puesto el jugador.
// Retorno: (int) Entero con la cantidad de (minas - banderas).
/*=============================================================================*/
int minesInMinefield(char minefield[][DIM], char playerfield[][DIM])
{
   int mines = 0;
   int flags = 0;
   int isFlagged = FALSE;

   for(int i = 0; i < DIM; i++)
   {
      for(int j = 0; j < DIM; j++)
      {
         if(minefield[i][j] == 'M')
            mines++;
      }
   }

   for(int i = 0; i < DIM; i++)
   {
      for(int j = 0; j < DIM; j++)
      {
         if(playerfield[i][j] == 'F')
         {
            isFlagged = TRUE;
            flags++;
         }
      }
   }

   if(isFlagged)
   {
      mines = mines-flags;
   }

   return mines;
}

/*=============================================================================*/
// Función: defaultMinefield
// Argumentos: char minefield[][DIM] = Matriz de campo minado.
// Objetivo: Rellenar la matriz de 'N' que es el valor neutral por defecto.
// Retorno: Sin retorno.
/*=============================================================================*/
void defaultMinefield(char minefield[][DIM])
{
   for(int i = 0; i < DIM; i++)
   {
      for(int j = 0; j < DIM; j++)
      {
         minefield[i][j] = 'N';
      }
   }
}

/*=============================================================================*/
// Función: isSpaced
// Argumentos: char* str = Cadena de caracter cualquiera.
// Objetivo: Verifica si la cadena de caracter contiene un 'Espacio'.
// Retorno: (int) Entero que indica si contiene un 'Espacio' o no.
/*=============================================================================*/
int isSpaced(char* str)
{
   int spaced = 1;
   char* c;

   for(c = str; *c; c++)
   {
      if( *c == ESP) spaced = 0;
   }

   return spaced;
}

/*=============================================================================*/
// Función: isDotted
// Argumentos: char* str = Cadena de caracter cualquiera.
// Objetivo: Verifica si la cadena de caracter contiene un 'Punto'.
// Retorno: (int) Entero que indica si contiene un 'Punto' o no.
/*=============================================================================*/
int isDotted(char* str)
{
   int spaced = 1;
   char* c;

   for(c = str; *c; c++)
   {
      if( *c == PUNTO) spaced = 0;
   }

   return spaced;
}

/*=============================================================================*/
// Función: getfsize
// Argumentos: FILE* pf = Archivo cualquiera.
// Objetivo: Conseguir la cantidad de elementos del archivo.
// Retorno: (long) Entero largo que indica la cantidad de elementos.
/*=============================================================================*/
long getfsize(FILE* pf)
{
   long posent = ftell(pf);
   fseek(pf,0L,SEEK_END);
   long longarch = ftell(pf);
   fseek(pf,posent,SEEK_SET);
   return longarch;
}

/*=============================================================================*/
// Función: waitEnter
// Argumentos: Ninguno.
// Objetivo: Mantener el programa hasta que se presione la tecla 'Enter' o 'ESC'
// Retorno: Sin retorno.
/*=============================================================================*/
void waitEnter()
{
   char key;
   do{
      key = getch();
   }while(key != ENTER && key != ESC);
   return;
}

/*=============================================================================*/
// Función: randrange
// Argumentos: int minimun = Número minimo.
//             int maximum = Número máximo.
// Objetivo: Generar un número aleatorio entre 'minimum' y 'maximum'.
// Retorno: (int) Entero generado aleatoriamente.
/*=============================================================================*/
int randrange(int minimun, int maximum)
{
   int number;

   number = rand() % (maximum + 1 - minimun) + minimun;

   return number;
}

/*=============================================================================*/
// Función: setcolor
// Argumentos: int text = Color de texto.
//             int background = Color de fondo.
// Objetivo: Adjustar el color de texto y fondo de la consola.
// Retorno: Sin retorno.
/*=============================================================================*/
void setcolor(int text, int background)
{
   textcolor(text);
   textbackground(background);
}

/*=============================================================================*/
// Función: colordefault
// Argumentos: Ninguno.
// Objetivo: Adjustar el color de texto y fondo a su color por defecto.
// Retorno: Sin retorno.
/*=============================================================================*/
void colordefault()
{
   setcolor(LIGHTGRAY,BLACK);
}
