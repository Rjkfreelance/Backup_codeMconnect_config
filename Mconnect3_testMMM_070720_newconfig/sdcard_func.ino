void sdbegin()
{
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  } else {
    Serial.println("SD Card OK");
  }

}
void assignConfig(fs::FS &fs, const char* path) {
  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open directory");
    return;
  }
  
  Serial.print("Reading file: ");
  Serial.println(path);
    int n =0;
    while (file.available()) {
     eachline = file.readStringUntil('\n');    
     int posi = eachline.indexOf(':');
     String val = eachline.substring(posi+1);
      //Serial.println(val);
      Line[n] = val;
      Line[n].trim();
      n++;
    }
   wifi_ssid = (const char*)Line[0].c_str();
   
   wifi_pwd = (const char*)Line[1].c_str();
  
   mqtt_server = (const char*)Line[2].c_str();
 
   mqttPort = Line[3].toInt();
   
   //Serial.println(mqtt_server);//debug ok
   //Serial.println(mqttPort);//debug ok

   mqttUser = (const char*)Line[4].c_str();

   mqttPassword = (const char*)Line[5].c_str();
   
   clientId = (const char*)Line[6].c_str();
 
   otatimeout = Line[7].toInt();
   sendtopic = (const char*)Line[8].c_str();
  
   gtopic = (const char*)Line[9].c_str();
   ctopic = (const char*)Line[10].c_str();
   stopic = (const char*)Line[11].c_str();//otamachine 
   dbreply =  (const char*)Line[12].c_str(); // add by rjk
   getconf = (const char*)Line[13].c_str();
   //sendperiod = (const char*)Line[14].c_str();// add by rjk
   sendperiod = Line[14];
   ackota = (const char*)Line[15].c_str();
   sendconf = (const char*)Line[16].c_str();
   cirmachine = Line[17].toInt();
}

String readcert(fs::FS &fs, const char* path){
 File fr = fs.open(path);
  String SL = "";
  while(fr.available()){
    SL += fr.readStringUntil('\n');
    }
   fr.close();
  return SL;
}
void listcountfileindir(fs::FS &fs, const char * dirname) {
  int chkfile = 0;
  const char *historydelname;
  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  if (file) {
    delay(50);
    historydelname = file.name();
    delay(50);
    buffilenamedel = historydelname;
  }
  while (file) {
    file = root.openNextFile();
    chkfile++;
  }
  countfileinsd = chkfile;
}

const char* listDir(fs::FS &ff, const char * dirname, uint8_t levels) {
  const char *historyfilename;
  Serial.printf("Listing directory: %s\n", dirname);
  File root = ff.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return "";
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return "";
  }

  File file = root.openNextFile();
  if (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(ff, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.println(file.name());
      delay(50);
      historyfilename = file.name();
      delay(50);
      Serial.print("filenameinsdcard : ");
      Serial.println(historyfilename);
      return historyfilename;
    }
    file = root.openNextFile();
  }
  historyfilename = "0";
  return historyfilename;

}

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}


void readFileinSD(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.print("Reading file: ");
  Serial.println(path);
  while (file.available()) {
    datainfilesd += file.readStringUntil('\n');
  }  file.close();
}

int writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.print("Writing file: ");
  Serial.println(path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return file;
  }
  if (file.print(message)) {
    Serial.println("File written");

  } else {
    Serial.println("Write failed");
  }
  file.close();
  return file;
}

void WriteSDcfg(fs::FS &fs, const char * path,String cdata){
  Serial.print(F("         WRITE NEW CONFIG To SD card     "));
    File iotfmx = fs.open(path, FILE_WRITE);
   
   iotfmx = SD.open(path,FILE_WRITE);
    iotfmx.print(cdata);
       iotfmx.close();
     
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void ReadConfig(fs::FS &fs, const char* path) {
  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open directory");
    return;
  }
   int n =0;
    while (file.available()) {
     eachline = file.readStringUntil('\n');    
      Serial.println(eachline);
      n++;
    }
  file.close();
}

void ReadSendConfig(fs::FS &fs, const char* path) {
  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open directory");
    return;
  }
   int n =0;
    while (file.available()) {
     String L = file.readStringUntil('\n');    
      int posi = L.indexOf(':');
       if(n < 17){
         Eachline += L.substring(posi+1)+",";
       }else{
         Eachline += L.substring(posi+1);
       }
      n++;
    }
  file.close();
  Eachline = "#CF,"+Eachline;
}

void SetSDcfg(fs::FS &fs, const char * path,const char* cdata){
 Serial.printf("Writing file: %s\n", path);
 
       File file = fs.open(path, FILE_WRITE);
        if(!file){
           Serial.println("Failed to open file for writing");
            return;
          }
         if(file.print(cdata)){
               Serial.println("File New config written");
           } else {
               Serial.println("Write failed");
           }
   
}
