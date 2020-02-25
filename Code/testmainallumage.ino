
#include <Adafruit_NeoPixel.h>

const int b_rouge=2; //pin boutons
const int b_jaune=3;
const int b_vert=4;
const int b_cyan=5;
const int b_bleu=6;
const int b_violet=7;
int nbBouton=6;
int boutonListe[6]= {2,3,4,5,6,7};
int b_color[6][3]={{255,0,0},{255,255,0},{0,255,0},{0,255,255},{0,0,255},{155,0,255}}; //permet de connaitre la couleur en fonction du bouton (classé dans le même ordre que les boutons)
int i=0;
int j=0;
const int LED_PIN=8;
const int NUM_LEDS=4;
int reponse[12][4];
int led[12][4]={{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15},{16,17,18,19},{20,21,22,23},{24,25,26,27},{28,29,30,31},{32,33,34,35},{36,37,38,39},{40,41,42,43},{44,45,46,47}};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS,LED_PIN,NEO_GRB + NEO_KHZ800);

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
  strip.begin();
  strip.setBrightness(30);
  strip.show();
  strip.setPixelColor(led[0][0],100,100,100);
  strip.setPixelColor(led[0][1],100,100,100);
  strip.setPixelColor(led[0][2],100,100,100);
  strip.setPixelColor(led[0][3],100,100,100);
  strip.show();
  delay(1000);
  strip.clear();
  strip.show();
  
}

void loop() {
  Serial.println("début");
    
  for (int a=0;a<nbBouton;a++){ //on parcourt la liste des boutons
    if (digitalRead(boutonListe[a])==LOW){ //dès qu'une valeur est à high
      Serial.println(boutonListe[a]); //on l'affiche pour vérifier que tout marche
      for (int c=0;c<nbBouton;c++){
        if (boutonListe[a]==boutonListe[c]){
          Serial.println(boutonListe[c]);
          strip.setPixelColor(led[i][j],b_color[c][0],b_color[c][1],b_color[c][2]);
          strip.show();
        }
      }
      while (digitalRead(boutonListe[a])==LOW){} //tant qu'on appuye sur le bouton rien ne se passe et le programme est en "pause"
      reponse[i][j]=a; //dès qu'on relache le bouton, le programme reprend et on ajoute la réponse à la liste réponse. 
      j+=1;
      /*
      if (boutonListe[a]==b_vert){ //si on appuye sur le bouton vert
        Serial.println("vert");
        strip.setPixelColor(led[i][j],0,255,0); //la led jème led de la ième ligne s'allume en vert
        strip.show();
        }
     else if (boutonListe[a]==b_jaune){ //idem
        Serial.println("jaune");
        strip.setPixelColor(led[i][j],125,125,0);
        strip.show();
        }*/
    } 
  }
}
 
