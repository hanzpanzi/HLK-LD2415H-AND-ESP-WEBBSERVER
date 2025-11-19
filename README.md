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
