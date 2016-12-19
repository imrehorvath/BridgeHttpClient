A practical and easy to use generic HTTP client library for the Yun
===================================================================

Features
--------

* GET/POST/PUT/DELETE
* Both sync and async requests
* You can access the HTTP response code and headers, not just the body
* Basic authorization
* Multiple extra request headers
* Extraction of a particular response header value
* Designed for efficiency and easy usage
* Built on top of the Bridge library
* Uses cURL on the Linux side

Usage
-----

__First instantiate a client__

```c++
BridgeHttpClient client;
```

__Adding Basic Auth. to your request (optional)__

```c++
client.basicAuth("user", "password");
```

__Adding extra request headers to your request (optional)__

```c++
client.addHeader("X-Api-Key: 12345");
client.addHeader("Accept: application/json");
client.addHeader("Content-Type: application/json");
```

_Note:_ The library supports up to 16 extra user addable request headers.

__When issuing HTTPS requests consider this__

If your HTTPS request fails due to the "certificate cannot be authenticated with known CA certificates" problem, you can circumvent the issue by calling the following method before the request.

```c++
client.enableInsecure();
```

__Synchronous requests__

```c++
// HTTP GET, call won't return until finished
client.get("https://httpbin.org/headers");

// HTTP POST a JSON payload, call won't return until finished
client.post("https://httpbin.org/post", "{\"key\":\"value\"}");
```

__Asynchronous requests__

To check whether the async request has completed or not, you can use the following method from the parent class.

```c++
// HTTP GET, call returns before completion.
client.getAsync("https://httpbin.org/headers");
// ...
// Later you can check whether the call has finished
if (client.finished()) {
  // Request has finished
}
```

__Exit status of the client__

After the sync or async request has finished, you can check the exit status of the client like this.

```c++
if (client.exitValue() == 0) {
  // Success, continue processing
} else {
  // Error
}
```

__HTTP response code__

If you need the HTTP response code for your application, use the following method.

```c++
if (client.getResponseCode() == 200) {
  // HTTP OK
}
```

__HTTP response headers__

To access all the received HTTP response headers, you can use this method.

```c++
String responseHeaders = getResponseHeaders();
// Process response headers by yourself
```

_Note:_ It returns all the headers in one string object. This is useful if you want to process the headers by yourself. If you're interested in a particular header value only, use the next method instead!

__HTTP response header value__

```c++
String server;
if (client.getResponseHeaderValue("Server", server)) {
  SerialUSB.print("Header \"Server\" has value: ");
  SerialUSB.println(server);
} else {
  SerialUSB.println("Header \"Server\" not found");
}
```

TODO
----

* Add proxy support
* Provide better description
* Create useful examples
