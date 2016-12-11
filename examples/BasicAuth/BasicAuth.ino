
#include <Bridge.h>
#include <BridgeHttpClient.h>

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin(); // Initialize Bridge
  digitalWrite(13, HIGH);

  SerialUSB.begin(9600);
  while (!SerialUSB); // wait for a serial connection

  BridgeHttpClient client;
  client.basicAuth("user", "password");

  client.enableInsecure(); // Using HTTPS and peer cert. will not be able to auth.

  client.get("https://httpbin.org/headers");

  SerialUSB.print("Exit status: ");
  SerialUSB.println(client.exitValue());

  if (client.exitValue() == 0) {
    SerialUSB.print("Response Code: ");
    SerialUSB.println(client.getResponseCode());

    SerialUSB.println("Response Headers:");
    SerialUSB.println(client.getResponseHeaders());

    SerialUSB.println("Response Body:");
    while (client.available() > 0) {
      char c = client.read();
      SerialUSB.print(c);
    }
  }
}

void loop() {
}
