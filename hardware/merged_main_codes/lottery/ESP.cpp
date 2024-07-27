#include "ESP.h"

void setupESP() {
    Serial.begin(115200);
    EEPROM.begin(512);

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.on("/", handleRoot);
    server.on("/config", handleWiFiConfig);
    server.on("/reset", handleReset);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loopESP() {
    server.handleClient();
}

void handleRoot() {
    String html = "<html><body><h1>ESP8266 Web Server</h1></body></html>";
    server.send(200, "text/html", html);
}

void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void handleWiFiConfig() {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    String content;
    int statusCode;

    if (ssid.length() > 0 && pass.length() > 0) {
        for (int i = 0; i < 96; ++i) EEPROM.write(i, 0);
        for (int i = 0; i < ssid.length(); ++i) EEPROM.write(i, ssid[i]);
        for (int i = 0; i < pass.length(); ++i) EEPROM.write(32 + i, pass[i]);
        EEPROM.commit();

        content = "{\"Success\":\"Saved to EEPROM. Connecting...\"}";
        statusCode = 200;
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", content);

        connectToWifi(ssid.c_str(), pass.c_str());

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Successfully connected to new network!");
        } else {
            Serial.println("Failed to connect to new network.");
        }
    } else {
        content = "{\"Error\":\"Invalid credentials provided\"}";
        statusCode = 400;
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", content);
    }
}

void handleReset() {
    String content = "{\"Success\":\"Resetting...\"}";
    int statusCode = 200;
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
    delay(500);
    ESP.restart();  // Make sure ESP.restart() is correctly included
}

void connectToWifi(const char* ssid, const char* pass) {
    WiFi.begin(ssid, pass);
    Serial.print("Connecting to ");
    Serial.println(ssid);

    int retry_count = 0;
    while (WiFi.status() != WL_CONNECTED && retry_count < 20) {
        delay(500);
        Serial.print(".");
        retry_count++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        Serial.print("Local IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi");
    }
}
