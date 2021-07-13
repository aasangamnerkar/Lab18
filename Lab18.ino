SYSTEM_THREAD(ENABLED);
#include "MQTT.h"
#include "oled-wing-adafruit.h"
#include "blynk.h"

String auth_token = "cnjXDn_t7vcs3I-PR-jbSobGd__j2qM9";
String topic1 = "blocka";
String topic2 = "blocka2";
String topic3 = "blocka3";
bool valid = false;

WidgetMap myMap(V1);

double longitude = 0.0;
double latitude = 0.0;

void callback(char *topic, byte *payload, unsigned int length)
{
  //receive msg
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String s = p;
  double value = s.toFloat();

  //assign appropriate value
  if ((String) topic == "blocka2")
  {
    longitude = value;
  }
  if ((String) topic == "blocka3")
  {
    latitude = value;
  }

}

MQTT client("lab.thewcl.com", 1883, callback);
OledWingAdafruit display;



void printToDisplay(String output)
{
  //reset display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  //output to display
  display.println(output);
  display.display();
}

void setup()
{ 
  //set up display
  display.setup();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("");
  display.display();
  Serial.begin(9600);

  //cinnect to MQTT
  client.connect(System.deviceID());
  if (client.isConnected())
  {
    client.subscribe(topic2);
    client.publish(topic1,"hello world");
  }

  //conenct to blynk
  Blynk.begin(auth_token, IPAddress(167, 172, 234, 162), 9090);
  Blynk.run();

  //set RGB
  RGB.control(true);
  RGB.control(true);
  RGB.color(255, 255, 255);
  RGB.brightness(255);

}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  display.loop();
  client.connect(System.deviceID());
  
  //run MQTT when connected
  if (client.isConnected())
  {
    client.subscribe(topic2);
    client.subscribe(topic3);
    client.publish(topic1,"hello world");

    client.loop();

  }
  else
  {
    client.connect(System.deviceID());
  }

  //update OLED
  String output = "Long: " + (String) longitude;
  output += "\nLatitude: " + (String) latitude;
  printToDisplay(output);

  //map longitude and latitude
  myMap.location(1, latitude, longitude, "value");
}