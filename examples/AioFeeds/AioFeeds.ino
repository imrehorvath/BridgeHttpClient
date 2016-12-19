/*
  Adafruit AIO REST API example

  What it does: Get your feeds on the Adafruit IO using the REST API.

  Please note that this sketch is Yun-specific!
  Works only with Yun and its clones like the Seeeduino Cloud, but not with
  an Uno or other Arduinos.

  Usage:
  1. Replace the placeholder XXXs below with your actual AIO KEY
  2. Upload the sketch using a USB cable.
  3. Wait for the RED LED to light up on the board.
  4. Open the serial monitor in the Arduino IDE.

  Written by Imre Horvath, 2016
 */

#include <Bridge.h>
#include <BridgeHttpClient.h>

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

  // Dump the response body
  SerialUSB.println("Response Body:");
  while (client.available() > 0) {
    char c = client.read();
    SerialUSB.print(c);
  }

  // Print the HTTP response code
  SerialUSB.print("Response code: ");
  SerialUSB.println(client.getResponseCode());

  // Print a response header value
  String server;
  if (client.getResponseHeaderValue("Server", server)) {
    SerialUSB.print("Header \"Server\" has value: ");
    SerialUSB.println(server);
  } else {
    SerialUSB.println("Header \"Server\" not found");
  }
}

void loop() {
  // Do nothing
}
