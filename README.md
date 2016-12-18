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

Because the number of extra request headers needed may vary case to case, the library do not want to enforce a fix number. The user should be able to configure this in a flexible way.

The provided feature is to use a so called non-type parameter template class.

The library sets storage for 2 extra request headers by default. If you do not use this feature at all, or you do not exceed this number you can use the library like this:

```c++
BridgeHttpClient<> client;
```

However if you need more than 2 request headers, you must set the appropriate number like this:

```c++
BridgeHttpClient<4> client;
```

With this the library remains efficient and flexible at the same time.

__Adding Basic Auth. to your request__

```c++
void basicAuth(const char *user, const char *passwd);
```

__Adding extra request headers to your request__

```c++
int addHeader(const char *header);
```

__HTTPS requests__

If your HTTPS request fails due to the "certificate cannot be authenticated with known CA certificates" problem, you can circumvent the issue by calling the following method before the request.

```c++
void enableInsecure();
```

__Synchronous requests__

```c++
void get(const char *url);
void post(const char *url, const char *data);
void put(const char *url, const char *data);
void del(const char *url);
```

__Asynchronous requests__

```c++
void getAsync(const char *url);
void postAsync(const char *url, const char *data);
void putAsync(const char *url, const char *data);
void delAsync(const char *url);
```

To check whether the async request has completed or not, you can use the following method from the parent class.

```c++
boolean running();
```

If it's not running then it's completed.

__Exit status of the client__

After the sync or async request has finished, you can check the exit status of the client like this.

```c++
unsigned int exitValue();
```

__HTTP response code__

If you need the HTTP response code for your application, use the following method.

```c++
unsigned int getResponseCode();
```

__HTTP response headers__

To access all the received HTTP response headers, you can use this method.

```c++
String getResponseHeaders();
```

It returns all the headers in one string object. This is useful if you want to process the headers by yourself. If you're interested in a particular header value only, use the next method instead!

__HTTP response header value__

```c++
String getResponseHeaderValue(const String& header);
```

TODO
----

* Add proxy support
* Provide more examples
