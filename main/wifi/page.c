// page.c
#include "page.h"

const char *html_welcome = 
    "<html>"
    "<head>"
    "<title>Welcome to Smart Indicator</title>"
    "<style>"
    "body {"
    "    font-family: Arial, sans-serif;"
    "    background-color: #f2f2f2;"
    "    text-align: center;"
    "}"
    "h1 {"
    "    color: #333;"
    "}"
    "form {"
    "    background-color: #fff;"
    "    border-radius: 10px;"
    "    padding: 20px;"
    "    display: inline-block;"
    "    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);"
    "}"
    "input {"
    "    width: 100%;"
    "    padding: 10px;"
    "    margin: 5px 0;"
    "    border: 1px solid #ccc;"
    "    border-radius: 3px;"
    "}"
    "input[type='text'], input[type='password'] {"
    "    display: block;"
    "    width: 80%;"
    "    margin: 0 auto;"
    "}"
    "input[type='submit'] {"
    "    background-color: #4CAF50;"
    "    color: white;"
    "    border: none;"
    "    border-radius: 5px;"
    "    padding: 10px 20px;"
    "    cursor: pointer;"
    "}"
    "input[type='submit']:hover {"
    "    background-color: #45a049;"
    "}"
    "</style>"
    "</head>"
    "<body>"
    "<h1>Welcome to Smart Indicator</h1>"
    "<form method='post' action='/connect'>"
    "<input type='text' name='ssid' placeholder='Wi-Fi SSID' required><br>"
    "<input type='password' name='password' placeholder='Wi-Fi Password' required><br>"
    "<input type='submit' value='Connect'>"
    "</form>"
    "</body>"
    "</html>";