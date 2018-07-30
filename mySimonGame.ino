/*
 * Projet SIMON
 */

// Initialisation des LEDs et boutons poussoirs
const int redLEDPin       = 2; // LED rouge
const int greenLEDPin     = 3; // LED verte
const int blueLEDPin      = 4; // LED bleue
const int yellowLEDPin    = 5; // LED jaune

const int redButtonPin    = 6; // Bouton rouge
const int greenButtonPin  = 7; // Bouton vert
const int blueButtonPin   = 8; // Bouton bleu
const int yellowButtonPin = 9; // Bouton jaune

const int speakerPin      = 10; // Speaker

// Les numéros correspondent aux numéros d'entrées/sorties
// Les 0 et 1 sont réservés aux transmissions/réceptions

// Fréquences des sons de chaque couleur
const int son[] = {261, 330, 392, 523};
//                 Do,  Mi,  Sol, Do

// Tableau de séquence à jouer
int sequence[100];
// Personne ne dépasse jamais le niveau 100...

/*
 * Routine d'initialisation
 */
void setup(){
  // Initialisation des entrées/sorties
  pinMode(redLEDPin,       OUTPUT);
  pinMode(greenLEDPin,     OUTPUT);
  pinMode(blueLEDPin,      OUTPUT);
  pinMode(yellowLEDPin,    OUTPUT);
  pinMode(redButtonPin,    INPUT);
  pinMode(greenButtonPin,  INPUT);
  pinMode(blueButtonPin,   INPUT);
  pinMode(yellowButtonPin, INPUT);
  pinMode(speakerPin,      OUTPUT);
  
  Serial.begin(9600);
}

/*
 * Boucle principale
 */
void loop(){
  randomSeed(analogRead(A0));

  // Attente de pression sur une touche
  lectureBouton();

  // Témoin de lancement du jeu
  melodieLancement();
  delay(2000);

  // Début du jeu
  boolean gameOver = false;
  int presse;
  int niveau = 0;
  
  while (!gameOver){
    nouvelleNote(niveau);  // Rajoute une note à la séquence
    jouerSequence(niveau); // Joue la séquence

    for (int i=0; i<=niveau; i++){ // Attend que le joueur tape chaque note de la séquence
      presse = lectureBouton();    // Lecture du bouton appuyé par le joueur
      jouerNote(presse);           // Joue la note du joueur
      if (presse!=sequence[i]){    // Si la note est bonne, on continue, sinon GameOver
        gameOver = true;
        melodieGameOver();
      }
      if (gameOver){
        break;
      }
    }
    niveau++;
    delay(1000);
  }
}

/*
 * Fonction lectureBouton
 *
 * Vérification des boutons un par un
 * et retourne celui qui est appuyé
 */
int lectureBouton(){
  int boutonPresse = 0;

  while (!boutonPresse){
    if (digitalRead(redButtonPin)){
      boutonPresse = 1;
    }
    if (digitalRead(greenButtonPin)){
      boutonPresse = 2;
    }
    if (digitalRead(blueButtonPin)){
      boutonPresse = 3;
    }
    if (digitalRead(yellowButtonPin)){
      boutonPresse = 4;
    }
  }
  return boutonPresse;
}

/*
 * Fonction jouerNote
 *
 * Joue la note et 
 * allume la LED associée
 */
void jouerNote(int numero){
  switch (numero){
    case 1:
      digitalWrite(redLEDPin,    HIGH);
      break;
    case 2:
      digitalWrite(greenLEDPin,  HIGH);
      break;
    case 3:
      digitalWrite(blueLEDPin,   HIGH);
      break;
    case 4:
      digitalWrite(yellowLEDPin, HIGH);
      break;
  }
  tone(speakerPin, son[numero-1]); // Le tableau commence à 0 (non à 1)
  delay(800);

  switchOnOffAllLED(LOW);
  noTone(speakerPin);

  delay(200);
}

/*
 * Fonction jouerSequence
 *
 * Joue la séquence musicale
 */
void jouerSequence(int niveau){
  for (int i=0; i<=niveau; i++){
    jouerNote(sequence[i]);
  }
}

/*
 * Fonction nouvelleNote
 *
 * Rajoute une nouvelle note
 * dans la séquence
 */
void nouvelleNote(int niveau){
  sequence[niveau] = (int)random(1,5);
  
  Serial.print(sequence[niveau]);
}

/*
 * Fonction melodieLancement
 *
 * Joue la mélodie de lancement
 * et fait clignoter les LEDs
 */
void melodieLancement(){
  switchOnOffAllLED(HIGH);
  for (int i=0; i<4; i++){
    tone(speakerPin, son[i]);
    delay(300);
  }
  switchOnOffAllLED(LOW);
  noTone(speakerPin);
}

/*
 * Fonction melodieGameOver
 * 
 * Joue la mélodie Game Over
 * et fait clignoter les LEDs
 */
void melodieGameOver(){
  for (int i=3; i>=0; i--){
    switchOnOffAllLED(HIGH);
    tone(speakerPin, son[i]);
    delay(150);
    switchOnOffAllLED(LOW);
    delay(150);
  }
  noTone(speakerPin);
  
  Serial.println("");
} 

/*
 * Fonction switchOnOffAllLED
 *
 * Allume ou éteint toutes les LEDs
 */
void switchOnOffAllLED(int state){
  for (int i=redLEDPin; i<=yellowLEDPin; i++){
   digitalWrite(i, state); 
  }  
}
