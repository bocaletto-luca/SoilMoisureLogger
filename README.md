# Soil Moisture Logger with RTC & SD Card
#### Author: Bocaletto Luca

**Files**  
- `README.md`  
- `SoilMoistureLogger.ino`

## 1. Concept  
An autonomous data‐logger that measures soil moisture with a capacitive sensor at regular intervals (and on‐demand via a push‐button), timestamps each reading using a DS3231 RTC, and appends entries to a CSV file on a microSD card. Perfect for greenhouse monitoring or unattended field deployments.

## 2. Bill of Materials  
- Arduino Uno (or Nano)  
- Capacitive soil moisture sensor (e.g. Chirp or SEN0193)  
- DS3231 RTC module  
- Micro SD card module + 4 GB (or larger) microSD card  
- Push‐button + 10 kΩ pull‐down resistor  
- Status LED (optional) + 220 Ω resistor  
- Breadboard & jumper wires  
- 5 V power supply (USB or battery pack)

## 3. Wiring Diagram

    Capacitive Sensor        Arduino Uno
    ┌─────────────┐          ┌─────────┐
    │ VCC ───► 5V │          │         │
    │ GND ───► GND│          │         │
    │ AOUT ──► A0 │          │         │
    └─────────────┘          └─────────┘

    DS3231 RTC (I2C)
    ┌─────────────┐
    │ SDA ───► A4 │
    │ SCL ───► A5 │
    │ VCC ───► 5V │
    │ GND ───► GND│
    └─────────────┘

    SD Card Module (SPI)
    ┌─────────────┐          ┌──────────┐
    │ CS  ───► D10│          │         │
    │ MOSI ─► D11│          │         │
    │ MISO ─► D12│          │         │
    │ SCK  ─► D13│          │         │
    │ VCC ───► 5V │          │         │
    │ GND ───► GND│          │         │
    └─────────────┘          └──────────┘

    Push‐button             Arduino Uno
    ┌─────────┐             ┌─────────┐
    │ one leg─┬─► D2        │         │
    │ other ──┴─► GND       │         │
    └─────────┘             └─────────┘
    (Use 10 kΩ from D2 to GND for pull‐down)

    Optional LED
    ┌─────────┐             ┌─────────┐
    │ Anode ─┬─► D8         │         │
    │ Cath. ─┴─► GND        │         │
    └─────────┘             └─────────┘
    (Include 220 Ω resistor in series)

## 4. Software Setup  
1. Install Arduino IDE (≥1.8.x).  
2. In Library Manager, install:
   - **RTClib** by Adafruit  
   - **SD** (built-in)  
3. Create folder `SoilMoistureLogger` and save both files.  
4. Open `SoilMoistureLogger.ino` in the IDE.
5. The logger writes `soil_log.csv` on the SD root with columns: timestamp,moisture 2025-06-20 14:00:00,412
6. Automatic readings every 10 minutes; press the button to force a manual reading.  
7. LED blinks once on each log.

---

Say **continua** when you’re ready for project #9!  

6. Select **Arduino Uno** and the correct COM port.

## 5. Usage  
1. Format your SD card as FAT32.  
2. Ensure `RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));` is commented out after initial time‐set.  
3. Upload sketch.  
4. Open Serial Monitor at 9600 baud to follow status messages.  
5. The logger writes `soil_log.csv` on the SD root with columns:
