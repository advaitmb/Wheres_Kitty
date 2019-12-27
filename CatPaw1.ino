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
int IRpin = A0;               // IR photodiode on analog pin A0
int IRemitter = 2;            // IR emitter LED on digital pin 2
int ambientIR;                // variable to store the IR coming from the ambient
int obstacleIR;               // variable to store the IR coming from the object
int value[10];                // variable to store the IR values
int distance;                 // variable that will tell if there is an obstacle or not

//music stuff
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

//Wifi stuff
char ssid[] = "Bhat Home";          //  your network SSID (name)
char pass[] = "flashbhat";   // your network password

int status = WL_IDLE_STATUS;
IPAddress server(192, 168, 0, 104);
int port = 8081;

// Initialize the client library
WiFiClient client;

int countRec = 0;

//functions
bool activated() {
  String code = client.readStringUntil('\n');
  if (code == "activate") {
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

//function to read distance from ground
int readIR(int times) {
  for (int x = 0; x < times; x++) {
    digitalWrite(IRemitter, LOW);          // turning the IR LEDs off to read the IR coming from the ambient
    delay(1);                                             // minimum delay necessary to read values
    ambientIR = analogRead(IRpin);  // storing IR coming from the ambient
    digitalWrite(IRemitter, HIGH);         // turning the IR LEDs on to read the IR coming from the obstacle
    delay(1);                                             // minimum delay necessary to read values
    obstacleIR = analogRead(IRpin);  // storing IR coming from the obstacle
    value[x] = ambientIR - obstacleIR; // calculating changes in IR values and storing it for future average
  }

  for (int x = 0; x < times; x++) { // calculating the average based on the "accuracy"
    distance += value[x];
  }
  return (distance / times);         // return the final value
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

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("\nStarting connection...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");
  }


  //IR stuff
  pinMode(IRemitter, OUTPUT); // IR emitter LED on digital pin 2
  digitalWrite(IRemitter, LOW); // setup IR LED as off
  distance = readIR(5); // Distance from ground with accuracy 5


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
  if (engagemode == HIGH) {
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
//4 main parts

//checking for mode: engaged or disengaged
//if engaged: play meow
//checking for condition: lifted or kept-down
//playing instructions
//tiggering
//disengage

//tech parts:
//DFPlayer
//IR sensing
//Serial shit
