//输入reset为重启
//输入键名＋','字符改变键值
#include <Arduino.h>
#include <Preferences.h>

String comdata="";
char*myPins[] = {"MQTT_PORT","MQTT_SERVER","MQTT_BTN_STATE","MQTT_BTN_MESS",
                 "MQTT_TASK_MESS","MQTT_AGV_M_MESS","WIFI_SSID","WIFI_PASSWD",
                 "BUTTON_NAME","NTP_SERVER"};//键名不能超过15位
char*myPins_first[] = {"1883","192.168.31.46","iqr/default/callbutton/state",
                       "iqr/default/callbutton/message","iqr/default/task/message",
                       "iqr/default/agv_manager/message","ip-5G","88888888","btn_01","192.168.0.2"};
int myPins_sum=0;
void setup() {
 for(int i = 0;i<sizeof(myPins)/sizeof(myPins[0]);i++){
    myPins_sum=i+1;
    }
  Serial.begin(115200);
  Serial.println();
  delay(100);
  Preferences prefs;
  prefs.begin("mynamespace");
  for(int i=0;i<myPins_sum;i++){
     if(prefs.isKey(myPins[i])){
    Serial.printf("%s",myPins[i]);
    Serial.printf(":%s\n", prefs.getString(myPins[i]).c_str());
    }
   else {
    Serial.printf("%s",myPins[i]);
    Serial.println("is not found");
    Serial.printf("%s",myPins[i]);
    Serial.printf(":%s\n",myPins_first[i]);
    prefs.putString(myPins[i],myPins_first[i]);
    delay(100);
    }
  }
  prefs.end();
}
void loop() {
  Preferences prefs;
  prefs.begin("mynamespace");

  if(Serial.available()>0) {
    String comdata="";
    String str1="";
    String str2="";
    while(Serial.available()>0) {
      comdata+=char(Serial.read());
      delay(10);
    }
    if(comdata.length()>0) {
      str1=comdata;
      Serial.printf("recv:%s\n", comdata.c_str());
      if(str1.indexOf("reset")>=0){ 
        prefs.clear();
        ESP.restart();}
        //restart 消除所有键值
      if(str1.indexOf(',')>=0) { 
        for(int i=0;i<myPins_sum; i++){
          int num_2=str1.indexOf(',');
        
          if(str1.indexOf(myPins[i])>=0){
            num_2=num_2+1;
            str2=str1.substring(num_2);
            prefs.putString(myPins[i],String(str2));
            delay(100);
          }
        } 
      }
     if(str1.indexOf("format")>=0){
        for(int i=0;i<myPins_sum; i++){
            if(str1.indexOf(myPins[i])>=0){
            prefs.putString(myPins[i],myPins_first[i]);
            Serial.printf("%s:",myPins[i]);
            Serial.printf("%s",myPins_first[i]);
            delay(100);
            //键名+format 重置单个键值处置
            }
        }
     }
    }
  }
 }
