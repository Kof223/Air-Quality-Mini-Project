#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_SHT31.h>
#include "CJMCU8128.h"

// Wifi Setup
const char *ssid = "5H5F2";        //Enter your WIFI ssid
const char *password = "28749510"; //Enter your WIFI password
ESP8266WebServer server(80);

// Static IP
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

// Sensors
CJMCU8128 cjmcu;
Adafruit_SHT31 sht;

// Prepares the html page with the data from the sensor
String prepareHtmlPage()
{
    String htmlPage =
        String("<!DOCTYPE html>") +
        "<html>" +
        "<head>" +
            "<style>" +
                "body {background-color: powderblue; font-family: Arial; font-size: 4vw}" +
            "</style>" +
            "<title>Sensor Data</title>" +
        "</head>" +
        "<body>" +
            "<h1>Sensor Data</h1>" +
            "<p>Temperature (" + char(176) + "C): " + sht.readTemperature() + "</p>" +
            "<p>Humidity (%): " + sht.readHumidity() + "</p>" +
            "<p>Air Pressure (hpa): " + cjmcu.getPressure() + "</p>" +
            "<p>eCO2 (ppm): " + cjmcu.getCO2() + "</p>" +
            "<p>TVOC (ppb): " + cjmcu.getTVOC() + "</p>" +
            "<form action=\"/\" method=\"Refresh\">" +
                "<input type=\"submit\" value=\"Refresh\" style=\"height:200px; width:100%; font-size:100%\">" +
            "</form>" +
        "</body>" +
        "</html>";
    return htmlPage;
}

// Only one page for the html page
void handleRoot()
{
    cjmcu.update();
    server.send(200, "text/html", prepareHtmlPage());
}

// Setup html page
void serverSetup()
{
    if (!WiFi.config(local_IP, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }
    Serial.print("Configuring access point...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Wire.begin(); // Start I2C
    cjmcu.begin(); // Start CJMCU8128
    sht.begin(0x44); // Start SHT31
    serverSetup();
}

void loop()
{
    server.handleClient();
}
