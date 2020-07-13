void taskLEDStatus(void*){
 
  VOID SETUP(){      
      pinMode(xLED2,OUTPUT);
      
   }
  VOID LOOP(){      
     digitalWrite(xLED2,LOW);
     DELAY(xTimeBlink);
     digitalWrite(xLED2,HIGH);
     DELAY(xTimeBlink);
   }
}
