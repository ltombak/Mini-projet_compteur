int triggerPin = 2;
int echoPin = 3;

bool comparator(float d1, float d2)
{
  float tolerance = 0.05;// Tolérance en m
  if (d1 < (d2 - tolerance))
  {
    return true;
  }
  else
  {
    return false;
  }
}


double distance(int triggerPin, int echoPin)
{
  float SOUND_SPEED = 340.0 / 1000; // vitesse du son en mm/uS
  unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s
  
  digitalWrite(triggerPin, LOW);// On vérifie que le capteur est éteint au repos
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);// On allume pour 10uS
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);// On éteint

  long measure = pulseIn(echoPin, HIGH, MEASURE_TIMEOUT);// Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe)
  float distance_mm = measure / 2.0 * SOUND_SPEED;// Calcul la distance à partir du temps mesuré

  if (distance_mm < 1 || distance_mm > 3000) {// Condition pour vérifier qu'on est dans les limites voulues
        return -1.0 ;// Si on y est pas on renvoie un code d'erreur: -1
    } else {
        return (distance_mm / 1000.0);// On renvoi la valeur en m
    }
}


void setup() {
  Serial.begin(115200);// Initialisation du serial

  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(echoPin, INPUT);
}


float d1 = 0;
float d2 = 0;

void loop() {
  d1 = distance(triggerPin,echoPin);// On effectue la mesure
  
  Serial.print("(1) d1,d2: ");
  Serial.print(d1);
  Serial.print(" ");
  Serial.println(d2);
  
  // Affiche les résultats en m
  //Serial.print(F("Distance: "));
  //Serial.print(d1);
  //Serial.println(F("m"));
  
  d2 = d1;
  if (comparator(d1,d2))
    Serial.println("OKAY");
  else
    Serial.println("BOOOOO");

  
  // délai d'attente entre 2 mesures
  delay(1000);
}
