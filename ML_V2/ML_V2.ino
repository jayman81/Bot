#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 7    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define BTN 2
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 6

#define FALSE 0
#define TRUE 1



//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(DHTPIN, DHTTYPE);

int data[5];
int i = 0, counts = 0, start = 0, check = 0, buttonState = 0, lastButtonState = 0, btnPosition = 1;
char lcdString[16];
float h, t, CPM;
char str_siev[6];
char timerString[10];


void setup() {
  pinMode(BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN), btnChangeState, CHANGE);

  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
}

void loop() {


 

 

  if (btnPosition == 1) {
    if (getTempHum(data) != 0) {
      Serial.println("AM2303 error.");
    }
    setLCD(btnPosition);
  } else if (btnPosition == 2) {
    getGeiger(data);
  } else if (btnPosition == 3) {
    batterieState();
    setLCD(btnPosition);
  } else {
    Serial.println("im else");
  }
  delay(50);
}

void btnChangeState(){

   buttonState = digitalRead(BTN);
   
   if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      btnPosition ++;
      Serial.print("Btn pos: ");
      Serial.println(btnPosition);
      if (btnPosition > 3) {
        btnPosition = 0;
      }
    }
    delay(50);
  }
  lastButtonState = buttonState;
}

void batterieState() {
  Serial.println("BatterieCheck");
  
}

int getTempHum(int *data) {
  Serial.println("TempCheck");
  h = dht.readHumidity();
  t = dht.readTemperature(FALSE);

  if (isnan(h) || isnan(t)) {
    return -1;
  }

  data[0] = t;
  data[1] = h;

  return 0;
}

void calcSievert() {
  dtostrf((CPM * (0.000001 / 120)), 3, 2, str_siev);
}

void setLCD(int pos)  {

  if (pos == 1) {
    sprintf(lcdString, "Temp: %d C      ", data[0]);
    lcd.setCursor(0, 0);
    lcd.print(lcdString);
    sprintf(lcdString, "LF: %d %%       ", data[1]);
    lcd.setCursor(0, 1);
    lcd.print(lcdString);
  } else if (pos == 2) {
    sprintf(lcdString, "Rad: %d cpm  ", data[2]);
    lcd.setCursor(0, 0);
    lcd.print(lcdString);
    sprintf(lcdString, "S: %s uS/m   ", str_siev);
    lcd.setCursor(0, 1);
    lcd.print(lcdString);
    lcd.setCursor(13, 0);
    lcd.print(timerString);
  } else if (pos == 3) {
    sprintf(lcdString, "Batt: %d V    ", data[4]);
    lcd.setCursor(0, 0);
    lcd.print(lcdString);
    sprintf(lcdString, "               ");
    lcd.setCursor(0, 1);
    lcd.print(lcdString);
  } else if (pos == 0) {
    sprintf(lcdString, "Press BTN now  ");
    lcd.setCursor(0, 0);
    lcd.print(lcdString);
    sprintf(lcdString, "               ");
    lcd.setCursor(0, 1);
    lcd.print(lcdString);
  }

  


}


void getGeiger(int *data) {
  Serial.println("GeigerCheck");
  i = 0;
  counts = 0;
  start = millis();


  while (i < 30) {
    //for (i = 0; i <= 30; i++) {
    if (Serial.available() > 0) {  // If information available
      check = Serial.read();       // Read serial input
      if (check > 0) {
        counts++;                    // If it is a hit, increment counter
      }
    }

    i = millis();
    i = i - start;
    i = i / 1000;
    if (i == 30) {
      i = 0;
      CPM = counts * 2;
      data[2] = CPM;
      calcSievert();
      break;
    }
    sprintf(timerString, "%2d", 30 - i);
    setLCD(btnPosition);
  }

  //setLCD(btnPosition);
}

