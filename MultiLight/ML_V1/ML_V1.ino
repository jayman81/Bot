#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 7    // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define GEIGER 6
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

#define FALSE 0
#define TRUE 1



//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(DHTPIN, DHTTYPE);

int data[3];
int i = 0, counts = 0, start = 0, cnt = 0;
char lcdString[16];



void setup() {
  pinMode(GEIGER, INPUT);
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
}

void loop() {
  setLCD();

  if (getTempHum(data) != 0) {
    Serial.println("AM2303 error.");
  }

  getGeiger(data);



}

int getTempHum(int *data) {
  float h = dht.readHumidity();
  float t = dht.readTemperature(FALSE);

  if (isnan(h) || isnan(t)) {
    return -1;
  }

  data[0] = t;
  data[1] = h;

  return 0;
}


void setLCD()  {
  sprintf(lcdString, "T: %dC, H: %d%", data[0], data[1]);
  lcd.setCursor(0, 0);
  lcd.print(lcdString);
  sprintf(lcdString, "Sievert: %d uS/m", data[2]);
  lcd.setCursor(0, 1);
  lcd.print(lcdString);
}


void getGeiger(int *data) {
  i = 0;
  counts = 0;
  start = millis();
  cnt = 0;
  char data2[20];

  for (i = 0; i <= 30; i++) {
    sprintf(data2, "i: %d", i);
    Serial.println(data2);
    counts = analogRead(GEIGER);
    if (counts > 350) {
      cnt++;
      // piezo enable
    }
    sprintf(data2, "----------->i: %d", cnt);
    Serial.println(data2);
    i = millis();
    i = i - start;
    i = i / 1000;
    if (i == 30) {
      i = 0;
      break;
    }
  }
  data[3] = cnt * 2;

}

