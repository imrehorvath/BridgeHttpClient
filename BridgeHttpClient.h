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
    // The library supports up to 16 user addable request headers
    static const size_t EXTRA_HEADERS_MAX = 16;

    BridgeHttpClient() : _isInsecureEnabled(false),
                         _user(NULL), _passwd(NULL),
                         _extraHeaderIdx(0) {}

    /*
     * Synchronous methods
     */
    void get(const char *url);
    void post(const char *url, const char *data);
    void put(const char *url, const char *data);
    void del(const char *url);

    /*
     * Asynchronous methods
     */
    void getAsync(const char *url);
    void postAsync(const char *url, const char *data);
    void putAsync(const char *url, const char *data);
    void delAsync(const char *url);

    /*
     * Method to check, if the async request is still running or has finished.
     */
    bool finished();

    /*
     * Call this method after the request has finished,
     * to get the HTTP response code from the server.
     *
     * Returns the response code eg. 200
     */
    unsigned int getResponseCode();

    /*
     * Call this method after the request has finished,
     * to get the response headers received from the server.
     *
     * Returns a String object with all the response headers included.
     */
    String getResponseHeaders();

    /*
     * Call this method before issuing the requests,
     * to allows "insecure" SSL.
     *
     * Useful in the following case for example:
     * Certificate cannot be authenticated with known CA certificates.
     */
    void enableInsecure();

    /*
     * Call this method before issuing the request,
     * to add an extra header to the request.
     *
     * Returns 0 if the header fits into the array of headers. -1 otherwise.
     */
    int addHeader(const char *header);

    /*
     * Call this method before issuing the request,
     * to include basic authorization into the request.
     */
    void basicAuth(const char *user, const char *passwd);

    /*
     * Call this method between the different request calls on the same client,
     * to clear/setup the request headers for the next call.
     */
    void clearHeaders();

    /*
     * Call this method between the different request calls on the same client,
     * to clear the previously set basic authorization for the subsequent call.
     */
    void clearAuth();

    /*
     * Call this method after the request has finished,
     * to get a particular response header value.
     *
     * Returns true and sets the value parameter if found,
     * otherwise return false and doesn't set value parameter at all.
     */
    bool getResponseHeaderValue(const String& header, String& value);

    /*
     * Reset client request settings.
     * Clears the request headers, basic auth. settings and the insecure-enabled flag.
     */
    void startRequest();

  private:
    String _tempFileName;

    bool _isInsecureEnabled;

    const char *_user;
    const char *_passwd;

    const char *_extraHeaders[EXTRA_HEADERS_MAX];
    int _extraHeaderIdx;

    String _cachedRespHeaders;

    /*
     * Sends the actual request via cURL on the Linux side
     */
    void request(const char *verb, const char *url, const char *data, bool async=false);

    /*
     * Clears the cached response headers between subsequent requests
     */
    void clearCachedRespHeaders();
};

#endif
