#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  /* Slave Select Pin */
#define RST_PIN 7  /* Reset Pin */



/* Create an instance of MFRC522 */
MFRC522 mfrc522(SS_PIN, RST_PIN);
/* Create an instance of MIFARE_Key */
MFRC522::MIFARE_Key key;          

#include <SoftwareSerial.h>
SoftwareSerial bSerial(4,5); // RX, TX
byte buffer[18];
void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   bSerial.begin(9600);
   SPI.begin();
   mfrc522.PCD_Init();
   for (byte i = 0; i < 6; i++)
    {
      key.keyByte[i] = 0xFF;
    }
    Serial.println("Waiting for card");
}
void copy(byte* src, byte* dst, byte len) {
    memcpy(dst, src, sizeof(src[0])*len);
}
void loop() {
    Serial.print(".");
    if (  mfrc522.PICC_IsNewCardPresent()){
    if (  mfrc522.PICC_ReadCardSerial()) {
    
    Serial.print("\n");
    Serial.println("Reading from Data Block...");


    byte productName[18];
    ReadDataFromBlock(2, buffer);
    copy(buffer,productName, 18);
    for (char i=0;i<18;i++){buffer[i]=0;}
    byte productPrice[18];
    ReadDataFromBlock(4, buffer);
    copy(buffer, productPrice, 18);
    for (int j=0 ; j<16 ; j++)
   { 
     Serial.write(productName[j]);
     bSerial.write(productName[j]);
     if (productName[j+1]==0){
      break;
     }
   }
   bSerial.write(", ");
   Serial.print(", ");
   for (int j=0 ; j<16 ; j++)
   { 
     Serial.write(productPrice[j]);
     bSerial.write(productPrice[j]);
     if (productPrice[j+1]==0){
      break;
     }
   }
   bSerial.write(" pesos");
   bSerial.write(0x7E);
   bSerial.write("\n");
   Serial.write(" pesos");
   Serial.write(0x7E);
   Serial.write("\n");
   mfrc522.PICC_HaltA();
   mfrc522.PCD_StopCrypto1();
   Serial.println("Waiting for card");
    }
    } 
  delay(100);
 }



void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  /* Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, sizeof(readBlockData));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Block was read successfully");  
  }
  
}
