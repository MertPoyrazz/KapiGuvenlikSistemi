#define SS_PIN D4 //SDA Pini
#define RST_PIN D3

int alarmPin = D8;
int yesilLed = D1;
int kirmiziLed = D2;
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   
int statuss = 0;
int out = 0;

char auth[] = "slek-mVqXKPk3G2dKSgrmTNXCQbEg1uk"; //mobil uygulamasına bağlanmak için gerekli anahtar

char ssid[] = "TurkTelekom_TFB64";

char pass[] = "emreyavuz";

BlynkTimer timer;

WidgetRTC rtc; // mobil uygulamada ki widget tanımlandı.

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}
void setup() 
{
  pinMode(alarmPin, OUTPUT);
  pinMode(yesilLed, OUTPUT);
  pinMode(kirmiziLed, OUTPUT);
  
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init(); 
  Blynk.begin(auth, ssid, pass); //mobil uygulama başlatıldı.
  
}

void loop() 
{
  Blynk.run();
 
  // Yeni kartlari tara
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Kart okundu ise
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Kartin UID'sini oku ve seri porta yaz
  Serial.println();
  Serial.print(" UID etiketi :");
  
  String content= "";
  byte letter;
  // okutulan kartın uid si kontrol ediliyor
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  // kayıtlı uid'ler ile okutulan kartın uid'si karşılaştırılıyor
  if (content.substring(1) == "77 D2 02 2C") 
  {
    String currentTime = String(hour()) + ":" + minute() + ":" + second();
    Serial.print("Current time: ");
    
    Serial.println(" Giris Basarili ! ");
    String a = "Giriş yapıldı.";
    Blynk.virtualWrite(V4 , a + currentTime); 
   
     digitalWrite(alarmPin, HIGH);
     digitalWrite(yesilLed, HIGH);
      delay(500);
    digitalWrite(alarmPin, LOW);
    digitalWrite(yesilLed, LOW);
    delay(1000);
    
    
    statuss = 1;
   
  }
  
  else   {
    String currentTime = String(hour()) + ":" + minute() + ":" + second();
    Serial.println(" Kart Tanimlanmadi ! ");
    Serial.println(" Giris Basarisiz. ");
    String b = "İzinsiz Giriş Denemesi.";
      
    Blynk.virtualWrite(V4 , b + currentTime);
    digitalWrite(alarmPin, HIGH);
    digitalWrite(kirmiziLed, HIGH);
      delay(100);
    digitalWrite(alarmPin, LOW);
    digitalWrite(kirmiziLed, LOW);
      delay(100);
    digitalWrite(alarmPin, HIGH);
    digitalWrite(kirmiziLed, HIGH);
      delay(100);
    digitalWrite(alarmPin, LOW);
    digitalWrite(kirmiziLed, LOW);
      delay(100);
    digitalWrite(alarmPin, HIGH);
    digitalWrite(kirmiziLed, HIGH);
      delay(100);
    digitalWrite(alarmPin, LOW);
    digitalWrite(kirmiziLed, LOW);
      delay(100);
    
    delay(3000);
  }
  } 

 BLYNK_WRITE(V2)
 {
  digitalWrite(kirmiziLed, HIGH);
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String c =  "Sistem Kapatıldı.";
  Blynk.virtualWrite(V4 , c + currentTime);

  }
  BLYNK_WRITE(V3)
 {
  digitalWrite(kirmiziLed, LOW);
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String d =  "Sistem Açıldı.";
  Blynk.virtualWrite(V4 , d + currentTime);

  }
 
