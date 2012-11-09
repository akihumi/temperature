#include <SPI.h>
#include <Ethernet.h>
#include <Twitter.h>

#define MESSAGE_LIMIT 141
#define BUTTON 8
#difine LED_NUMBER 6

#define TEMP_CENSOR 0
// Ethernet data
byte mac[] = { Your EthernetSheld Mac address };

char msg[MESSAGE_LIMIT];
int temp;
// temperature convert binary number data
int binaryTemp[LED_NUMBER];
int button;

 //  use Tweet Libraly information see URL http://arduino-tweet.appspot.com/
Twitter twitter("Your Access token");

void setup() {
// bigin serial port 9600. this port communication processing.
     Serial.begin(9600) ;
     Ethernet.begin(mac);
     for(int i = 2; i < 8;i++){
       pinMode(i, OUTPUT);      
     }
     pinMode(8, INPUT);
     button = 0;
}
void loop() {
// TEMP_CENSOR read 0~1023
     temp = analogRead(TEMP_CENSOR);
     for(int i = 0; i < 9; i++){
       temp += analogRead(TEMP_CENSOR);
     }
     //  temperature average read
     temp = temp / 10;
     // convert to temperature
     temp  = map(temp, 0, 1023, 0, 5000) ;     
     temp = map(temp, 300, 1600, -30, 100) ;
     // magic!!
     temp = temp - 4;
     Serial.write(temp) ;
     
     // temp convert binary
     int t = temp;
     for(int i = 0; i < LED_NUMBER; i++){
       binaryTemp[i] = t % 2;
       t = t /2;
     }
     // display binary temp
     for(int i = 0; i < LED_NUMBER; i++){
       if(binaryTemp[i] == HIGH){
         digitalWrite(i + 2, HIGH);
       }else{
         digitalWrite(i + 2, LOW);
       }
     }
     // button push post tweet     
     button = digitalRead(BUTTON);
     if(button == HIGH){
       post();
     }
}

void post(){
  snprintf(msg, MESSAGE_LIMIT,  "ただいま私の周囲の温度は %d℃くらいです", temp);
  if(twitter.post(msg)){
    int status = twitter.wait();
    if(status == 200){
    // success tweet when bright LED left side
      digitalWrite(7, HIGH);
      for(int i = 2; i < 7; i++){
        digitalWrite(i, LOW);
      }
    } else{
    // not success tweet when bright LED right side
      digitalWrite(2, HIGH);
      for(int i = 3; i < 8; i++){
        digitalWrite(i, LOW);
      }
    }
  }else{
  // faild post tweet when bright LED middle
    digitalWrite(2, LOW);
    digitalWrite(7, LOW);
    for(int i = 3; i < 7; i++){
      digitalWrite(i, HIGH);
    }
  }
  delay(1000);
}
