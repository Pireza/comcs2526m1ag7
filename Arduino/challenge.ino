#include <WiFi.h>
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
#define LED_BUILTIN 2     // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED
char Incoming_value = 0;  //Variable for storing Incoming_value

const char* ssid = "labs";
const char* pwd = "782edcwq#";

IPAddress server(192, 168, 1, 117);
WiFiClient client;


void setup() {
  delay(5000);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("Connecting to the WiFi network...");
  WiFi.begin(ssid, pwd);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (client.connect(server, 2000)) {

    Serial.println("connected");

  } else {
    Serial.println("failed connection...");
    for(;;){} // block
  }

  if (!SerialBT.begin("comcs2526m1ag7")) {  //change 99 to your group´s id
    Serial.println("An error occurred initializing Bluetooth");
  } else {
    Serial.println("Bluetooth initialized");
  }
  Serial.println("Bluetooth Started! Ready to pair...");  //Sets the data rate in
}
void loop() {
  static char cmd[256];
  static int i = 0;

  while (SerialBT.available()) {
    char c = SerialBT.read();

    // Filter out carriage returns and other non-printable chars
    if (c == '\r') continue;

    // End of command
    if (c == '\n') {
      cmd[i] = '\0';  // null-terminate string

      Serial.print("The command was: ");
      Serial.println(cmd);

      if (strcmp(cmd, "on") == 0) {
        SerialBT.println("The LED has been turned on!");
        client.println("The LED has been turned on!");
        digitalWrite(LED_BUILTIN, HIGH);
      } else if (strcmp(cmd, "off") == 0) {
        SerialBT.println("The LED has been turned off!");
        client.println("The LED has been turned off!");
        digitalWrite(LED_BUILTIN, LOW);
      } else {
        Serial.println("Unknown command...");
      }

      // Reset buffer for next command
      i = 0;
      memset(cmd, 0, sizeof(cmd));
    } else if (i < sizeof(cmd) - 1) {
      cmd[i++] = c;
    }
  }
}
