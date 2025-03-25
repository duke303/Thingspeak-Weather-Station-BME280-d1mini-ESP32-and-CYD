D1mini ESP32 BME280 infos:

1. Libraries Used:
Wire.h: This library is for I2C communication, which is used to communicate with devices like the BME280 sensor.

WiFi.h: This library is used for connecting the ESP32 to a Wi-Fi network.

ThingSpeak.h: This library allows the ESP32 to send data to the ThingSpeak platform.

BME280I2C.h: This library is used to interact with the BME280 sensor, which measures temperature, humidity, and pressure over I2C.

2. Configuration Variables:
I2C_SDA & I2C_SCL: These variables define the I2C data and clock pins on the ESP32 (pins 21 and 22, respectively).

sleepInterval: Defines the interval for deep sleep in seconds (1800 seconds = 30 minutes).

WiFi credentials: Your Wi-Fi network name and password.

ThingSpeak credentials: The ThingSpeak server host, channel number, and API key for sending data.

BME280: An object for interacting with the BME280 sensor.

3. Functions:
connectWiFi(): This function connects the ESP32 to the Wi-Fi network. It keeps trying until it is connected, and then prints the IP address assigned to the ESP32.

4. Setup Function:
Serial.begin(115200): Starts serial communication for debugging.

Wire.begin(I2C_SDA, I2C_SCL): Initializes I2C communication on the specified SDA and SCL pins.

bme.begin(): Initializes the BME280 sensor. If there is an error, the program halts with an error message.

WiFi connection: Calls connectWiFi() to connect to the Wi-Fi network.

Reading Sensor Data: The code reads temperature, humidity, and pressure data from the BME280 sensor using the bme.read() function.

Sending Data to ThingSpeak:

The data (temperature, humidity, and pressure) is sent to ThingSpeak using the ThingSpeak.setField() and ThingSpeak.writeFields() functions.

Each field corresponds to one of the sensor values (temperature, humidity, and pressure).

Printing Data: The code prints the temperature, humidity, and pressure values to the serial monitor for debugging.

Deep Sleep: The ESP32 goes into deep sleep mode for sleepInterval seconds (30 minutes). During deep sleep, the ESP32 is inactive and consumes very little power, making it suitable for battery-powered projects.

5. Loop Function:
The loop() function is empty because the ESP32 will not run the loop during deep sleep. Once the device wakes up after the sleep interval, the process repeats from the beginning.

Key Concepts:
Wi-Fi and ThingSpeak: This code connects the ESP32 to a Wi-Fi network, reads sensor data, and sends it to ThingSpeak for monitoring and logging.

BME280 Sensor: The BME280 sensor is used for measuring environmental parameters such as temperature, humidity, and pressure.

Deep Sleep: The ESP32 goes into deep sleep mode to save power between data uploads, which is useful for IoT projects with long battery life requirements.

Summary:
This code connects the ESP32 to Wi-Fi, reads data from the BME280 sensor, sends the data to ThingSpeak, and then puts the ESP32 into deep sleep mode for 30 minutes before repeating the process. The loop function is not used as the ESP32 relies on deep sleep and wakes up after the set interval to repeat the process.








CYD infos:

1. Libraries Used:
TFT_eSPI.h: A library for driving TFT displays using the ESP32. It allows easy control over the display's properties, such as text size, colors, and positions.

WiFi.h: This library is used for connecting the ESP32 to a Wi-Fi network.

HTTPClient.h: This library allows the ESP32 to make HTTP requests, enabling it to fetch data from a web server (in this case, ThingSpeak).

ArduinoJson.h: A library for parsing and creating JSON objects. This is used to decode the response from ThingSpeak, which is in JSON format.

2. Configuration Variables:
WiFi credentials: These variables store the Wi-Fi network name (SSID) and password required to connect the ESP32 to the network.

ThingSpeak settings:

outdoorApiKey: The API key for accessing the outdoor channel on ThingSpeak.

outdoorChannelID: The channel ID for the outdoor data on ThingSpeak.

serverName: The URL of the ThingSpeak API, where data is fetched from.

TFT Display initialization: An instance of the TFT_eSPI library is created to control the display.

Timing:

thingSpeakInterval: This sets the interval (in milliseconds) for fetching data from ThingSpeak (1800000 ms = 30 minutes).

previousThingSpeakMillis: This variable is used to track when the last data fetch occurred, to trigger the next one after the set interval.

3. Functions:
setup():

Initializes serial communication for debugging.

Initializes the TFT display and displays a "Connecting to WiFi" message.

Connects to the Wi-Fi network and displays the connection status on the screen.

Immediately fetches and displays data from ThingSpeak after the Wi-Fi connection is established.

loop():

The loop checks if the set time interval (30 minutes) has passed and calls fetchAndUpdateData() to fetch the latest data from ThingSpeak.

fetchAndUpdateData():

This function checks if the device is connected to Wi-Fi. If connected, it constructs the ThingSpeak URL and fetches data using httpGETRequest().

The fetched data is then parsed and displayed on the TFT screen. If the Wi-Fi is disconnected, an error message is shown on the screen.

httpGETRequest():

This function makes an HTTP GET request to the specified URL (serverName) and retrieves the response as a string.

If the request is successful, it returns the payload (JSON data); otherwise, it prints an error message.

parseThingSpeakResponse():

This function parses the JSON data received from ThingSpeak using the ArduinoJson library.

It extracts the temperature, humidity, and pressure values from the response. If any value is missing, it defaults to "--".

updateDisplay():

This function updates the TFT display with the fetched data (temperature, humidity, and pressure).

It first clears areas of the screen to prepare for the new data and then displays the temperature, humidity, and pressure values in a readable format.

initializeDisplay():

Initializes the TFT display, sets the rotation, and clears the screen.

The display colors are set, and the display inversion is disabled for better readability.

4. Overall Functionality:
The code connects to a Wi-Fi network and periodically fetches environmental data (temperature, humidity, and pressure) from ThingSpeak.

It updates the display with the fetched data every 30 minutes and shows real-time updates on the TFT screen.

If the Wi-Fi is disconnected at any time, the display shows a "WiFi disconnected" message.

The data displayed includes:

Temperature (°C).

Humidity (%).

Pressure (hPa).

Key Concepts:
Wi-Fi and ThingSpeak Integration: The code connects to a Wi-Fi network and uses the ThingSpeak API to fetch environmental data for display.

TFT Display Update: The TFT display is used to show real-time sensor data, such as temperature, humidity, and pressure.

JSON Parsing: The ArduinoJson library is used to parse the JSON response from ThingSpeak to extract the sensor data.

Periodic Data Fetch: The code uses a timing mechanism (millis()) to ensure that data is fetched every 30 minutes without blocking the main loop.

Summary:
The code is designed for an ESP32 to periodically fetch environmental data (temperature, humidity, pressure) from a ThingSpeak channel, parse the data, and display it on a TFT screen. The data is fetched every 30 minutes, and the TFT display is updated with the latest values. If Wi-Fi is disconnected, an error message is shown on the display.
