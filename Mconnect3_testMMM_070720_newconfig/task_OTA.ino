void taskOTA(void*){
  VOID SETUP(){
    
  }
  VOID LOOP(){
   
  switch (state)
  {
  case Runnning_e:
      /*-------------------Start GET CHIPID--------------------------*/
  chipid = ESP.getEfuseMac();
  sprintf(Chipid_buf, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)(chipid));
   
    //Serial.println("NOW: "+DateTimeNOW());
  DELAY(10);
  /*-------------------Start DATETIME--------------------------*/
   data_time();
  
  /*-------------------Start PackData--------------------------*/
  packdata_HEAD();
  packdata_DATE();
  packdata_IO();
  packdata_PWM();
  packdata_RELAY();
  packdata_PWM2();
  packdata_AD();
  packdata_GPS();
  packdata_M1();
  packdata_M2();
  packdata_M3();
 
  DELAY(10);
  /*-------------------------- Send DATA MQTT  --------------------------*/
  sendmqtt();
  
  break;
    case Fota_e:
      DlInfo info;
      info.url = url;
      // info.caCert = NULL;//if only use http then remember to set this to NULL
      info.caCert =  certssl; //SSL Cert iotfmx.com (secure server load from certi.txt)
      info.md5 = md5_1; // info.md5 is argument of setMD5(const char * expected_md5) in Update.h
      info.startDownloadCallback =  startDl;
      info.endDownloadCallback =    endDl;
      info.progressCallback  = progress;
      info.errorCallback     = error;
      int result = httpFOTA.start(info); //OTA Method
      if(result < 0){ // Check error return from class HttpFOTA
        DELAY(100);
        ESP.restart(); 
      }
      if(result == 1){
       String DT =  DateTimeNOW();
               DT += " OTA OK";
        client.publish(ackota,DT.c_str(),Qos);
        DELAY(1000);
        ESP.restart();  
      }
     break;
   }
  }
}
