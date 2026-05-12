#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h> // Include SoftwareSerial library

const char* ssid = "essid"; (((chnage this your wifi)))
const char* password = "password"; (((chnage this your passw)))

ESP8266WebServer server(80);

// Global variable to store the latest speed value as a String
String detectedSpeed = "N/A";

// Define SoftwareSerial pins for the radar
// ESP8266 RX pin must be a valid GPIO (e.g., GPIO12, GPIO13)
// Connect HLK-LD2415H TX pin to ESP8266 RX_PIN (GPIO 12 here)
// Connect HLK-LD2415H RX pin to ESP8266 TX_PIN (GPIO 13 here)
// Make sure you use a Logic Level Shifter if the radar output is 5V.
#define RADAR_RX_PIN D6 // GPIO 12
#define RADAR_TX_PIN D7 // GPIO 13

SoftwareSerial radarSerial(RADAR_RX_PIN, RADAR_TX_PIN); // RX, TX

void handleRoot() {
  String htmlResponse = "<html><head><meta http-equiv='refresh' content='2'><title>LD2415H Radar Data</title></head><body>";
  htmlResponse += "<h1>Hastighet (LD2415H)</h1>";
  htmlResponse += "<p>Latest Detected Speed: <strong>" + detectedSpeed + "</strong></p>";
  htmlResponse += "</body></html>";
  server.send(200, "text/html", htmlResponse);
}

void setup() {
  Serial.begin(115200); // USB Serial Monitor for debugging
  delay(100);
  Serial.println("\nSerial Monitor Initialized at 115200 baud.");

  // Initialize Software Serial for LD2415H sensor at 9600 baud
  radarSerial.begin(9600);
  Serial.println("Radar Serial Initialized at 9600 baud.");


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  readAndParseRadarData(); 
}

// Function to read and parse the 9-byte ASCII data frame from the LD2415H sensor
void readAndParseRadarData() {
  // Use a static char array to build the incoming frame
  static char speedBuffer[10]; // Max 9 bytes + null terminator
  static uint8_t bufferIndex = 0;

  while (radarSerial.available() > 0) { // Check the radar serial port
    char incomingChar = radarSerial.read(); // Read from the radar port
    Serial.write(incomingChar); // Optional: Forward data to USB Serial monitor for debugging

    // Check for the end of the frame (Newline character 0x0A)
    if (incomingChar == '\n') {
      // We received the end marker. Null-terminate the buffer to make it a valid C string.
      speedBuffer[bufferIndex] = '\0'; 

      // Process the complete frame
      // Check if the frame starts with 'V' and '+' or '-'
      if (speedBuffer[0] == 'V' && (speedBuffer[1] == '+' || speedBuffer[1] == '-')) {
        // The speed data starts from the second character (index 1)
        detectedSpeed = String(speedBuffer).substring(1); 
        detectedSpeed += " KM/H";
        Serial.println("\nParsed Speed: " + detectedSpeed); // Debug print
      }
      
      // Reset the buffer index for the next frame
      bufferIndex = 0; 
      
    } else if (incomingChar != '\r') { 
      // Ignore the Carriage Return (0x0D) and add other characters to the buffer
      if (bufferIndex < sizeof(speedBuffer) - 1) {
        speedBuffer[bufferIndex++] = incomingChar;
      }
    }
  }
}
