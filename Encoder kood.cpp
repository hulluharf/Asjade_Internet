#include <Arduino.h>
#include <ittiot.h>
#include <Ticker.h>
#include <ClickEncoder.h>

#define WIFI_NAME "name"
#define WIFI_PASSWORD "password"

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
    Serial.println("MQTT connected callback");
    iot.log("IoT encoder example!");
}

void setEncFlag() {
    encFlag = true;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Booting");

    // iot.setConfig("wname", WIFI_NAME);
    // iot.setConfig("wpass", WIFI_PASSWORD);
    iot.printConfig();
    iot.setup();

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
            encoder.clicks++;
        }
    }
    else {
        if (personDetected) {
            personDetected = false;
            Serial.println("Person exited the room");
            encoder.clicks += 2;
        }
    }

    if (lastService + 1000 < micros()) {
        lastService = micros();
        encoder.service();
    }

    static int16_t last, value;
    value += encoder.getValue();

    if (value != last) {
        last = value;
        Serial.print("Encoder Value: ");
        Serial.println(value);
    }

    if (encFlag) {
        encFlag = false;
        String msg = String(value);
        iot.publishMsg("enc", msg.c_str());
    }
}







