/*
  Adafruit AIO REST API example

  What it does: Get your feeds on the Adafruit IO using the REST API.

  Please note that this sketch is Yun-specific!
  Works only with Yun and its clones like the Seeeduino Cloud, but not with
  an Uno or other Arduinos.

  Dependencies: ArduinoJson library

  Usage:
  1. Make sure you've installed the ArduinoJson library in the Library Manager
  2. Replace the placeholder XXXs below with your actual AIO KEY
  3. Upload the sketch using a USB cable.
  4. Wait for the RED LED to light up on the board.
  5. Open the serial monitor in the Arduino IDE.

  Written by Imre Horvath, 2016
 */

#include <Bridge.h>
#include <BridgeHttpClient.h>

#include <ArduinoJson.h>

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin(); // Initialize Bridge
  digitalWrite(13, HIGH);

  SerialUSB.begin(9600);
  while (!SerialUSB); // wait for the serial connection

  BridgeHttpClient client;

  // Add request headers
  // REPLACE THE XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX WITH YOUR AIO KEY!!!
  client.addHeader("X-AIO-Key: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  client.addHeader("Accept: application/json");

  // Using HTTPS and peer cert. will not be able to auth.
  client.enableInsecure();

  // Adafruit IO REST API call
  client.get("https://io.adafruit.com/api/feeds");

  // Collect the response body into this string for parsing
  String response;

  // Dump the response body
  SerialUSB.println("Response Body:");
  while (client.available() > 0) {
    char c = client.read();
    response += c;
    SerialUSB.print(c);
  }

  // Print the HTTP response code
  SerialUSB.print("Response code: ");
  SerialUSB.println(client.getResponseCode());

  // Parse the list of feeds and print the name and ids, limited to 4 feeds
  const int JSON_BUFFER = JSON_ARRAY_SIZE(4) + 4*JSON_OBJECT_SIZE(14);
  StaticJsonBuffer<JSON_BUFFER> jsonBuffer;

  JsonArray& array = jsonBuffer.parseArray(response);
  if (!array.success()) {
    SerialUSB.println("parseArray() failed");
    while (1) {}
  }

  for (JsonArray::iterator it = array.begin(); it != array.end(); ++it) {
    JsonObject& feed = *it;

    SerialUSB.print("Feed name: ");
    feed["name"].printTo(SerialUSB);
    SerialUSB.println();

    SerialUSB.print("Feed id: ");
    feed["id"].printTo(SerialUSB);
    SerialUSB.println();
  }
}

void loop() {
  // Do nothing
}
