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

API
---

```c++
void get(const char *url);
void post(const char *url, const char *data);
void put(const char *url, const char *data);
void del(const char *url);

/**
 * Call this method after the request has finished,
 * to get the HTTP response code from the server.
 *
 * Returns the response code eg. 200
 */
unsigned int getResponseCode();

/**
 * Call this method after the request has finished,
 * to get the response headers received from the server.
 *
 * Returns a String object with all the response headers included.
 */
String getResponseHeaders();

// Method in superclass to retrieve the exit status
// unsigned int exitValue();

// Method in superclass which can be used to check if the async request is still running or has finished
// boolean running();

void getAsync(const char *url);
void postAsync(const char *url, const char *data);
void putAsync(const char *url, const char *data);
void delAsync(const char *url);

/**
 * Call this method before issuing the requests,
 * to allows "insecure" SSL.
 *
 * Useful in the following case for example:
 * Certificate cannot be authenticated with known CA certificates.
 */
void enableInsecure();

/**
 * Call this method before issuing the request,
 * to add an extra header to the request.
 *
 * Returns 0 if the header fits into the array of headers. -1 otherwise.
 */
int addHeader(const char *header);

/**
 * Call this method before issuing the request,
 * to include basic authorization into the request.
 */
void basicAuth(const char *user, const char *passwd);

/**
 * Call this method between the different request calls on the same client,
 * to clear/setup the request headers for the next call.
 */
void clearHeaders();

/**
 * Call this method between the different request calls on the same client,
 * to clear the previously set basic authorization for the subsequent call.
 */
void clearAuth();

/**
 * Call this method after the request has finished,
 * to get a particular response header value.
 *
 * Returns a String object representing the response header value
 */
String getResponseHeaderValue(const String& header);
```

TODO
----

* Add proxy support
* Provide more examples
