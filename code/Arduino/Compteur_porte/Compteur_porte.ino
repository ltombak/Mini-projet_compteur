#include<M5Stack.h>
#include <NewPing.h>

const int trigPin = 22;
const int echoPin = 21;
const int trigPin2 = 16;
const int echoPin2 = 17;
long dur;
float dis,dis2;
float c;
float s;

NewPing sonar1(trigPin, echoPin, 400);
NewPing sonar2(trigPin2, echoPin2, 400);

double distance(int triggerPin, int echoPin){
  
  float SOUND_SPEED = 340.0 / 1000; // vitesse du son en mm/uS
  unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s
  
  digitalWrite(triggerPin, LOW);// On vérifie que le capteur est éteint au repos
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);// On allume pour 10uS
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);// On éteint

  long measure = pulseIn(echoPin, HIGH);// Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe)
  float distance_mm = (measure / 2.0) * SOUND_SPEED;// Calcul la distance à partir du temps mesuré

  if (distance_mm < 1 || distance_mm > 3000) {// Condition pour vérifier qu'on est dans les limites voulues
        return -1.0 ;// Si on y est pas on renvoie un code d'erreur: -1
    } else {
        return (distance_mm / 10.0);// On renvoi la valeur en cm
    }
}

void setup() {
  M5.begin();
  /*pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);// put your setup code here, to run once:
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT);// put your setup code here, to run once:
  */
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.print("AREA CAL: ");
  M5.Lcd.setCursor(3, 35);
  M5.Lcd.println("A FOR SQ:");
  M5.Lcd.println("B FOR CR:");
  M5.Lcd.println("C FOR DIS:");
}

void loop() {
  M5.update();
  /*digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  dur = pulseIn(echoPin, HIGH);
  dis= dur*0.034/2;*/
  dis = sonar1.ping_cm();
  delayMicroseconds(50);
  dis2 = sonar2.ping_cm();
  delayMicroseconds(50);
 
  /*if (M5.BtnA.wasReleased()) {
     s = dis*dis;
     M5.Lcd.clear(BLUE);
     M5.Lcd.setTextColor(WHITE);
     M5.Lcd.setTextSize(3);
     M5.Lcd.setCursor(65, 10);
     M5.Lcd.print("AREA CAL: ");
     M5.Lcd.setCursor(3, 35);
     M5.Lcd.println("Area(cm):");
     M5.Lcd.setTextSize(9);
     M5.Lcd.setCursor(140, 70);
     M5.Lcd.print(s);// leave
  }*/
  if (M5.BtnA.wasReleased()) {
     M5.Lcd.clear(BLUE);
     M5.Lcd.setTextColor(WHITE);
     M5.Lcd.setTextSize(3);
     M5.Lcd.setCursor(3, 10);
     M5.Lcd.print("DISTANCE CAL: ");
     M5.Lcd.setCursor(3, 35);
     M5.Lcd.println("DIST1(cm):");
     M5.Lcd.setTextSize(9);
     M5.Lcd.setCursor(3, 60);
     M5.Lcd.print(dis);
     M5.Lcd.setTextSize(3);
     M5.Lcd.setCursor(3, 120);
     M5.Lcd.println("DIST2(cm):");
     M5.Lcd.setTextSize(9);
     M5.Lcd.setCursor(3, 155);
     M5.Lcd.print(dis2);
  }
  if (M5.BtnB.wasReleased()) {
     M5.Lcd.clear(BLUE);
     M5.Lcd.setTextColor(WHITE);
     M5.Lcd.setTextSize(3);
     M5.Lcd.setCursor(3, 10);
     M5.Lcd.print("DISTANCE CAL: ");
     M5.Lcd.setCursor(3, 35);
     M5.Lcd.println("DIST2(cm):");
     M5.Lcd.setTextSize(9);
     M5.Lcd.setCursor(3, 70);
     M5.Lcd.print(dis2);
  }
  if (M5.BtnC.wasReleased()) {
     M5.Lcd.clear(BLUE);
     M5.Lcd.setTextColor(WHITE);
     M5.Lcd.setTextSize(3);
     M5.Lcd.setCursor(3, 10);
     M5.Lcd.print("DISTANCE CAL: ");
     M5.Lcd.setCursor(3, 35);
     M5.Lcd.println("DIST(cm):");
     M5.Lcd.setTextSize(9);
     M5.Lcd.setCursor(3, 70);
     M5.Lcd.print(dis);
  }
}
