#include <SoftwareSerial.h>
#include <FastLED.h>

#define ESP8266_rxPin 4
#define ESP8266_txPin 5
#define NUM_LEDS 50
#define NUM_LED_INIT 50
#define START_LED 0
#define LED_PIN 11
#define alarm_hour 18
#define alarm_minute 00 //ALARM MINUTE TIME
#define buttonPin 2

//SSID + KEY
const char SSID_ESP[] = "MooreCapel";
const char SSID_KEY[] = "13889Fox";

// THINGSPEAK APP
const char URL_current_time[] = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=C8WZYIN6EZC1J912 HTTP/1.0\r\n\r\n";

//MODES
const char CWMODE = '1';//CWMODE 1=STATION, 2=APMODE, 3=BOTH
const char CIPMUX = '1';//CWMODE 0=Single Connection, 1=Multiple Connections

//INITIALIZE
SoftwareSerial ESP8266(ESP8266_rxPin, ESP8266_txPin);// rx tx
CRGB led[NUM_LED_INIT];

//DEFINE ALL FUNCTIONS HERE
boolean setup_ESP();
boolean read_until_ESP(const char keyword1[], int key_size, int timeout_val, byte mode);
void timeout_start();
boolean timeout_check(int timeout_ms);
void serial_dump_ESP();
boolean connect_ESP();
void get_current_time();
void check_alarm();
void update_alarm();

//DEFINE ALL GLOBAL VAARIABLES HERE
unsigned long timeout_start_val; //TODO: Smaller data type?
char scratch_data_from_ESP[50];//first byte is the length of bytes
char payload[180];
byte payload_size=0, counter=0;
char ip_address[16];
char current_time[50];
int current_hour = 0; //TODO: Smaller data type?
int current_minute = 0; //TODO: Smaller data type?
int button = 0;

boolean alarm_on = 0;
int brightness = 50;
boolean reading = 0;
unsigned long currentLoopTime;
unsigned long setLoopTime;

//DEFINE KEYWORDS HERE
const char keyword_OK[] = "OK";
const char keyword_Ready[] = "Ready";
const char keyword_no_change[] = "no change";
const char keyword_blank[] = "#&";
const char keyword_ip[] = "192.";
const char keyword_rn[] = "\r\n";
const char keyword_quote[] = "\"";
const char keyword_carrot[] = ">";
const char keyword_sendok[] = "SEND OK";
const char keyword_linkdisc[] = "Unlink";

//keywords for current time
const char keyword_html_start_time[] = "ateTime";
const char keyword_html_end_time[] = "utcO";

void setup(){
  //Pin Modes for ESP TX/RX
  pinMode(ESP8266_rxPin, INPUT);
  pinMode(ESP8266_txPin, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LED_INIT);

  for(int xl = START_LED; xl < NUM_LEDS + START_LED; xl++){
    led[xl] = CHSV(0, 0, 0);
  }

  FastLED.show();
  
  ESP8266.begin(9600);//default baudrate for ESP
  ESP8266.listen();//not needed unless using other software serial instances TODO: Remove?
  Serial.begin(115200); //for status and debug
  delay(5000);//delay before starting the process
  
  setup_ESP();//go setup the ESP
  currentLoopTime = millis();
  setLoopTime = currentLoopTime; 
  Serial.println("Exiting Setup");
}

void loop(){
  button = digitalRead(buttonPin);
//  Serial.println(button);
  if(button != 0 && reading){
    Serial.println("Button pressed. Switching to alarm.");
    reading = 0;
    for(int xl = START_LED; xl < NUM_LEDS + START_LED; xl++){
      led[xl] = CHSV(55, 200, 0);
    }
    FastLED.show();
    delay(400);
  } else if(button != 0 && !reading){
    Serial.println("Button pressed. Switching to reading.");
    reading = 1;
    for(int xl = START_LED; xl < NUM_LEDS + START_LED; xl++){
      led[xl] = CHSV(55, 200, 255);
    }
    FastLED.show();
    delay(400);
  }

  if(!reading){
    currentLoopTime = millis();
    if(currentLoopTime >= (setLoopTime + 15000)){
      Serial.println("Starting update");
      get_current_time();//current time
      setLoopTime = currentLoopTime;
    }
  }
}
