void get_current_time(){
  counter=0;
  payload_size=0;
  for(int i=0; i<(sizeof(URL_current_time)-1); i++){
  payload[payload_size+i] = URL_current_time[i];
  counter++;
  }//for int
     payload_size = counter+payload_size;
     if(connect_ESP()){

      if(read_until_ESP(keyword_html_start_time,sizeof(keyword_html_start_time),5000,0)){
      if(read_until_ESP(keyword_html_end_time,sizeof(keyword_html_end_time),5000,1)){
      for(int i=1; i<=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_time)+1); i++)// i=0 is total lenght including key, so sub the key length-1, not < since we're not starting at 0
      current_time[i]=scratch_data_from_ESP[i];
      current_time[0]=(scratch_data_from_ESP[0]-sizeof(keyword_html_end_time)+1);   
      
      if(read_until_ESP(keyword_linkdisc,sizeof(keyword_linkdisc),5000,0)){
      Serial.println(F("FOUND DATA & DISCONNECTED"));
      serial_dump_ESP();

      
      }//link
       
     }//end  time
     }//start time

  }//connect ESP

  Serial.print(F("Current Time="));

  //parse data
  current_hour = atoi(current_time+15);
  current_minute = atoi(current_time+18);
     
  //print time
  if(current_hour >= 13){
    Serial.print(current_hour - 12);
  } else {
    Serial.print(current_hour);
  }
  Serial.print(":");
  if(current_minute <= 9){
    Serial.print("0");
    Serial.print(current_minute);
  } else {
    Serial.print(current_minute);
  }
  Serial.println("");
  delay(1000);
  
  check_alarm();


  
}//get current time
