long pote;

long pote_anterior;

float dec;

float dec2;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd_0x27(0x27, 16, 2);

//sensor de ultrasonido
long u_tiempo(int trigger_pin, int echo_pin) {
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);
    long microseconds = pulseIn(echo_pin ,HIGH);
    return microseconds;
}

long u_distancia(int trigger_pin, int echo_pin) {
    long microseconds = u_tiempo(trigger_pin, echo_pin);
    long u_distancia;
    u_distancia = microseconds/29/2;
    if (u_distancia == 0){
        u_distancia = 999;
    }
    return u_distancia;
}

void setup()
{
  pote=0;

  pote_anterior=0;

  dec=-1;

  dec2=100;


  lcd_0x27.init();
  lcd_0x27.backlight();


    lcd_0x27.setCursor(0, 0);
    lcd_0x27.print("Ajuste la dosis:");

    lcd_0x27.setCursor(0, 1);
    lcd_0x27.print("0");

    lcd_0x27.setCursor(4, 1);
    lcd_0x27.print("ml");

    lcd_0x27.setCursor(7, 1);
    lcd_0x27.print("Op:");

    lcd_0x27.setCursor(14, 1);
    lcd_0x27.print("ml");

  pinMode(A0,INPUT);

  pinMode(2,INPUT);

  pinMode(5,INPUT);

  Serial.begin(9600);
  Serial.begin(9600);

  pinMode( 7 , INPUT );

  pinMode( 6 , OUTPUT );

  pinMode(3,OUTPUT);

  pinMode(4,OUTPUT);

}


void loop()
{


        pote = map(analogRead(A0),0,1023,1,5);
    pote = pote * 250;
    if (pote != pote_anterior) {
      pote_anterior = pote;

        lcd_0x27.setCursor(0, 1);
        lcd_0x27.print(pote);
      if (pote < 1000) {

          lcd_0x27.setCursor(3, 1);
          lcd_0x27.print(" ");

      }

    }
    if (digitalRead(2) == 1) {

        lcd_0x27.setCursor(10, 1);
        lcd_0x27.print(pote);
      if (pote < 1000) {

          lcd_0x27.setCursor(13, 1);
          lcd_0x27.print(" ");

      }
      if (digitalRead(5) == 0) {
        Serial.println("Carga con Ultrasonico");
        Serial.print("Distancia a alcanzar: ");
        Serial.println(((dec / dec2) * pote + 15));
        while (u_distancia(6,7) >= (dec / dec2) * pote + 15) {
          digitalWrite(3,LOW);
          digitalWrite(4,HIGH);
         }
        Serial.println("Carga Finalizada");
        digitalWrite(4,LOW);
        digitalWrite(3,HIGH);

      } else {
        Serial.println("Carga por tiempo");
        Serial.print("Tiempo de bomba:");
        Serial.println((String(pote * 30) + String("ms")));
        digitalWrite(3,LOW);
        digitalWrite(4,HIGH);
        Serial.println((String("Cargando por:") + String(pote * 30) + String("ms")));
        delay(pote * 30);
        digitalWrite(3,HIGH);
        digitalWrite(4,LOW);
        Serial.println("Carga Finalizada");

      }
      delay(500);

    }


}
