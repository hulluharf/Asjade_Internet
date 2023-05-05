#include <Arduino.h>
#include <ittiot.h>
#include <Ticker.h>
#include <ClickEncoder.h>
 
#define WIFI_NAME "TalTech"
#define WIFI_PASSWORD ""
 
// Defining pins as need for the encoder
#define ENC_PINA 12
#define ENC_PINB 13
#define ENC_BTN   0
#define ENC_STEPS_PER_NOTCH 4
ClickEncoder encoder = ClickEncoder(ENC_PINA, ENC_PINB, ENC_BTN, ENC_STEPS_PER_NOTCH);
 
Ticker encTicker;
 
bool encFlag;
uint16_t button;
 
// Function started after the connection to the server is established.
void iot_connected()
{
  Serial.println("MQTT connected callback");
  iot.log("IoT encoder example!");
}
 
void setEncFlag()
{
    encFlag=true;
}
 
void setup()
{
  Serial.begin(115200); // setting up serial connection parameter
  Serial.println("Booting");
  iot.setConfig("wname", "TalTech");
  iot.setConfig("wpass", "");
  iot.setConfig("msrv", "193.40.245.72");
  iot.setConfig("mport", "1883");
  iot.setConfig("muser", "test");
  iot.setConfig("mpass", "test");
  //iot.setConfig("wname", WIFI_NAME);
  //iot.setConfig("wpass", WIFI_PASSWORD);
  iot.printConfig(); // print json config to serial
  iot.setup(); // Initialize IoT library
 
  // Activating additional button functions
  encoder.setButtonHeldEnabled(true);
  encoder.setDoubleClickEnabled(true);
 
  encTicker.attach(1, setEncFlag);
}
 
//Main code, which runs in loop
void loop()
{
  iot.handle(); // IoT behind the plan work, it should be periodically called
  delay(10); // wait for 0.01 second
  static uint32_t lastService = 0;
 

 
  if (lastService + 1000 < micros())
  {
    lastService = micros();
    encoder.service();
  }
 
  static int16_t last, value;
  value += encoder.getValue();
 
  // Send encoder reading to serial monitor, if it has changed
  if(value != last)
  {
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);
  }
  // Publishing encoder value in MQTT broker
  if(encFlag)
  {
      encFlag = false;
      String msg = String(value);
      iot.publishMsg("enc", msg.c_str());
  }
}