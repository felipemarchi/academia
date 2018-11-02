#include <SPI.h>
#include <MFRC522.h>

int javaCommand;
const int R = 5;
const int G = 6;
const int B = 7;

constexpr uint8_t RST_PIN = 9; // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10; // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

byte nuidPICC[4]; // Init array that will store new NUID

void setup() {    
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  setColor(0, 0, 255);  // blue
  
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  //if(Serial.available() > 0) { // Verifica se existe comunicação com a porta serial
    
    // Look for new cards
    if ( ! rfid.PICC_IsNewCardPresent())
      return;
    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
      return;
      
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    
    // Print NUID into SerialPort
    byte *buffer = rfid.uid.uidByte;
    byte bufferSize = rfid.uid.size;
    for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], DEC);
    }
    Serial.print("\n"); 

    delay(750);

    // Read from SerialPort and display result in Led
    javaCommand = Serial.read();
    if (javaCommand == 0) {
      setColor(255, 0, 0);  // red
      delay(1000);
      setColor(0, 0, 255);  // blue
    } else if (javaCommand == 1) { 
      setColor(0, 255, 0);  // green 
      delay(1000);
      setColor(0, 0, 255);  // blue
    }
    
    // Halt PICC
    rfid.PICC_HaltA();
  
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();   
    
  //}
}


void setColor(int red, int green, int blue)
{
  // conversao para anodo
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  analogWrite(R, red);
  analogWrite(G, green);
  analogWrite(B, blue);  
}
