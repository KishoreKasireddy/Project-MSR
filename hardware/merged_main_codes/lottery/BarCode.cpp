#include "BarCode.h"

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Firebase settings
#define FIREBASE_HOST "https://lottery-ticket-scanner-f9948-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "lottery-ticket-scanner-f9948"

// Firebase Data object
FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig firebaseConfig;

SoftwareSerial barcodeScanner(4, 5); // RX, TX (D2 -> GPIO4, D1 -> GPIO5)

void setupBarCode() {
    Serial.begin(115200);           // Start the Serial communication with the PC
    barcodeScanner.begin(115200);   // Start the Serial communication with the barcode scanner
    Serial.println("GM65 Barcode Scanner Interface with ESP8266 at 115200 baud");

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());

    // Configure Firebase
    firebaseConfig.host = FIREBASE_HOST;
    firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

    Firebase.begin(&firebaseConfig, &firebaseAuth);
    Firebase.reconnectWiFi(true);
}

void loopBarCode() {
    if (barcodeScanner.available()) {
        String barcodeData = "";
        while (barcodeScanner.available()) {
            char c = barcodeScanner.read();
            barcodeData += c;
        }
        Serial.print("Scanned Barcode: ");
        Serial.println(barcodeData);

        // Upload to Firebase
        if (Firebase.push(firebaseData, "/barcodeData", barcodeData)) {
            Serial.println("Data uploaded successfully.");
        } else {
            Serial.print("Error uploading data: ");
            Serial.println(firebaseData.errorReason());
        }
    }
}
