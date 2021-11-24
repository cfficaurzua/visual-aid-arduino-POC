#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  /* Slave Select Pin */
#define RST_PIN 7  /* Reset Pin */



/* Create an instance of MFRC522 */
MFRC522 mfrc522(SS_PIN, RST_PIN);
/* Create an instance of MIFARE_Key */
MFRC522::MIFARE_Key key;      
byte productName[16];
byte productPrice[16];
char buffer[16];
int charsRead;

void copy(byte* src, byte* dst, byte len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   SPI.begin();
   mfrc522.PCD_Init();
   Serial.println("Scan a MIFARE 1K Tag to write data...");
   for (byte i = 0; i < 6; i++)
    {
      key.keyByte[i] = 0xFF;
    }
}

void loop() { 

  Serial.print("\n");
  Serial.println("Product name: (max 15 chars)");
   while (true){
     if (Serial.available() > 0) {                                             
        charsRead = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);  
        buffer[charsRead] = '\0';                                              
        Serial.println(buffer);
        break;
     } 
   }
  copy(buffer,productName, 16);
  Serial.print("\n");
  Serial.println("Product price: (max 15 chars)");
  delay(500);
  while (Serial.read() >= 0){}
  while (true){
     if (Serial.available() > 0) {                                             
      charsRead = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
      buffer[charsRead] = '\0';                                            
      Serial.println(buffer);
      break;
     }
   }
   
   copy(buffer,productPrice, 16);
    Serial.print("\n");
    Serial.println("Place the Rfid card to save the data");
  delay(500);
  while (Serial.read() >= 0){}
  while (true){
    if (  mfrc522.PICC_IsNewCardPresent()){
    if (  mfrc522.PICC_ReadCardSerial()) {
    
    Serial.print("\n");
    Serial.println("Writing data");

    WriteDataToBlock(2, productName);
    WriteDataToBlock(4, productPrice);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    break;
    }
   }
   
  }
    
}

void WriteDataToBlock(int blockNum, byte blockData[]) 
{
  /* Authenticating the desired data block for write access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Data was written into Block successfully");
  }
  
}
