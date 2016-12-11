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

class BridgeHttpClient : public Process {

  public:
    BridgeHttpClient() : isInsecureEnabled(false),
                         user(NULL), passwd(NULL),
                         headerIndex(0) {}

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

    // unsigned int exitValue();  // Method in superclass to retrieve the exit status
    // boolean running();  // Method in superclass which can be used to check if the async request is still running or has finished.

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
    void enableInsecure() { isInsecureEnabled = true; }

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
    void clearHeaders() { headerIndex = 0; }

    /**
     * Call this method between the different request calls on the same client,
     * to clear the previously set basic authorization for the subsequent call.
     */
    void clearAuth() { user = passwd = NULL; }

    /**
     * Call this method after the request has finished,
     * to get a particular response header value.
     *
     * Returns a String object representing the response header value
     */
    String getResponseHeaderValue(const String& header);

  private:
    String tempFileName;
    bool isInsecureEnabled;
    const char *user;
    const char *passwd;
    static const int HEADERCNT = 10; // We handle max. 10 extra request headers
    const char *headers[HEADERCNT];
    int headerIndex;
    String cachedRespHeaders;

    void request(const char *verb, const char *url, const char *data, bool async=false);
    void clearCachedRespHeaders() { cachedRespHeaders = ""; }
};

#endif