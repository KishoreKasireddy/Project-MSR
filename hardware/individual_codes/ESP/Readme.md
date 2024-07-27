
# ESP8266 Wi-Fi Configurator

This project provides a simple way to configure the Wi-Fi settings of an ESP8266 microcontroller through a web interface. You can connect to the ESP8266 using its built-in access point and configure the SSID and password of the Wi-Fi network you want it to connect to.

## Features

- Connects to a predefined default Wi-Fi network if no credentials are stored.
- Provides a web interface to configure Wi-Fi credentials.
- Displays available Wi-Fi networks.
- Allows resetting the device to connect to another network.
- Saves credentials in EEPROM for persistence.

## How It Works

If the ESP8266 cannot connect to a previously saved Wi-Fi network, it will start its own access point (AP). You can connect to this AP, navigate to a web page served by the ESP8266, and enter new Wi-Fi credentials. The ESP8266 will then attempt to connect to the new network and save these credentials for future use.

## Usage Instructions

1. **Power on the ESP8266**:
    - The ESP8266 will attempt to connect to a previously saved Wi-Fi network.
    - If it fails, it will start its own access point.

2. **Connect to the ESP8266's Access Point**:
    - Use your mobile device to connect to the ESP8266's AP named "ESP8266".

3. **Configure Wi-Fi Settings**:
    - Open a web browser and navigate to [http://192.168.4.1/](http://192.168.4.1/).
    - The page will display available Wi-Fi networks.
    - Enter the SSID and password of the network you want the ESP8266 to connect to and submit the form.

4. **Connect to the Network**:
    - The ESP8266 will attempt to connect to the new network.
    - If successful, it will save the credentials in EEPROM and use them for future connections.

## Troubleshooting

- If the ESP8266 fails to connect to the specified Wi-Fi network, ensure that the credentials are correct and that the network is within range.
- Check the serial monitor for debug messages that provide information about the connection status.

## Contributing

Feel free to fork this repository and submit pull requests. Contributions are welcome!


---

By following the above instructions, you should be able to set up and configure the Wi-Fi settings for your ESP8266 microcontroller easily.
