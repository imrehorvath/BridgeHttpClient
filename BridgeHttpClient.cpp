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
  p.runShellCommand("head -n 1 " + tempFileName + " | cut -d' ' -f2");
  return p.parseInt();
}

String BridgeHttpClient::getResponseHeaders() {
  String responseHeaders;

  Process p;
  p.runShellCommand("tail -n +2 " + tempFileName);

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

int BridgeHttpClient::addHeader(const char *header) {
  if (headerIndex < HEADERCNT) {
    headers[headerIndex++] = header;
    return 0;
  }
  return -1;
}

void BridgeHttpClient::basicAuth(const char *user, const char *passwd) {
  this->user = user;
  this->passwd = passwd;
}

String BridgeHttpClient::getResponseHeaderValue(const String& header) {
  if (cachedRespHeaders.length() == 0) {
    cachedRespHeaders = getResponseHeaders();
  }
  int startOfValue = cachedRespHeaders.indexOf(':', cachedRespHeaders.indexOf(header)) + 1;
  String respValue = cachedRespHeaders.substring(startOfValue,
                                                 cachedRespHeaders.indexOf('\n', startOfValue));
  respValue.trim();
  return respValue;
}

void BridgeHttpClient::request(const char *verb, const char *url, const char *data, bool async) {
  Process p;
  p.runShellCommand("mktemp");
  tempFileName = p.readStringUntil('\n');

  clearCachedRespHeaders();

  begin("curl");
  addParameter("-D");
  addParameter(tempFileName);

  if (verb != "GET") {
    addParameter("-X");
    addParameter(verb);
  }

  if (isInsecureEnabled) {
    addParameter("-k");
  }

  if (user && passwd) {
    String auth;
    auth += user;
    auth += ":";
    auth += passwd;

    addParameter("-u");
    addParameter(auth);
  }

  for (int i = 0; i < headerIndex; i++) {
    addParameter("-H");
    addParameter(headers[i]);
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
