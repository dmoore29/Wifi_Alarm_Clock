void check_alarm(){

  if(alarm_hour == current_hour && alarm_minute == current_minute){
    alarm_on = 1;
  }
  
  Serial.print(F("Alarm state = "));
  Serial.print(alarm_on);
  Serial.println("");

  

  if(current_minute == alarm_minute + 20){
    alarm_on = 0;
    for(int xl = START_LED; xl < NUM_LEDS + START_LED; xl++){
        led[xl] = CHSV(10, 0, 0);
    }
    FastLED.show();
  }
  
  if (alarm_on == 1){
    update_alarm();
  }
  Serial.println("Finished alarm check.");
} //check alarm end
