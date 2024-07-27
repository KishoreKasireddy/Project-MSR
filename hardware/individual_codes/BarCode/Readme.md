
# GM65 Barcode Scanner with ESP8266 and Firebase

This project interfaces a GM65 barcode scanner with an ESP8266 microcontroller and uploads scanned barcode data to Firebase Realtime Database.

## Features

- Connects to Wi-Fi and Firebase.
- Reads data from the GM65 barcode scanner.
- Uploads scanned barcode data to Firebase.

## Hardware Connections

Ensure the barcode scanner is correctly connected:

- GM65 TX -> ESP8266 RX (GPIO4)
- GM65 RX -> ESP8266 TX (GPIO5)
- GM65 VCC -> ESP8266 3.3V
- GM65 GND -> ESP8266 GND

## Usage Instructions

1. **Set Up Wi-Fi Credentials**:
    - Replace `YOUR_SSID` and `YOUR_PASSWORD` in the code with your Wi-Fi credentials.

2. **Configure Firebase**:
    - Set the `FIREBASE_HOST` and `FIREBASE_AUTH` to your Firebase project's details.

3. **Upload and Run the Code**:
    - Upload the code to your ESP8266 microcontroller.
    - Open the serial monitor to check the status of the connection and scanned barcode data.

4. **Scan Barcodes**:
    - The barcode scanner will read and upload barcode data to Firebase automatically.

## Serial Monitor Output

- Displays connection status to Wi-Fi and Firebase.
- Shows scanned barcode data.
- Confirms successful data upload or shows error messages.

## User Manual for GM65 Barcode Scanner

Refer to the [GM65 Barcode Reader Module User Manual](https://uelectronics.com/wp-content/uploads/2022/03/GM65-Barcode-reader-mudule-User-Manual.pdf) for more information on using and configuring the barcode scanner.

## Contributing

Feel free to fork this repository and submit pull requests. Contributions are welcome!



---

By following the above instructions, you should be able to set up and interface the GM65 barcode scanner with your ESP8266 microcontroller and upload the scanned data to Firebase easily.
