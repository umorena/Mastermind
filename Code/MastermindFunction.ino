#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <SoftwareSerial.h>;

Servo trappe;

//_____________________________________________________________________________________________________________
//                                                  BLUETOOTH                    
//_____________________________________________________________________________________________________________

#define RXB 17
#define TXB 18
SoftwareSerial BlueT(RXb,TXB);

int bVar=0;
//_____________________________________________________________________________________________________________
//                                                MUSIQUE                       
//_____________________________________________________________________________________________________________

#define RX 15
#define TX 16
SoftwareSerial mySerial(RX,TX);
//all the commands needed in the datasheet(http://geekmatic.in.ua/pdf/Catalex_MP3_board.pdf) 

static int8_t Send_buf[8] = {0} ;//The MP3 player undestands orders in a 8 int string 

                                 //0X7E FF 06 command 00 00 00 EF;(if command =01 next song order)  

#define NEXT_SONG 0X01  
#define PREV_SONG 0X02  
#define CMD_PLAY_W_INDEX 0X03 //DATA IS REQUIRED (number of song) 
#define VOLUME_UP_ONE 0X04 
#define VOLUME_DOWN_ONE 0X05 
#define CMD_SET_VOLUME 0X06//DATA IS REQUIRED (number of volume from 0 up to 30(0x1E)) 
#define SET_DAC 0X17 
#define CMD_PLAY_WITHVOLUME 0X22 //data is needed  0x7E 06 22 00 xx yy EF;(xx volume)(yy number of song) 
#define CMD_SEL_DEV 0X09 //SELECT STORAGE DEVICE, DATA IS REQUIRED 
#define DEV_TF 0X02 //HELLO,IM THE DATA REQUIRED 
#define SLEEP_MODE_START 0X0A 
#define SLEEP_MODE_WAKEUP 0X0B 
#define CMD_RESET 0X0C//CHIP RESET 
#define CMD_PLAY 0X0D //RESUME PLAYBACK 
#define CMD_PAUSE 0X0E //PLAYBACK IS PAUSED 
#define CMD_PLAY_WITHFOLDER 0X0F//DATA IS NEEDED, 0x7E 06 0F 00 01 02 EF;(play the song with the directory \01\002xxxxxx.mp3 
#define STOP_PLAY 0X16 
#define PLAY_FOLDER 0X17// data is needed 0x7E 06 17 00 01 XX EF;(play the 01 folder)(value xx we dont care) 
#define SET_CYCLEPLAY 0X19//data is needed 00 start; 01 close 
#define SET_DAC 0X17//data is needed 00 start DAC OUTPUT;01 DAC no output 

int16_t music[] = {0X0F00701,0X0F00702,0X0F00703,0X0F00704,0X0F00705,0X0F00706,0X0F00707};
int choixM=666;

void sendCommand(int8_t command, int16_t dat) { 
  delay(20); 
  Send_buf[0] = 0x7e; //starting byte 
  Send_buf[1] = 0xff; //version 
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte 
  Send_buf[3] = command; // 
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback 
  Send_buf[5] = (int8_t)(dat >> 8);//datah 
  Send_buf[6] = (int8_t)(dat); //datal 
  Send_buf[7] = 0xef; //ending byte 
  for(uint8_t i=0; i<8; i++){ 
    mySerial.write(Send_buf[i]) ; 
  } 
} 

//_____________________________________________________________________________________________________________
//                                                  BOUTONS                        
//_____________________________________________________________________________________________________________

const int b_rouge=2; //pin boutons
const int b_jaune=3;
const int b_vert=4;
const int b_cyan=5;
const int b_bleu=6;
const int b_violet=7;
int nbBouton=6;
int boutonListe[6]= {2,3,4,5,6,7};
int b_color[6][3]={{255,0,0},{255,255,0},{0,255,0},{0,255,255},{0,0,255},{155,0,255}}; //permet de connaitre la couleur en fonction du bouton (classé dans le même ordre que les boutons)

//_____________________________________________________________________________________________________________
//                                                  LEDS                     
//_____________________________________________________________________________________________________________


int ledAnaly[12][4]={{0,1,3,2},{4,5,7,6},{8,9,11,10},{12,13,15,14},{16,17,19,18},{20,21,23,22},{24,25,27,26},{28,29,31,30},{32,33,35,34},{36,37,39,38},{40,41,43,42},{44,45,47,46}};//liste de toutes les leds
int led[12][4]={{0,1,2,3},{7,6,5,4},{8,9,10,11},{15,14,13,12},{16,17,18,19},{23,22,21,20},{24,25,26,27},{31,30,29,28},{32,33,34,35},{39,38,37,36},{40,41,42,43},{47,46,45,44}};
int ledSol[4]={0,1,2,3}; //leds solution (dans la trappe)

const int LED_PIN=8;//pin leds
const int LED_PIN_SOL=9;//pin leds sol
const int LED_ANALY_PIN=10; //pin leds analyseuses

const int NUM_LEDS=48; //nb leds
const int NUM_LEDS_SOL=4;//nb leds sol
const int NUM_LEDS_ANALY=48; //nb leds analyseuses

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS,LED_PIN,NEO_GRB + NEO_KHZ800); //bande de leds
Adafruit_NeoPixel stripSol = Adafruit_NeoPixel(NUM_LEDS_SOL,LED_PIN_SOL,NEO_GRB + NEO_KHZ800); //bande de leds solution (trappe)
Adafruit_NeoPixel stripAnaly = Adafruit_NeoPixel(NUM_LEDS_ANALY,LED_ANALY_PIN,NEO_GRB + NEO_KHZ800); //bande de leds analyseuses 

//_____________________________________________________________________________________________________________
//                                                  OPTIONS                        
//_____________________________________________________________________________________________________________

const int restart=11;
const int gm=12; //gamemode
const int undo=13;
String etat;
String etatTab[] = {"solo","duo"};

//_____________________________________________________________________________________________________________
//                                                  SETUP                        
//_____________________________________________________________________________________________________________

boolean finDuJeu = false;
int i=0; //ligne  
int j=0; //colonne
int vert=0; //nb de LEDS allumées pour bonne couleur bon placement
int rouge=0; //nb de LEDS allumées pour bonne couleur mauvais placement

//_____________________________________________________________________________________________________________
//                                                  REPONSE                        
//_____________________________________________________________________________________________________________

int reponse[12][4]; //liste de toutes les réponses (ici on garde pour chaque ligne)
int sol[4]={9,9,9,9}; //etat initial, 9 ne correspond à aucune des valeurs possibles
int solVar[4]; //liste temporaire des solutions (voir suite du code)
int reponseVar[4]; //liste temporaire des réponses (voir suite du code)



void setup() {
  
  mySerial.begin(9600);//Start our Serial coms for our serial monitor! 
  delay(500);//Wait chip initialization is complete 
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card   
  int choixM=666;
  delay(200);

  
  BlueT.begin(9600);
  Serial.begin(9600);
  trappe.attach(14);
  trappe.write(190);
  etat=etatTab[digitalRead(gm)];
  
  pinMode(b_rouge,INPUT);
  pinMode(b_vert,INPUT);
  pinMode(b_jaune,INPUT);
  pinMode(b_cyan,INPUT);
  pinMode(b_bleu,INPUT);
  pinMode(b_violet,INPUT);
  pinMode(restart,INPUT);
  pinMode(gm,INPUT);
  
  stripSol.begin();
  stripAnaly.begin();
  strip.begin();
  stripSol.setBrightness(70);
  strip.setBrightness(70);
  strip.show();
  stripAnaly.setBrightness(70);
  stripAnaly.show();
  stripSol.show();


//_____________________________________________________________________________________________________________
//                                                  SETUP LEDS                        
//_____________________________________________________________________________________________________________

  int l=0;
  int cpt=0;
  for (int k=0; cpt<NUM_LEDS;k++){ //pour allumer toutes les leds au démarrage
    if (k==4){
      l+=1;
      k=0;
    }
    strip.setPixelColor(led[l][k],100,100,100);
    cpt+=1;
  }
  cpt=0;
  l=0;
  for (int k=0; cpt<NUM_LEDS;k++)
  {                                     //pour allumer toutes les leds au démarrage
    if (k==4){
      l+=1;
      k=0;
    }
    stripAnaly.setPixelColor(ledAnaly[l][k],100,100,100);
    cpt+=1;
  }  
  cpt=0;
  
  for (cpt;cpt<4;cpt++)
    stripSol.setPixelColor(ledSol[cpt],100,100,100);
  
  strip.show();
  stripSol.show();
  stripAnaly.show();
  
  delay(1000);
  
  strip.clear();
  stripAnaly.clear();
  stripSol.clear();
  
  strip.show();
  stripSol.show();
  stripAnaly.show();
  
  delay(1000);
  
}

//_____________________________________________________________________________________________________________
//                                                  FONCTIONS                        
//_____________________________________________________________________________________________________________

void _blueT()
{
  bVar=0;
  if (BlueT.available())
  {
    bVar=BlueT.read();
  }
}

//************************************************************************************************************//

void _restart()
{
  if (digitalRead(restart)==LOW)
  {
    for (int i=0;i<4;i++)
    {
      sol[i]=9;
    }
    if (finDuJeu==true)
    {
      sendCommand(STOP_PLAY, music[choixM]); 
      trappe.write(190); 
      delay(50);
      finDuJeu=false;
    }
    
    strip.clear(); //efface tout
    stripSol.clear();
    stripAnaly.clear();
    
    strip.show();
    stripSol.show();
    stripAnaly.show();
    i=0; //valeur à 0
    j=0;
    
    while (digitalRead(restart)==LOW){} // tant qu'on appuye dessus
    delay(70);
    etat=etatTab[digitalRead(gm)];
  }   
}

//****************************************************************************************************************************//

void _choix()
{
  for (int a=0;a<nbBouton;a++)                                               //on parcourt la liste des boutons  
  { 
    if ((digitalRead(boutonListe[a])==LOW)||(boutonListe[a]==bVar))          //dès qu'un bouton est appuyé
    { 
      for (int c=0;c<nbBouton;c++)
      {
        if (boutonListe[a]==boutonListe[c])
        {
          strip.setPixelColor(led[i][j],b_color[c][0],b_color[c][1],b_color[c][2]);
          strip.show();
        }
      }
      while (digitalRead(boutonListe[a])==LOW){}          //tant qu'on appuye sur le bouton rien ne se passe et le programme est en "pause"
      delay(70);
      reponse[i][j]=boutonListe[a];                         //dès qu'on relache le bouton, le programme reprend et on ajoute la réponse à la liste réponse. 
      j+=1;
    } 
  }
}

//****************************************************************************************************************************//

void _comparaison()
{
  for (int sVar=0;sVar<4;sVar++)                //on stocke les valeurs dans une liste temporaire, pour pouvoir supprimer la valeur une fois qu'elle a été analysée
  {                
    solVar[sVar]=sol[sVar];
  }
  for (int rVar=0;rVar<4;rVar++)                //idem pour la réponse
  { 
    reponseVar[rVar]=reponse[i][rVar];
  }
  
  vert=0;
  rouge=0;
  for(int a=0;a<4;a++)                             //chaque élément des réponses va parcourir la liste de la solution et comparer les valeurs 
  {                
    for (int b=0;b<4;b++)
    {
      if ((reponseVar[a]==solVar[b]) && (a==b))    //si bonne réponse bon emplacement
      {                                  
        vert+=1;
        solVar[b]=9;                             //on supprime de la liste les valeurs qui ont été analysées pour ne pas fausser le résultat, cad ne pas compter deux fois la même valeur
        reponseVar[a]=10;                        //comme par exemple si la solution est (1,1,1,1) et qu'on rentre (1,2,3,4), il faut compter le 1 qu'une seule fois 
      }
    }
  }
  for(int a=0;a<4;a++)                                   //maintenant on refait mais pour connaitre les bonnes couleurs mauvais emplacements
  { 
    for (int b=0;b<4;b++)
    {
      if ((reponseVar[a]==solVar[b]) && (a!=b))           //bonne couleur mauvais emplacement
      {                
        rouge+=1; 
        solVar[b]=9;                                      //cette fois-ci on remplace uniquement celle de la solution par 9 
      }
    }
  }
}

//****************************************************************************************************************************//

void _affichageFinTour()
{
  int jA=0;
  for (int vertVar=0;vertVar<vert;vertVar++)
  {
    stripAnaly.setPixelColor(ledAnaly[i][jA],0,255,0);
    jA+=1;
  }
  for (int rougeVar=0;rougeVar<rouge;rougeVar++)
  {
    stripAnaly.setPixelColor(ledAnaly[i][jA],255,0,0);
    jA+=1;
  }
  stripAnaly.show();
}

//****************************************************************************************************************************//

void _choixSolutionDuo()
{
  int b=0;                                          //variable permettant de compter les ledSol
  while (sol[b]==9)
  {
    _blueT();
    for (int a=0;a<nbBouton;a++)                     //on parcourt la liste des boutons
    { 
      if ((digitalRead(boutonListe[a])==LOW)||(boutonListe[a]==bVar))       //dès qu'une valeur est à high
      {         
        for (int c=0;c<nbBouton;c++)                 //boucle permettant de parcourir les couleurs des boutons
        { 
          if (boutonListe[a]==boutonListe[c])        //on repère quel bouton est cliqué 
          {        
            stripSol.setPixelColor(ledSol[b],b_color[c][0],b_color[c][1],b_color[c][2]); //puis on allume la led numero b, de la couleur trouvé en c (numéro du bouton dans la liste)
            stripSol.show(); 
            sol[b]=boutonListe[a];                   //on ajoute le bouton cliqué à la liste solution
          }
        }
        b+=1;
        while (digitalRead(boutonListe[a])==LOW){}   //tant qu'on appuye sur ce bouton
        delay(70);     
      } 
    } 
  }
}

//****************************************************************************************************************************//

void _choixSolutionSolo()                       //si joueur en solo  
{                 
    for (int b=0;b<4;b++)
    {   
      int r=random(0,5);                                                                   //génération d'un chiffre aléatoire (à modifier)
      stripSol.setPixelColor(ledSol[b],b_color[r][0],b_color[r][1],b_color[r][2]);        //puis on allume la led numero b, de la couleur trouvé en c (numéro du bouton dans la liste)
      stripSol.show();
      sol[b]=boutonListe[r];                                                                //ajout du bouton, correspondant à l'emplacement du chiffre, à la liste solution
    }
}

//****************************************************************************************************************************//

void _undo()
{
  if (j!=0)                                                                   //si on est pas au début de ligne
  {
    j-=1;                                                                     //on revient à la dernière colonne
    strip.setPixelColor(led[i][j],0,0,0);                                     //et on éteint la led jouée précedemment
    strip.show();                                                             //on l'affiche
    while (digitalRead(undo)==LOW){}                                          //tant qu'on appuye rien ne se passe
  }
}


//_____________________________________________________________________________________________________________
//                                                  MAIN                       
//_____________________________________________________________________________________________________________

void loop() {
 /* RESTART */
  _blueT();
  if ((digitalRead(restart)==LOW)||(bVar==11))           
  {
    _restart();
  }
  
  /* SI CHANGEMENT DE JOUEUR */
  if (etat!=etatTab[digitalRead(gm)])       //si l'etat stockant le nb de joueurs à changer en cours de jeu
  {
    _restart();
  }

  /* GENERATION SOLUTION SOLO/DUO */
  
  if (sol[0]==9){                           //initialement, forcément à 9 
    if (etat=="solo")                    //si joueur en solo
    { 
      _choixSolutionSolo();
    } 
    
    else if (etat=="duo")                  //si joueur en duo
    {
      _choixSolutionDuo();
    }
  }
  
  /* JEU PRINCIPAL */
  else                                              // Si la solution existe déjà, c-a-d que la partie est en cours
  {                                        
    if ((digitalRead(undo)==LOW)||(bVar=13))        //pour annuler le mouvement précédent 
    {
      _undo();
    }
    _choix();
    
    /* FIN TOUR */
    /* ANALYSE FIN DE LIGNE*/  
    if (j==4)                         
    {
      _comparaison();                 /* COMPARAISON REPONSE SOLUTION */
      _affichageFinTour();            /* AFFICHAGE DES RESULTATS */
      if (vert==4)                    //si 4 couleurs trouvées
      {
        finDuJeu=true;            
        for (int position = 190; position >=80;position --)        //ouverture trappe
        {
          trappe.write(position);
          delay(10);
        }
        choixM=random(4,6);                                    
        sendCommand(CMD_PLAY_WITHFOLDER, music[choixM]);           //joue une des musiques gagnantes
      }
      
      else 
      {
        i+=1;                                                       //on passe à la ligne suivante
        j=0;                                                        //on revient à la première colonne
      }

      if (i>=12)                                                    //on vient de finir la 12ème ligne
      {
        finDuJeu=true;
        for (int position = 190; position >=80;position --)
        {
          trappe.write(position);
          delay(10);
        }
        choixM=random(0,4);
        sendCommand(CMD_PLAY_WITHFOLDER, music[choixM]);           //si on est arrivé après la dernière ligne 
      }
      if (finDuJeu==true)                                          //si le jeu est fini on recommence
      {
        while ((digitalRead(restart)==HIGH)||(bVar!=11))
        {
          _blueT();
        }                       //tant qu'on ne recommence pas, rien ne se passe
        _restart();
      }
      
    }
  }
}
