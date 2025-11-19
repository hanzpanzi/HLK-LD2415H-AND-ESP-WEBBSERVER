# HLK-LD2415H-AND-ESP-WEBBSERVER
Webserver ESP8266 and HLK-LD2415H
HLK-LD2415H-radarsensorn kan integreras med en ESP8266-mikrokontroller genom att använda dess TTL seriella (UART) gränssnitt. Integrationen kräver korrekt inkoppling och programvara (t.ex. Arduino-kod eller ESPHome) som kan kommunicera via UART med en fast baudhastighet på 9600 bps. 
Viktiga aspekter för integrationen
Kommunikationsprotokoll: Sensorn använder seriell kommunikation (UART) för att skicka data, primärt för hastighetsmätning av fordon (1 till 240 km/h).
Baudhastighet: Den fasta baudhastigheten är 9600 bps.
Spänningskompatibilitet: HLK-LD2415H drivs med 9-24V, medan ESP8266 arbetar med 3.3V. En spänningsomvandlare eller nivåskiftare (logic level shifter) kan behövas för att säkerställa kompatibilitet mellan sensorns logiknivåer och ESP8266:s GPIO-pinnar. 
Inkoppling (Föreslaget)
Följande anslutningar krävs mellan HLK-LD2415H och ESP8266:
HLK-LD2415H Pin 	ESP8266 Pin (t.ex. NodeMCU)	Anmärkningar
VCC	Extern strömkälla (9-24V)	Kräver egen strömförsörjning
GND	GND (på både extern källa och ESP8266)	Gemensam jord är nödvändig
För att integrera HLK-LD2415H-radarsensorn med en ESP8266 enligt din beskrivning och källkoden ovan gäller följande:

## Integration – Kopplingsschema och Kommentarer

| HLK-LD2415H Pin | ESP8266 Pin (t.ex. NodeMCU) | Anmärkning                                          |
|------------------|-----------------------------|-----------------------------------------------------|
| VCC              | Extern strömkälla (9-24V)   | Sensorn drivs EJ av ESP8266 – separat spänningskälla|
| GND              | GND på extern källa    & ESP8266 | **Gemensam jord krävs**           |
| TX               | RADAR_RX_PIN (ex. D6 / GPIO12) | **Via nivåskiftare!** HLK-LD2415H > ESP8266 RX      |
| RX               | RADAR_TX_PIN (ex. D7 / GPIO13) | **Via nivåskiftare!** ESP8266 TX > HLK-LD2415H RX   |

> **Viktigt!** Logic Level Shifter rekommenderas, då HLK-LD2415H kan skicka 5V signaler och ESP8266-toleransen är 3.3V.

## Programvaruimplementation

- Programmet (Arduino/ESP8266) använder `SoftwareSerial` för att skapa en extra UART-port så att ESP8266 kan kommunicera direkt med HLK-LD2415H.
- Baudrat/hastighet är konfigurerad till **9600** (`radarSerial.begin(9600);`).
- Inkommande data från sensorn läses in, och eventuella hastighetsvärden (`V+<värde>`/`V-<värde>`) skickas till en webbsida över WiFi (webserver på port 80).
- Kodexemplet hanterar och skriver ut värden på en enkel webbsida.

## Styrning och tolkning av data

- Sensorn skickar hastighetsdata som ett 9-byte ASCII-paket, t.ex. `"V+0012\r\n"`, där "`V`" identifierar paketet, tecknen efter förmedlar hastighet och tecken ("+" eller "-") anger riktning.
- Kodexemplet tolkar och visar detta på den webbsidan och Seriell Monitor.

## Praktiska tips

- Se till att GND verkligen är gemensam mellan sensor, ESP8266 och strömkälla.
- Logiknivåskiftare är avgörande så att ESP8266 inte skadas av 5V-signal från radaren.
