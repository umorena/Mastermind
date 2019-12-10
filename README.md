# Mastermind
Mastermind project arduino
DESCRIPTION PROJET ARDUINO - MASTERMIND

Gubeno Manon - PeiP2 Groupe 2
Morena Ugo - PeiP2 Groupe 2


Description générale :

Idée principale : Reprendre le concept du jeu Mastermind sous une forme électronique à l’aide d’Arduino. 

Notre idée de base est d’avoir une caisse (en bois) laquelle laisserait apparaître l’interface du jeu (rubans de LED pour les combinaisons de couleur choisies par le jouer, signaux lumineux pour indiquer le nombre de bonnes et mauvaises réponses etc..)




















Dans l’idéal, on peut s’imaginer le mastermind comme composé d’une matrice 4*12 leds RGB sur les lignes principales + une matrice 4*12 leds RGB sur les lignes secondaires (= indiquent le nombre de bonnes/mauvaises réponses).

6 COULEURS DIFFÉRENTES DE LEDS :

-	Rouge   o
-	Bleu   o
-	Vert   o
-	Jaune   o
-	Orange   o
-	Violet   o

Nous avons dans un premier temps pensé à utiliser des leds « classiques » RGB mais cela nous demanderait un nombre trop important de ports pour les brancher, il existe à la place des bandes de LEDS, permettant une utilisation plus simple.



Caisse / Support : 

Maquette : en PVC, recherche des dimensions 
Version finale : en bois

La boîte laissera apparaître toutes les LEDS et les boutons de choix des couleurs
Ajout d’une petite trappe qui s’ouvre lorsque le joueur trouve la bonne combinaison de couleur pour laisser apparaître la solution de la partie.


Développement mode de jeu :

Deux types de parties : 

-	Solution choisie par un autre joueur (duo)
-	Solution déterminée aléatoirement (solo)
 
Deux types de mode de jeu :

-	Utilisation des boutons matériels situés sur la boîte
-	Choix des couleurs sur smartphone 

Lancement d’une nouvelle partie : Interrupteur (pour allumer le mastermind)
Choix du nombre de joueurs/type de parties : Interrupteur
Recommencer la partie : bouton matériel


Premières idées de conception : 

•	Utiliser un circuit imprimé pour brancher directement les leds dessus 
Contrainte : demande un grand nombre de pins pour les leds.
•	Utiliser des bandes de leds pour chaque ligne.
•	Utiliser un panneau de leds.

Idées très avancées :
•	Utiliser des capteurs de couleur qui reconnait la couleur de la bille posée à son emplacement. 
Problème : une fois la bille posée il n’y a plus de lumière qui passe, ainsi il faudrait rajouter des “lampes” qui permettent de voir la couleur.
•	Utiliser des billes avec des aimants, qui une fois posées ne peut plus être enlevée jusqu’à la fin de la partie.
Problème : réussir à détecter la couleur.
Nous pensons que le plus efficace sans être la solution de facilité est d’utiliser des bandes de leds et d’allumer progressivement au fur et à mesure de la partie.


Comment ouvrir la trappe ? 

•	Mettre dans le trou un petit moteur avec une tige, qui une fois la partie terminée, pousserait la trappe vers le haut avec la vis et donnera ainsi la possibilité au joueur d’ouvrir la trappe. Cela permet ainsi de « débloquer » la trappe.
•	Mettre une tige qui s’insère horizontalement dans l’épaisseur de la trappe. La trappe est accrochée d’un côté, et dès que la tige s’enlève du trou (le moteur la fait reculer), un ressort situé juste en dessous de la trappe (celle-ci est posée dessus) va pousser la trappe vers le haut avec un effet de recul. Pour refermer la trappe, il suffit de fermer la boite et d’appuyer sur le bouton recommencer, ainsi le moteur va remettre la tige dans le trou. 




Sitographie : 

https://www.instructables.com/id/Mastermind-Game-w-Arduino-Uno/
https://forum.arduino.cc/index.php?topic=79978.0
https://www.instructables.com/id/Arduino-Mastermind-Box-Prototype/


