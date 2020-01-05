#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <DFMiniMp3.h>

class Mp3Notify
{
  public:
    static void OnError(uint16_t errorCode)
    {
      // see DfMp3_Error for code meaning
      Serial.println();
      Serial.print("Com Error ");
      Serial.println(errorCode);
    }

    static void OnPlayFinished(uint16_t globalTrack)
    {
      Serial.println();
      Serial.print("Play finished for #");
      Serial.println(globalTrack);
    }

    static void OnCardOnline(uint16_t code)
    {
      Serial.println();
      Serial.print("Card online ");
      Serial.println(code);
    }

    static void OnUsbOnline(uint16_t code)
    {
      Serial.println();
      Serial.print("USB Disk online ");
      Serial.println(code);
    }

    static void OnCardInserted(uint16_t code)
    {
      Serial.println();
      Serial.print("Card inserted ");
      Serial.println(code);
    }

    static void OnUsbInserted(uint16_t code)
    {
      Serial.println();
      Serial.print("USB Disk inserted ");
      Serial.println(code);
    }

    static void OnCardRemoved(uint16_t code)
    {
      Serial.println();
      Serial.print("Card removed ");
      Serial.println(code);
    }

    static void OnUsbRemoved(uint16_t code)
    {
      Serial.println();
      Serial.print("USB Disk removed ");
      Serial.println(code);
    }
};

SoftwareSerial secondarySerial(D7, D6); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);

char ssid[] = "Bhat Home";    //  your network SSID (name)
char pass[] = "flashbhat";   // your network password

int status = WL_IDLE_STATUS;
IPAddress server(192, 168, 0, 110);
const uint16_t port = 8080;

// Initialize the client library
WiFiClient client;


uint16_t IRpin = D4;  // IR photodiode on analog pin D4
int engage = D3;
int escape;
int activated;

void setup() {

  escape = 0;
  activated = 0;

  pinMode(engage, INPUT);
  pinMode(IRpin, INPUT);
  digitalWrite(engage, HIGH);

  Serial.begin(115200);

  Serial.println("initializing...");

  mp3.begin();

  uint16_t volume = mp3.getVolume();
  Serial.print("volume ");
  Serial.println(volume);
  mp3.setVolume(24);

  uint16_t count = mp3.getTotalTrackCount();
  Serial.print("files ");
  Serial.println(count);

  Serial.println("starting...");


  //Wifi stuff
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
  if (client.connect(server, port))
    Serial.println("connected");
}

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();

  while ((millis() - start) < msWait)
  {
    // calling mp3.loop() periodically allows for notifications
    // to be handled without interrupts
    mp3.loop();
    delay(1);
  }
}

void loop()
{
  //Enagage
  while (!digitalRead(engage))
  {
    Serial.println("engaged");


    if (escape == 0) {

      //Activate
      String code = client.readStringUntil('\n');
      Serial.println(code);
      if ( code == "a") {
        activated = 1;
      }

      if (activated == 1) {
        //Meow
        Serial.println("meow");
        mp3.playMp3FolderTrack(2);  // sd:/mp3/0002.mp3
        //IR Read

        if (!digitalRead(IRpin))
        {
          mp3.playMp3FolderTrack(1);  // sd:/mp3/0001.mp3
          client.write(1);
          Serial.println("f");
          delay(100);
          escape = 1;
        }
        delay(10);
      }
      delay(10);
    }
    delay(10);
  }
  delay(10);
}

