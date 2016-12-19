/*
  Asynchronous POST example

  What it does: Issues a POST request to httpbin using the async API.

  Please note that this sketch is Yun-specific!
  Works only with Yun and its clones like the Seeeduino Cloud, but not with
  an Uno or other Arduinos.

  Usage:
  1. Upload the sketch using a USB cable.
  2. Wait for the RED LED to light up on the board.
  3. Open the serial monitor in the Arduino IDE.

  Written by Imre Horvath, 2016
 */

#include <Bridge.h>
#include <BridgeHttpClient.h>

BridgeHttpClient client;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin(); // Initialize Bridge
  digitalWrite(13, HIGH);

  SerialUSB.begin(9600);
  while (!SerialUSB); // wait for a serial connection

  client.addHeader("X-Api-Key: 12345");
  client.addHeader("Accept: application/json");
  client.addHeader("Content-Type: application/json");

  client.enableInsecure(); // Using HTTPS and peer cert. will not be able to auth.

  client.postAsync("https://httpbin.org/post", "{\"key\":\"value\"}");
  SerialUSB.print("Sending request");
}

void loop() {

  if (client.finished()) {

    SerialUSB.println();
    SerialUSB.println("Response Body:");
    while (client.available() > 0) {
      char c = client.read();
      SerialUSB.print(c);
    }

    SerialUSB.print("Response Code: ");
    SerialUSB.println(client.getResponseCode());

    while (1) {} // stop

  } else {
    // not finished yet, wait and retry
    SerialUSB.print(".");
    delay(100);
  }
}
