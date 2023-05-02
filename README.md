IoT Encoder Example
This is an example code for an IoT Encoder using an ESP32 microcontroller, an infrared sensor and an MQTT broker. The code monitors the presence of a person in the room using the infrared sensor and increments an encoder value based on the person's entry or exit. The encoder value is published to the MQTT broker using the iTTiOT library.

Prerequisites
Before running this code, you will need the following:

An ESP32 microcontroller
An infrared sensor
An MQTT broker
Arduino IDE
iTTiOT library
Getting Started
Set the WiFi name and password by uncommenting the iot.setConfig("wname", WIFI_NAME) and iot.setConfig("wpass", WIFI_PASSWORD) lines and providing the WiFi name and password.
Connect the infrared sensor to analog pin A0 and the encoder to digital pins 12, 13 and 0 (for the button).
Install the iTTiOT library using the Arduino IDE library manager.
Upload the code to the ESP32 microcontroller.
Usage
The code monitors the presence of a person in the room using the infrared sensor and increments the encoder value based on the person's entry or exit. The encoder value is published to the MQTT broker using the iTTiOT library.

When the sensor reading indicates that a person has entered the room (analog value less than 100), the code sets personDetected to true and increments the encoder.clicks variable by 1, which will result in a single click being sent to the MQTT broker. When the sensor reading indicates that a person has exited the room (analog value greater than or equal to 100), the code sets personDetected to false and increments the encoder.clicks variable by 2, which will result in two clicks being sent to the MQTT broker. The encoder.clicks variable is used to accumulate the number of clicks to be sent to the MQTT broker, and is reset by the encoder.service() function after each publish.

Authors
[Your Name]
License
This project is licensed under the MIT License.
