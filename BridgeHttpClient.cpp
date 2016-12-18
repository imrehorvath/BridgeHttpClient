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

#include "BridgeHttpClient.h"

void BridgeHttpClient::get(const char *url) {
  request("GET", url, NULL);
}

void BridgeHttpClient::post(const char *url, const char *data) {
  request("POST", url, data);
}

void BridgeHttpClient::put(const char *url, const char *data) {
  request("PUT", url, data);
}

void BridgeHttpClient::del(const char *url) {
  request("DELETE", url, NULL);
}

unsigned int BridgeHttpClient::getResponseCode() {
  Process p;
  p.runShellCommand("head -n 1 " + _tempFileName + " | cut -d' ' -f2");
  return p.parseInt();
}

String BridgeHttpClient::getResponseHeaders() {
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

void BridgeHttpClient::getAsync(const char *url) {
  request("GET", url, NULL, true);
}

void BridgeHttpClient::postAsync(const char *url, const char *data) {
  request("POST", url, data, true);
}

void BridgeHttpClient::putAsync(const char *url, const char *data) {
  request("PUT", url, data, true);
}

void BridgeHttpClient::delAsync(const char *url) {
  request("DELETE", url, NULL, true);
}

void BridgeHttpClient::enableInsecure() {
  _isInsecureEnabled = true;
}

int BridgeHttpClient::addHeader(const char *header) {
  if (_extraHeaderIdx < HEADERCNT) {
    _extraHeaders[_extraHeaderIdx++] = header;
    return 0;
  }
  return -1;
}

void BridgeHttpClient::basicAuth(const char *user, const char *passwd) {
  _user = user;
  _passwd = passwd;
}

void BridgeHttpClient::clearHeaders() {
  _extraHeaderIdx = 0;
}

void BridgeHttpClient::clearAuth() {
  _user = _passwd = NULL;
}

String BridgeHttpClient::getResponseHeaderValue(const String& header) {
  if (_cachedRespHeaders.length() == 0) {
    _cachedRespHeaders = getResponseHeaders();
  }
  int startOfValue = _cachedRespHeaders.indexOf(':', _cachedRespHeaders.indexOf(header)) + 1;
  String respValue = _cachedRespHeaders.substring(startOfValue,
                                                 _cachedRespHeaders.indexOf('\n', startOfValue));
  respValue.trim();
  return respValue;
}

void BridgeHttpClient::request(const char *verb, const char *url, const char *data, bool async) {
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

void BridgeHttpClient::clearCachedRespHeaders() {
  _cachedRespHeaders = "";
}
