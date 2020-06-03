#include<M5Stack.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

const int trigPin = 22;
const int echoPin = 21;
const int trigPin2 = 16;
const int echoPin2 = 17;
long dur;
float dist, dist2;
float oldDist, oldDist2;
long surface = 0 ;
unsigned long timerLoop = millis();
int select[4];
int buttonSelect = 0;
int menu = 0;
bool refresh = false;
bool aeraAutoCount = false;
bool majDistance = false;
bool BEEP = false;
int action; //action {0, 1, 2, 3} -> {None, buttonA, buttonB, buttonC}
unsigned int peoplesNumber = 0;
unsigned int peoplesNumberTmp = 0;
// int for surface 
int digits[] = {0,1,2,3,4,5,6,7,8,9};
  // iD[0] = 10e3, iD[1] = 10e2, iD[2] = 10e1, iD[3] = 10e0
int indexDigits[] = {0,0,0,0};
int modifDigit = 0; 
int selectDigits[] = {1,0,0,0};
#define GRAY  BLUE+0.3*RED

void MAJSurface() {
  surface = digits[indexDigits[0]]*1000 + digits[indexDigits[1]]*100 + digits[indexDigits[2]]*10 + digits[indexDigits[3]];
  }

double distance(int triggerPin, int echoPin){
  
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
        return (distance_mm / 10.0);// On renvoi la valeur en cm
    }
}

void takeDistance(){
  dist = 0;
  dist2 = 0;
  for(int i=0;i<5;i++){
    dist += distance(trigPin, echoPin);
    delay(5);
    dist2 += distance(trigPin2, echoPin2);
    delay(5);
  }
  dist /= 5;
  dist2 /= 5;
}
String btnName1, btnName2, btnName3;
String btnName4 = String("Next");

void drawButton(int x, int y, int w, int h, uint16_t backC, uint16_t textC , String btn1, bool center, bool highlight){
  M5.Lcd.setTextColor(textC);
  if (highlight){
    M5.Lcd.fillRoundRect(x-2, y-2, w+4 , h+4, 12,WHITE);
  }
  M5.Lcd.fillRoundRect(x, y, w , h, 10,backC); 
  if (center){
    M5.Lcd.setCursor(x+int(w/2)-int(btn1.length()*6) , y+8); 
  }
  else {
    M5.Lcd.setCursor(x+5 , y+8); 
  }
  
  M5.Lcd.print(btn1);
}

void drawButtonSection(String btn1, String btn2, String btn3){
  M5.Lcd.setTextSize(2);
  drawButton(35,205,70,30,GRAY,WHITE,btn1,true,false);
  drawButton(128,205,70,30,GRAY,WHITE,btn2,true,false);
  drawButton(222,205,70,30,GRAY,WHITE,btn3,true,false);
}

void resetSelect(){
  for(int i=0;i<4;i++){
    select[i]=0;
  }
}

void nextButton(){
  buttonSelect += 1;
  resetSelect();
  if(buttonSelect >=4){
    buttonSelect = 0;
  }
  select[buttonSelect]=1;
}

void popupSurface(){
  M5.Lcd.setTextSize(2);
  drawButton(30,30,270,140,GRAY,RED,String(""),true,true);
  drawButton(40,85,250,30,GRAY,RED,String("! SURFACE INVALIDE !"),true,false);
  // option button section
  btnName1 = String("");
  btnName2 = String("Ok");
  btnName3 = String("");
  
  drawButtonSection( btnName1, btnName2, btnName3);
  while(not(M5.BtnB.wasReleased())){
    M5.update();  
  }
  return;
}

void menuOption (){
  // action section {none, btnA, btnB, btnC}
  switch(action){
    case 1: // btnA
      nextButton();
      break;
    case 2: // btnB
      switch(buttonSelect){
        case 0: // activate the auto count of peoples
          aeraAutoCount = not(aeraAutoCount);
          break;
        case 1:
          action = 0;
          menu = 2;
          menuSurface();
          return;
          
        case 2: // Go to the counter
          action = 0;
          if (aeraAutoCount and surface < 10){
            // if the surface is invalid
            // we return to option screen
            popupSurface();
            menu = 0;
            buttonSelect = 0;
            nextButton();
            break;
          }
          menu = 3;
          menuCounter();
          return;
        case 3: // Go to the distance menu
          menu = 4;
          action = 0;
          menuDistance();
          return;
        default:
          break;
      }
    default:
      break;
  }
  // Background 
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(100, 10);
  M5.Lcd.print("OPTION:");
  M5.Lcd.setTextSize(2);
  // drawing button section
  drawButton(20,40,220,30,GRAY,WHITE,String("compte auto:"),false,select[0]==1);
    // display auto count bolean
  if (aeraAutoCount){
    drawButton(165,40,70,30,GRAY,GREEN,String("true "),false,false);
  }
  else {
    drawButton(165, 40,70,30,GRAY,RED,String("false"),false,false);
  }
  drawButton(20,80,220,30,GRAY,WHITE,String("Surface limite..."),false,select[1]==1);
  drawButton(20,120,220,30,GRAY,WHITE,String("Compteur..."),false,select[2]==1); 
  drawButton(20,160,220,30,GRAY,WHITE,String("Distance..."),false,select[3]==1); 

  // option button section
  btnName1 = String("Suiv.");
  btnName2 = String("Ok");
  btnName3 = String("");
  
  drawButtonSection( btnName1, btnName2, btnName3);

}

void digitSelection(){
  modifDigit += 1;
  for (int i=0;i<4;i++){
    selectDigits[i] = 0;
  }
  if (modifDigit>3){
    modifDigit=0;
  }
  selectDigits[modifDigit] = 1;
}

void menuSurface(){
  majDistance = true;
  // action section {none, btnA, btnB, btnC}
  switch(action){
    case 0:
      break;
    case 1: // btnA
      digitSelection();
      break;
    case 2: // btnB
      indexDigits[modifDigit] +=1;
      if(indexDigits[modifDigit]>9){
        indexDigits[modifDigit]=0;
      }
      break;
    case 3: // Go to the option menu btnC
      menu = 0;
      action = 0;
      menuOption();
      return;
    default:
      break;
  }
  // Background 
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(90, 10);
  M5.Lcd.print("SURFACE:");
  // Writing poeples number
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.println("surface du magasin");
  MAJSurface();
  M5.Lcd.setTextSize(5);
  for (int i=0;i<4;i++){    
    drawButton(30+int(i*40),75,35,50,BLACK,WHITE,String(digits[indexDigits[i]]),false,selectDigits[i]==1);
  }
  
  M5.Lcd.setCursor(200, 80);
  M5.Lcd.print("m");
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(230, 75);
  M5.Lcd.print("2");
  
  
  /*
  delay(1000);
  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(10,90);
  compteurDetection();
  M5.Lcd.print(peoplesNumber);*/
  // option button section
  btnName1 = String("Suiv.");
  btnName2 = String("+1");
  btnName3 = String("Ret.");
  
  drawButtonSection( btnName1, btnName2, btnName3);
}

void menuCounter(){
  majDistance = true;
  // action section {none, btnA, btnB, btnC}
  switch(action){
    case 0:
      break;
    case 1: // btnA
      peoplesNumber = 0;
      break;
    case 2: // btnB
      peoplesNumber += 1;
      break;
    case 3: // Go to the option menu btnC
      menu = 0;
      action = 0;
      menuOption();
      return;
    default:
      break;
  }
  // Background 
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(90, 10);
  M5.Lcd.print("COMPTEUR:");
  // Writing poeples number
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 50);
  M5.Lcd.println("Personnes dans le magasin");/*
  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(10,90);
  compteurDetection();
  M5.Lcd.print(peoplesNumber);*/
  
  if (aeraAutoCount and surface > 10){
    displayCountValue();
    M5.Lcd.setCursor(180,100);
    M5.Lcd.print("/");
    M5.Lcd.print(int(surface / 10));
    M5.Lcd.setTextColor(WHITE);
  }
  else {
    M5.Lcd.setTextColor(WHITE);
    displayCountValue();
  }
  // option button section
  btnName1 = String("Reset");
  btnName2 = String("");
  btnName3 = String("Ret.");
  
  drawButtonSection( btnName1, btnName2, btnName3);
}

void displayCountValue(){
  compteurDetection();
  M5.Lcd.setTextSize(5);
  M5.Lcd.fillRect(10, 90, 150, 50, BLACK);
  M5.Lcd.setCursor(10,90);
  if (peoplesNumber > 999){
    peoplesNumber=0;
  }
  if(peoplesNumber >= int(surface / 10)-1) {
    M5.Lcd.setTextColor(RED);
  }
  M5.Lcd.print(peoplesNumber);
    M5.Lcd.setTextColor(WHITE);
}

void displayDistValue(){
  M5.Lcd.setTextSize(5);
  M5.Lcd.fillRect(10, 75, 300, 50, BLACK);
  M5.Lcd.setCursor(10, 75);
  if(dist < 0){
    M5.Lcd.print("Erreur"); 
  }
  else{
    M5.Lcd.print(dist);
    M5.Lcd.print(" cm");
  }
  M5.Lcd.fillRect(10, 150, 300, 50, BLACK);
  M5.Lcd.setCursor(10,150);
  if(dist2 < 0){
    M5.Lcd.print("Erreur"); 
  }
  else{
    M5.Lcd.print(dist2);
    M5.Lcd.print(" cm");
  }
}

void menuDistance(){
  majDistance = true;
  // action section {none, btnA, btnB, btnC}
  switch(action){
    case 0:
      break;
    case 1: // btnA
      break;
    case 2: // btnB
      break;
    case 3: // Go to the option menu btnC
      menu = 0;
      action = 0;
      menuOption();
      return;
    default:
      break;
  }
  // Background 
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(90, 10);
  M5.Lcd.print("DISTANCE:");
  // Writing poeples numbe
    // text
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 50);
  M5.Lcd.print("Capteur 1 :");
  M5.Lcd.setCursor(5, 125);
  M5.Lcd.print("Capteur 2 :");
    // value
  displayDistValue();
  // option button section
  btnName1 = String("");
  btnName2 = String("");
  btnName3 = String("Ret.");
  
  drawButtonSection( btnName1, btnName2, btnName3);
}

void distanceMajTimerLoop(){
  if (majDistance){ // if the maj distance is asked
    if (micros() - timerLoop > 500){ //and thetimer is out the time loop
      switch(menu){
        case 3: // counter menu
          displayCountValue();// refresh poeple count value on the screen
          break;
        case 4: // Distance menu
          displayDistValue(); // refresh value on the screen
          break;
        default:
        break;
      }
      timerLoop = micros(); // reset the timer
    }
  }
 
}

void compteurDetection(){
  if(oldDist-dist > 20){ // input detection
    while ( oldDist2-dist2 < 21 and oldDist-dist > 20){
      M5.Lcd.setTextSize(2);
      M5.Lcd.fillRect(90, 150, 100, 50, BLACK);
      takeDistance();
      if (oldDist2-dist2 > 20 ){
        peoplesNumber += 1;
        return;
      }
      delay(2);
    }
  }
  else if(oldDist2-dist2 > 20){ // output detection
      M5.Lcd.setTextSize(2);
    while ( oldDist-dist < 21 and oldDist2-dist2 > 20){
      M5.Lcd.fillRect(90, 150, 100, 50, BLACK);
      takeDistance();
      if (oldDist-dist > 20 ){
        peoplesNumber -= 1;
        return;
      }
      delay(2);
    }
  }
  return;
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);// put your setup code here, to run once:
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT);// put your setup code here, to run once:
  // init to the first selection button
  select[0] = 1;
  menu = 0;
  menuOption();
  //menuCounter();
  //menuDistance();
}

void loop() {
  M5.update();
  oldDist = dist;
  oldDist2 = dist2;

  takeDistance();
  if (refresh){
    switch(menu){
      case 0: // Option menu
        majDistance = false;
        menuOption();
        
        break;
      case 1: // Surface limite menu
        majDistance = false;
        break;
      case 2: // surface modif menu
        majDistance = false;
        menuSurface();
        break;
      case 3: // counter menu
        menuCounter();
        if (peoplesNumber != peoplesNumberTmp){
          SerialBT.println(peoplesNumber);
          delay(500);
          peoplesNumberTmp = peoplesNumber;
        }
        break;
      case 4: // Distance menu
        menuDistance();
        break;
      default:
        menu = 0;
        break;
    }
    refresh = false;
  }

  // action button section
  action = 0;
  if (M5.BtnA.wasReleased()) {
    action = 1;
    refresh = true;
  }
  if (M5.BtnB.wasReleased()) {
    action = 2;
    refresh = true;
  }
  if (M5.BtnC.wasReleased()) {
    action = 3;
    refresh = true;
     
  }
  distanceMajTimerLoop();
}
