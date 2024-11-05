#include <Arduino.h>


// #define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
// #define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;

void goToDeepSleep(){
  Serial.println("ESP32 Going to sleep");
  
  // esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * in_Seconds * in_uSecond);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}
void setup(){
  Serial.begin(115200);
  
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  Serial.flush();
  goToDeepSleep();

}

void loop(){

}