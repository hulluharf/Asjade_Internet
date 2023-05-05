#include <Arduino.h>
#include <ittiot.h>
#include <Ticker.h>
#include <ClickEncoder.h>

#define WIFI_NAME "TalTech"
#define WIFI_PASSWORD ""

// Defining pins as need for the encoder
#define ENC_PINA 12
#define ENC_PINB 13
#define ENC_STEPS_PER_NOTCH 4
ClickEncoder encoder = ClickEncoder(ENC_PINA, ENC_PINB, ENC_STEPS_PER_NOTCH);

Ticker encTicker;

bool encFlag;
int16_t peopleCount = 0;

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

  iot.setConfig("wname", WIFI_NAME);
  iot.setConfig("wpass", WIFI_PASSWORD);
  iot.setConfig("wname", "TalTech");
  iot.setConfig("wpass", "");
  iot.setConfig("msrv", "193.40.245.72");
  iot.setConfig("mport", "1883");
  iot.setConfig("muser", "test");
  iot.setConfig("mpass", "test");
  iot.printConfig(); // print json config to serial
  iot.setup(); // Initialize IoT library

  encTicker.attach(100, setEncFlag);
  Serial.print("Encoder4");
}

//Main code, which runs in loop
void loop()
{
  iot.handle(); // IoT behind the plan work, it should be periodically called
  //delay(10); // wait for 0.01 second
  static uint32_t lastService = 0;
  Serial.print("Encoder3");

  if (lastService + 1000 < micros())
  {
    lastService = micros();
    encoder.service();
    Serial.print("Encoder2");
  }
  Serial.print("Encoder1");
  static int16_t last, value;
  value += encoder.getValue();
  Serial.print("Encoder6");

  if(value != last)
  {
    Serial.print("Encoder5");
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);

    if (value > 0) {
      peopleCount++;
      Serial.print("People Count: ");
      Serial.println(peopleCount);
    } else if (value < 0) {
      peopleCount--;
      Serial.print("People Count: ");
      Serial.println(peopleCount);
    }

    encFlag = true;
  }

  if(encFlag)
  {
    encFlag = true;
    String msg = String(peopleCount);
    iot.publishMsg("enc", msg.c_str());
  }
}
