/*
    Copyright (c) 2016 Imre Horvath. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _BRIDGE_HTTP_CLIENT_H_
#define _BRIDGE_HTTP_CLIENT_H_

#include <Process.h>

template <size_t C=2>
class BridgeHttpClient : public Process {

  public:
    BridgeHttpClient() : _isInsecureEnabled(false),
                         _user(NULL), _passwd(NULL),
                         _extraHeaderIdx(0) {}

    void get(const char *url) {
      request("GET", url, NULL);
    }

    void post(const char *url, const char *data) {
      request("POST", url, data);
    }

    void put(const char *url, const char *data) {
      request("PUT", url, data);
    }

    void del(const char *url) {
      request("DELETE", url, NULL);
    }

    /**
     * Call this method after the request has finished,
     * to get the HTTP response code from the server.
     *
     * Returns the response code eg. 200
     */
    unsigned int getResponseCode() {
      Process p;
      p.runShellCommand("head -n 1 " + _tempFileName + " | cut -d' ' -f2");
      return p.parseInt();
    }

    /**
     * Call this method after the request has finished,
     * to get the response headers received from the server.
     *
     * Returns a String object with all the response headers included.
     */
    String getResponseHeaders() {
      String responseHeaders;

      Process p;
      p.runShellCommand("tail -n +2 " + _tempFileName);

      while (p.available() > 0) {
        char c = p.read();
        responseHeaders += c;
      }

      responseHeaders.trim();
      return responseHeaders;
    }

    // unsigned int exitValue();  // Method in superclass to retrieve the exit status
    // boolean running();  // Method in superclass which can be used to check if the async request is still running or has finished.

    void getAsync(const char *url) {
      request("GET", url, NULL, true);
    }

    void postAsync(const char *url, const char *data) {
      request("POST", url, data, true);
    }

    void putAsync(const char *url, const char *data) {
      request("PUT", url, data, true);
    }

    void delAsync(const char *url) {
      request("DELETE", url, NULL, true);
    }

    /**
     * Call this method before issuing the requests,
     * to allows "insecure" SSL.
     *
     * Useful in the following case for example:
     * Certificate cannot be authenticated with known CA certificates.
     */
    void enableInsecure() {
      _isInsecureEnabled = true;
    }

    /**
     * Call this method before issuing the request,
     * to add an extra header to the request.
     *
     * Returns 0 if the header fits into the array of headers. -1 otherwise.
     */
    int addHeader(const char *header) {
      if (_extraHeaderIdx < C) {
        _extraHeaders[_extraHeaderIdx++] = header;
        return 0;
      }
      return -1;
    }

    /**
     * Call this method before issuing the request,
     * to include basic authorization into the request.
     */
    void basicAuth(const char *user, const char *passwd) {
      _user = user;
      _passwd = passwd;
    }

    /**
     * Call this method between the different request calls on the same client,
     * to clear/setup the request headers for the next call.
     */
    void clearHeaders() {
      _extraHeaderIdx = 0;
    }

    /**
     * Call this method between the different request calls on the same client,
     * to clear the previously set basic authorization for the subsequent call.
     */
    void clearAuth() {
      _user = _passwd = NULL;
    }

    /**
     * Call this method after the request has finished,
     * to get a particular response header value.
     *
     * Returns a String object representing the response header value
     */
    String getResponseHeaderValue(const String& header) {
      if (_cachedRespHeaders.length() == 0) {
        _cachedRespHeaders = getResponseHeaders();
      }
      int startOfValue = _cachedRespHeaders.indexOf(':', _cachedRespHeaders.indexOf(header)) + 1;
      String respValue = _cachedRespHeaders.substring(startOfValue,
                                                     _cachedRespHeaders.indexOf('\n', startOfValue));
      respValue.trim();
      return respValue;
    }

  private:
    String _tempFileName;

    bool _isInsecureEnabled;

    const char *_user;
    const char *_passwd;

    const char *_extraHeaders[C];
    int _extraHeaderIdx;

    String _cachedRespHeaders;

    /**
     * Sends the actual request via cURL on the Linux side
     */
    void request(const char *verb, const char *url, const char *data, bool async=false) {
      Process p;
      p.runShellCommand("mktemp");
      _tempFileName = p.readStringUntil('\n');

      clearCachedRespHeaders();

      begin("curl");
      addParameter("-D");
      addParameter(_tempFileName);

      if (verb != "GET") {
        addParameter("-X");
        addParameter(verb);
      }

      if (_isInsecureEnabled) {
        addParameter("-k");
      }

      if (_user && _passwd) {
        String auth;
        auth += _user;
        auth += ":";
        auth += _passwd;

        addParameter("-u");
        addParameter(auth);
      }

      for (int i = 0; i < _extraHeaderIdx; i++) {
        addParameter("-H");
        addParameter(_extraHeaders[i]);
      }

      if (data != NULL) {
        addParameter("-d");
        addParameter(data);
      }

      addParameter(url);

      if (async) {
        runAsynchronously();
      } else {
        (void) run();
      }
    }

    /**
     * Clears the cached response headers between subsequent requests
     */
    void clearCachedRespHeaders() {
      _cachedRespHeaders = "";
    }
};

#endif
