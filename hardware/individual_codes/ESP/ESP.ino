#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Constants
const char* myDefaultSSID = "Default_SSID";
const char* myDefaultPassphrase = "Default_Password";

// Server Setup
ESP8266WebServer server(80);

// Global Variables
String st;
String content;
int statusCode;

// Function Prototypes
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
void createWebServer(void);
void handleRoot(void);
void handleScan(void);
void handleSetting(void);
void handleReset(void);
void connectToWifi(const char* ssid, const char* pass);

// Setup Function
void setup() {
    Serial.begin(115200);
    Serial.println();
    WiFi.disconnect();
    EEPROM.begin(512);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Startup");

    // Read EEPROM for SSID and password
    String esid = "", epass = "";
    for (int i = 0; i < 32; ++i) esid += char(EEPROM.read(i));
    for (int i = 32; i < 96; ++i) epass += char(EEPROM.read(i));
    Serial.print("SSID: "); Serial.println(esid);
    Serial.print("PASS: "); Serial.println(epass);

    if (esid.length() > 0 && epass.length() > 0) {
        connectToWifi(esid.c_str(), epass.c_str());
    } else {
        connectToWifi(myDefaultSSID, myDefaultPassphrase);
    }

    if (!testWifi()) {
        Serial.println("Turning the HotSpot On");
        launchWeb();
        setupAP();
    }

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        server.handleClient();
    }
}

// Loop Function
void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        for (int i = 0; i < 10; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(1000);
            digitalWrite(LED_BUILTIN, LOW);
            delay(1000);
        }
    }
}

// WiFi Test Function
bool testWifi(void) {
    int attempts = 20;
    Serial.println("Waiting for Wifi to connect");
    while (attempts-- > 0) {
        if (WiFi.status() == WL_CONNECTED) return true;
        delay(500);
        Serial.print("*");
    }
    Serial.println("\nConnect timed out, opening AP");
    return false;
}

// Web Server Launch Function
void launchWeb() {
    if (WiFi.status() == WL_CONNECTED) Serial.println("WiFi connected");
    Serial.print("Local IP: "); Serial.println(WiFi.localIP());
    Serial.print("SoftAP IP: "); Serial.println(WiFi.softAPIP());
    createWebServer();
    server.begin();
    Serial.println("Server started");
}

// Access Point Setup Function
void setupAP() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks();
    Serial.println(n == 0 ? "no networks found" : String(n) + " networks found");
    for (int i = 0; i < n; ++i) {
        Serial.println(String(i + 1) + ": " + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ")" + (WiFi.encryptionType(i) == ENC_TYPE_NONE ? " " : "*"));
        delay(10);
    }

    st = "<ol>";
    for (int i = 0; i < n; ++i) {
        st += "<li>" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ")" + (WiFi.encryptionType(i) == ENC_TYPE_NONE ? " " : "*") + "</li>";
    }
    st += "</ol>";

    WiFi.softAP("ESP8266", "");
    Serial.println("Initializing softAP for WiFi credentials modification");
    launchWeb();
}

// Web Server Creation Function
void createWebServer() {
    server.on("/", handleRoot);
    server.on("/scan", handleScan);
    server.on("/setting", handleSetting);
    server.on("/reset", handleReset);  // Handle reset button action
}

// Handle Root URL
void handleRoot() {
    IPAddress ip = WiFi.softAPIP();
    content = "<!DOCTYPE HTML><html><head><title>ESP8266 Wi-Fi Config</title><style>"
              "body { font-family: Arial, sans-serif; background-color: #f2f2f2; }"
              ".container { width: 80%; margin: 40px auto; background-color: #fff; padding: 20px; border: 1px solid #ddd; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }"
              ".form-group { margin-bottom: 20px; }"
              ".form-group label { display: block; margin-bottom: 10px; }"
              ".form-group input[type='text'], .form-group input[type='password'] { width: 100%; padding: 10px; margin-bottom: 20px; border: 1px solid #ccc; }"
              ".btn { background-color: #4CAF50; color: #fff; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; }"
              ".btn:hover { background-color: #3e8e41; }"
              "</style></head><body><div class='container'><h2>ESP8266 Wi-Fi Config</h2>"
              "<form method='get' action='setting'><div class='form-group'><label for='ssid'>SSID:</label>"
              "<input type='text' name='ssid' length='32'></div><div class='form-group'><label for='pass'>Password:</label>"
              "<input type='password' name='pass' length='64'></div><button class='btn'>Submit</button></form>"
              "<form method='get' action='reset'><button class='btn' style='margin-top:20px;'>Connect to Another Network</button></form></div>"
              "<p>Local IP: " + ip.toString() + "</p><p>" + st + "</p></body></html>";
    server.send(200, "text/html", content);
}

// Handle Scan URL
void handleScan() {
    content = "<!DOCTYPE HTML><html><body><h1>Scan Complete</h1><a href='/'>Go Back</a></body></html>";
    server.send(200, "text/html", content);
}

// Handle Setting URL
void handleSetting() {
    String qsid = server.arg("ssid");
    String qpass = server.arg("pass");
    if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("Received new credentials");
        Serial.print("SSID: "); Serial.println(qsid);
        Serial.print("Password: "); Serial.println(qpass);

        for (int i = 0; i < 96; ++i) EEPROM.write(i, 0);
        for (int i = 0; i < qsid.length(); ++i) EEPROM.write(i, qsid[i]);
        for (int i = 0; i < qpass.length(); ++i) EEPROM.write(32 + i, qpass[i]);
        EEPROM.commit();

        content = "{\"Success\":\"Saved to EEPROM. Connecting...\"}";
        statusCode = 200;
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(statusCode, "application/json", content);

        // Connect to the new Wi-Fi network
        connectToWifi(qsid.c_str(), qpass.c_str());

        // Check connection status and update the user
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

// Handle Reset URL
void handleReset() {
    content = "{\"Success\":\"Resetting...\"}";
    statusCode = 200;
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
    delay(500);
    ESP.restart();
}

// Connect to WiFi Function
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