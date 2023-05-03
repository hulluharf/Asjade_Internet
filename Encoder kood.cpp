#include <Arduino.h>
#include <ittiot.h>
#include <Ticker.h>
#include <ClickEncoder.h>

#define WIFI_NAME "TalTech"
#define WIFI_PASSWORD ""

#define ENC_PINA 12
#define ENC_PINB 13
#define ENC_BTN 0
#define ENC_STEPS_PER_NOTCH 4
ClickEncoder encoder(ENC_PINA, ENC_PINB, ENC_BTN, ENC_STEPS_PER_NOTCH);

Ticker encTicker;

bool encFlag;
uint16_t button;

bool personDetected = false;

void iot_connected() {
    // Send message to serial port to show that connection is established
    Serial.println("MQTT connected callback");
    // Send message to MQTT server to show that connection is established
    iot.log("IoT encoder example!");
}

void setEncFlag() {
    encFlag = true;
}

void setup() {
    // Initialize serial port and send message
    // setting up serial connection parameter
    Serial.begin(115200);
    Serial.println("Booting");

    iot.setConfig("wname", WIFI_NAME);
    iot.setConfig("wpass", WIFI_PASSWORD);
    iot.setConfig("msrv", "193.40.245.72");
    iot.setConfig("mport", "1883");
    iot.setConfig("muser", "test");
    iot.setConfig("mpass", "test");
    iot.printConfig();
    iot.setup(); // Initialize IoT library
   
    encoder.setButtonHeldEnabled(true);
    encoder.setDoubleClickEnabled(true);

    encTicker.attach(1, setEncFlag);
}

void loop() {
    iot.handle();
    delay(10);
    static uint32_t lastService = 0;

    if (analogRead(A0) < 100) {
        if (!personDetected) {
            personDetected = true;
            Serial.println("Person entered the room");
            encoder.setValue(encoder.getValue() + 1); //Reduces the encoder value by 1 step in the counter-clockwise direction
        }
    }
    else {
        if (personDetected) {
            personDetected = false;
            Serial.println("Person exited the room");
            encoder.setValue(encoder.getValue() - 1);
        }
    }

    if (lastService + 1000 < micros()) {
        lastService = micros();
        encoder.service();
    }

    static int16_t last, value;
    value = encoder.getValue();

    if (value != last) {
        last = value;
        Serial.print("Encoder Value: ");
        Serial.println(value);
    }

    if (encFlag) {
        encFlag = false;
        String msg = String(value);
        iot.publishMsg("MQTT_Topic", msg.c_str());//change the topic when you get it
    }
}
