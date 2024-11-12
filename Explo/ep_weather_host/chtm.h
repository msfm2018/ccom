#ifndef _H_EP_WEATHER_PROVIDER_GOOGLE_HTML_H_
#define _H_EP_WEATHER_PROVIDER_GOOGLE_HTML_H_
#include <Windows.h>

wchar_t hstr1[] = L"<!DOCTYPE html>\n"
L"<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n"
L"<head>\n"
L"<meta charset=\"utf-8\" />\n"
L"<title>Weather</title>\n"
L"<style>\n"
L"html {\n"
L"  background-color: transparent !important;\n"
L"}\n"
L".google-weather-place {\n"
L"    width: calc(655px); height: calc(370px);\n"
L"}\n"
L".google-weather-crop {\n"
L"    width: calc(655px); height: calc(370px);\n"
L"    overflow: hidden;\n"
L"    transition: 0.3s;\n"
L"    position: absolute;\n"
L"}\n"
L".google-weather {\n"
L"    position: relative;\n"
L"    left: -180px; top: -180px;\n"
L"    width: 2560px; height: 5120px;\n"
L"    transform: scale(1.0);\n"
L"    transform-origin: 180px 180px;\n"
L"    transition: 0.3s;\n"
L"    position: absolute;\n"
L"}\n"
L".google-weather:hover {\n"
L"    transform: scale(1);\n"
L"    z-index: 1000;\n"
L"}\n"
L"</style>\n"
L"</head>\n"
L"<body>\n"
L"<div class=\"google-weather-place\">\n"
L"<div class=\"google-weather-crop\">\n"
L"<iframe id=\"frame\" allowtransparency=\"true\" class=\"google-weather\" src=\"https://www.google.com/search?igu=1&amp;hl=%s&amp;q=weather%s%s\">\n"
L"</iframe>\n"
L"</div>\n"
L"</div>\n"
L"</body>\n"
L"</html>";
//   wchar_t hstr[1024]; // 或者根据实际大小调整数组长度
//    swprintf(hstr, sizeof(hstr) / sizeof(wchar_t), L"<!DOCTYPE html>\n<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n<head>\n<meta charset=\"utf-8\" />\n<title>Weather</title>\n<style>html { background-color: transparent !important; } .google-weather-place { width: calc(655px); height: calc(370px); } .google-weather-crop { width: calc(655px); height: calc(370px); overflow: hidden; transition: 0.3s; position: absolute; } .google-weather { position: relative; left: -180px; top: -180px; width: 2560px; height: 5120px; transform: scale(1.0); transform-origin: 180px 180px; transition: 0.3s; position: absolute; } .google-weather:hover { transform: scale(1); z-index: 1000; }</style></head><body><div class=\"google-weather-place\"><div class=\"google-weather-crop\"><iframe id=\"frame\" allowtransparency=\"true\" class=\"google-weather\" src=\"https://www.google.com/search?igu=1&amp;hl=%s&amp;q=weather%s%s\"></iframe></div></div></body></html>", L"en", L"city", L"country");



#endif