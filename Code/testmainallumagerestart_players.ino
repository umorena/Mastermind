#include <Adafruit_NeoPixel.h>

const int b_rouge=2; //pin boutons
const int b_jaune=3;
const int b_vert=4;
const int b_cyan=5;
const int b_bleu=6;
const int b_violet=7;
const int restart=11;
const int gm=12; //gamemode
int gminit;
int nbBouton=6;
String etat;
String etatTab[] = {"solo","duo"};
int boutonListe[6]= {2,3,4,5,6,7};
int b_color[6][3]={{255,0,0},{255,255,0},{0,255,0},{0,255,255},{0,0,255},{155,0,255}}; //permet de connaitre la couleur en fonction du bouton (classé dans le même ordre que les boutons)
int i=0; //ligne  
int j=0; //colonne
const int LED_PIN=8;
const int LED_PIN_SOL=9;
const int NUM_LEDS=8;
const int NUM_LEDS_SOL=4;
int reponse[12][4]; //liste de toutes les réponses
int led[12][4]={{0,1,2,3},{7,6,5,4},{8,9,10,11},{12,13,14,15},{16,17,18,19},{20,21,22,23},{24,25,26,27},{28,29,30,31},{32,33,34,35},{36,37,38,39},{40,41,42,43},{44,45,46,47}};//liste de toutes les leds
int sol[4]={9,9,9,9}; //etat initial, 9 ne correspond à aucune des valeurs possibles
int ledSol[4]={0,1,2,3}; //leds solution (dans la trappe)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS,LED_PIN,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripSol = Adafruit_NeoPixel(NUM_LEDS_SOL,LED_PIN_SOL,NEO_GRB + NEO_KHZ800); //bande de leds solution (trappe)

void setup() {
  Serial.begin(9600);
  i=0;
  j=0;
  pinMode(b_rouge,INPUT);
  pinMode(b_vert,INPUT);
  pinMode(b_jaune,INPUT);
  pinMode(b_cyan,INPUT);
  pinMode(b_bleu,INPUT);
  pinMode(b_violet,INPUT);
  pinMode(restart,INPUT);
  pinMode(gm,INPUT);
  stripSol.begin();
  strip.begin();
  stripSol.setBrightness(30);
  strip.setBrightness(30);
  strip.show();
  stripSol.show();
  int l=0;
  int ct=0;
  for (int k=0; ct<NUM_LEDS;k++){
    if (k==4){
      l+=1;
      k=0;
    }
    strip.setPixelColor(led[l][k],100,100,100);
    ct+=1;
  }
  strip.show();
  delay(1000);
  strip.clear();
  strip.show();
  stripSol.setPixelColor(ledSol[0],100,100,100);
  stripSol.setPixelColor(ledSol[1],100,100,100);
  
  stripSol.setPixelColor(ledSol[2],100,100,100);
  stripSol.setPixelColor(ledSol[3],100,100,100);
  stripSol.show();
  delay(1000);
  
  stripSol.clear();
  stripSol.show();
  etat=etatTab[digitalRead(gm)];
}

void loop() {

  Serial.println(digitalRead(gm));
  Serial.println(etat);
  if (digitalRead(restart)==LOW) {
    
    for (int i=0;i<4;i++){
      sol[i]=9;
    } 
    strip.clear();
    strip.show();
    i=0;
    j=0;
    while (digitalRead(restart)==LOW){} // tant qu'on appuye dessus
    Serial.println("restart");
    delay(500);
    }
  if (etat!=etatTab[digitalRead(gm)]){
    Serial.println("gm change");
    for (int i=0;i<4;i++){
      sol[i]=9;
    } 
    stripSol.clear();
    strip.clear();
    //stripAnaly.clear();
    stripSol.show();
    strip.show();
    //stripAnaly.show();
    i=0;
    j=0;
    etat=etatTab[digitalRead(gm)];
  }
  
  if (sol[0]==9){ //initialement, forcément à 9 
    if (etat=="solo"){ //si joueur en solo  
    Serial.println(etat);
    for (int b=0;b<4;b++){ 
      int r=random(0,5); //génération d'un chiffre aléatoire
      stripSol.setPixelColor(ledSol[b],b_color[r][0],b_color[r][1],b_color[r][2]); //puis on allume la led numero b, de la couleur trouvé en c (numéro du bouton dans la liste)
      stripSol.show();
      sol[b]=boutonListe[r]; //ajout du bouton, correspondant à l'emplacement du chiffre, à la liste solution
      delay(500);
      }
    }
    else if (etat=="duo"){
      Serial.println(etat);
      delay(500);
      int b=0; //variable permettant de compter les ledSol
      while (sol[b]==9){
        for (int a=0;a<nbBouton;a++){ //on parcourt la liste des boutons
          if (digitalRead(boutonListe[a])==LOW){ //dès qu'une valeur est à high
            //Serial.println(boutonListe[a]); //on l'affiche pour vérifier que tout marche
            for (int c=0;c<nbBouton;c++){ //boucle permettant de parcourir les couleurs des boutons
              if (boutonListe[a]==boutonListe[c]){ //on repère quel bouton est cliqué 
                //Serial.println(boutonListe[c]); //affichage pour vérifier
                stripSol.setPixelColor(ledSol[b],b_color[c][0],b_color[c][1],b_color[c][2]); //puis on allume la led numero b, de la couleur trouvé en c (numéro du bouton dans la liste)
                stripSol.show(); 
                sol[b]=boutonListe[a]; //on ajoute le bouton cliqué à la liste solution
              }
            }
            b+=1;
            while (digitalRead(boutonListe[a])==LOW){} //tant qu'on appuye sur ce bouton
          }
        }
      }
    }
  } 
  else { 
    Serial.println("début");
    
    for (int a=0;a<nbBouton;a++){ //on parcourt la liste des boutons
      if (digitalRead(boutonListe[a])==LOW){ //dès qu'une valeur est à high
        //Serial.println(boutonListe[a]); //on l'affiche pour vérifier que tout marche
        for (int c=0;c<nbBouton;c++){
          if (boutonListe[a]==boutonListe[c]){
            //Serial.println(boutonListe[c]);
            strip.setPixelColor(led[i][j],b_color[c][0],b_color[c][1],b_color[c][2]);
            strip.show();
          }
        }
        while (digitalRead(boutonListe[a])==LOW){} //tant qu'on appuye sur le bouton rien ne se passe et le programme est en "pause"
        reponse[i][j]=a; //dès qu'on relache le bouton, le programme reprend et on ajoute la réponse à la liste réponse. 
        j+=1;
      } 
    }
  }
}
