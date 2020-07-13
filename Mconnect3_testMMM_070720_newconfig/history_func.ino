const char* listDirHist(fs::FS &fs, const char * dirname, uint8_t levels){
  const char* Fhis;
  String Lfile = "";
  int countfiles = 0;
  Serial.printf("Listing directory: %s\n", dirname);
  
   File root = fs.open(dirname);
   if(!root){
     Serial.println("Failed to open directory");
      return "F";
   }
   if(!root.isDirectory()){
    Serial.println("Not a directory");
    return "N";
   }
  File file = root.openNextFile();
        if(!file){
          return "E";//Empty file in Dir
        }
  while(file){
    if(file.isDirectory()){
      Serial.print(" DIR : ");
        Serial.println(file.name());
        
       if(levels){
         listDirHist(fs, file.name(), levels -1);
        }
     } else {
        //Serial.print(" FILE: ");
          //Serial.print(file.name());
           String L = file.name();
            unsigned int  S = file.size();
           Lfile  += L.substring(9)+":"+String(S)+" "; //substring from position 9 (/history/)
          
            
           /*
           Serial.print(" SIZE: ");
           Serial.println(file.size());
           */
       countfiles++;    
     }
    file = root.openNextFile();
   }
   Lfile  += String(countfiles);
   Fhis = Lfile.c_str();
  
   return Fhis;
  
  
}

String readHistFile(fs::FS &fs, const char * path){
 
   Serial.printf("Reading file: %s\n", path);
 
    File file = fs.open(path);
     if(!file){
        Serial.println("Failed to open file for reading");
        return "F";
     }
    
    String Datas = "";
    while(file.available()){
      Datas += file.readStringUntil('\n');
      //Serial.write(file.read());
    } file.close();
    //Serial.println(Datas);
    return Datas;
 }

const char* deleteHistFile(fs::FS &fs, const char * path){
   Serial.printf("Deleting file: %s\n", path);
     const char* result;
     if(fs.remove(path)){
        Serial.println("File deleted");
        result = "1";
        return result;
     } else {
        Serial.println("Delete failed");
        result = "0";
        return result;
     }
}

const char* delAll(fs::FS &fs, const char * dir){
  const char* res;
   File root = fs.open(dir);
   if(!root){
     Serial.println("Failed to open directory");
   }
   if(!root.isDirectory()){
    Serial.println("Not a directory");
   }
    File file = root.openNextFile();
      while(file){
        Serial.println(String("Delete: ") + file.name());
         if(fs.remove(file.name())){
          file = root.openNextFile();
         }
      }
     res = "1";
   return res;
}

/*---------------------------------------------HISTORY MANAGEMENT---------------------------------------------------*/
/*
void HistoryCallback(char *topic, byte *payload, unsigned int length){
  Serial.println(topic);//Print topic received
  Serial.println((char*)payload);//print payload (or message) in topic received
  
   
   if (strncmp(history, topic, strlen(history)) == 0){
      if((char)payload[0] == 'g' && (char)payload[1] == 'e' && (char)payload[2] == 't' && (char)payload[3] == 'h' && (char)payload[4] == 'i' && (char)payload[5] == 's'){
         const char* histfiles; 
          histfiles = listDirHist(SD,"/history",0);
          Serial.println(histfiles);
          client.publish(history,histfiles);
      }
   }

   if (strncmp(readhis, topic, strlen(readhis)) == 0){
      if((char)payload[0] == 'R'){
      String fda,Fpub;
     
      for(int p=0;p<length;p++){
        if(p>0){
          fda += (char)payload[p];
        }
      }
       Fpub = readHistFile(SD,fda.c_str());
       client.publish(readhis,Fpub.c_str());
      }
    }
   if (strncmp(delFile, topic, strlen(delFile)) == 0){
       if((char)payload[0] == 'D'){
          String dFa;
          const char* Dpub;
          for(int p=0;p<length;p++){
            if(p>0){
              dFa += (char)payload[p];
            }
          }
          Dpub = deleteHistFile(SD,dFa.c_str());
          client.publish(delFile,Dpub);
       }
     }
   if (strncmp(delAFile, topic, strlen(delAFile)) == 0){
    if((char)payload[0] == 'd' && (char)payload[1] == 'e' && (char)payload[2] == 'l' && (char)payload[3] == 'h' && (char)payload[4] == 'i' && (char)payload[5] == 's'){
       const char* DApub;
       DApub = delAll(SD,"/history");
       client.publish(delAFile,DApub);   
     }
   }
}
*/
