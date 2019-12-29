
//28 dec: things to finish
//  1. Test and debug triggering using switch and led
//  2. Write code for catpaw2, catpaw3 and cat
//  3. Test and switch triggering and full system by assembling things on breadboard
//  4. Check if anything else is left

// sd:/mp3/0001.mp3 = meow
// sd:/mp3/0002.mp3 = instruction
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <DFRobotDFPlayerMini.h>

//escape
int escape = 0;

//engage
int engage_switch = 3;
int engagemode;

//trigger
int trigger_threshhold;

//IR sensor stuff
int IRpin = A0;  // IR photodiode on analog pin A0
int distance = analogRead(IRpin);

//music stuff
SoftwareSerial mySoftwareSerial(1 , 2); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

//Wifi stuff
char ssid[] = "Bhat Home";          //  your network SSID (name)
char pass[] = "flashbhat";   // your network password

int status = WL_IDLE_STATUS;
IPAddress server(192, 168, 0, 103);
const uint16_t port = 8080;

// Initialize the client library
WiFiClient client;

int countRec = 0;

//functions
bool activated() {
  String code = client.readStringUntil('\n');
  if (code == "a") {
    return true;
  }
}

bool lifted(int threshhold) {
  if (distance > threshhold) {
    return true;
  }
}

void trigger() {
  client.write("1");
  delay(1000);
}


//Music functions
void meow() {
  myDFPlayer.play(1);
  Serial.print("Meow");
}

void instruction() {
  myDFPlayer.play(2);
  Serial.print("Instruction");
}

void setup() {

  //Wifi stuff
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.status());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("\nStarting connection...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");
  }
  
  int trigger_threshhold = 50;

  //engage button
  pinMode(engage_switch, INPUT);

  //Output
  //pinMode(11, OUTPUT);        // buzzer in digital pin 11

  //music stuff

  myDFPlayer.volume(10);  //Set volume value. From 0 to 30

}


void loop()
{

  //conditions and modes
  engagemode = digitalRead(engage_switch);
  while (engagemode == HIGH) {
    if (activated()) {
      if (lifted(trigger_threshhold)) //lifted condition to be apecified in the IR part
      {
        instruction();
        delay(7000);
        trigger();//trigger funstion to be defined
        delay(2000);
        escape = 1; //i think this is smart
        // break;//might work, will have to confirm
        //slightly doubtful about if it is kept lifted, it can trigger multiple triggers and can keep repeating instructions
      }
      else {
        meow();
        delay(3000);
      }
    }
  }
  while (escape = 1) {
    Serial.println("I am out");
    delay(10000000);
  }
}
