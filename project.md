# envelope

## project description
I live on the first floor of my building and, approximately 50 meters away from my office's window is my mailbox. The mailbox is a closed metal box with a space on its front part for letters/envelopes to be inserted. When the mailman arrives, he pushes this opening, inserting the letter. The box's dimensions are:
- Internal compartment: 25cm x 40cm x 12cm
- Opening: 35cm x 3cm

The opening is in the middle of the box.

The objective of this project, called "envelope", is to build a smart detection system that sits inside the mailbox. When something is inserted in it, the system detects it and sends a signal to a receiver.

## available components
- 3x Lilygo T3S3 ESP32-S3 Dev Board LoRa Module (868 MHz)
- 30x circular magnet (10x3)
- 3x LCD1602 display module
- 5x TCRT5000 Infrared Reflective Sensor IR Photoelectric Switch Barrier
- 4x 18650 battery and holders
- 10x N/O reed switch 2*14mm
- 10x N/C reed switch 2*14mm
- 20x photoresistor GL5528
- 1x HC-SR04 ultrasonic sensor
- soldering equipment: soldering iron, soldering wire, copper welding desoldering wire, iron tip refresher, pcb boards, tip cleaners

## project design
### multi-sensor fusion
I can make the three sensors complement each other:
  - Reed switch on the flap: will detect when something is pushed through the slot. Detects the mechanical event directly, regardless of lighting conditions.
  - Photoresistor: detects the light spike when the slot opens.
  - ~~TCRT5000: detects physical presence of an object inside the box. Can be used as a confirmation that something actually landed, not just that the flap was touched.~~ This idea won't work properly because this sensor's optimal distance is 2.5mm. A better idea is to use an ultrasonic sensor.
  - HC-SR04: Will be strapped to the top of the box - pointing down - and will be responsible for determining the passage of a letter/envelope.

I can use the reed switch + photoresistor to determine that the flap has been opened and the HC-SR04 to determine that something has been inserted.

### eliminating false positives
A false positive can happen in three ways: when something disturbs one of the sensors, making it seem that the slot was opened/a letter was inserted; when someone opens the slot with their hand but nothing is inserted; and when we open the box with a key to collect anything that's inside. To avoid this, we can follow a time/threshold approach in which we classify events by two dimensions: duration and intensity. A brief, partial light event is mail arriving. A prolonged, full intensity event across all sensors simultaneously is the box being opened with a key.

### N/O x N/C, which type to use?
For a flap that rests closed and briefly opens:
  - N/C means the circuit breaks when all the flap opens, so we detect the absence of the magnet. This is the more reliable choice because a dead battery or broken wire looks the same as "no event", not a false trigger
  - N/O would trigger on any wire fault, which could cause false alerts

### cases
Each sensor acts independently and, based on its readings, will determine true/false for the event of a letter insertion. I also want to determine when the box has been opened with a key. The combinations + interpretations are the following:

**With explanation**

| Box reed switch | Flap reed switch | Photoresistor | Ultrasonic sensor | Meaning |
|---|---|---|---|---|
| false | false | false | false | Nothing was inserted (100%) |
| false | false | false | true | Only movement inside the box = faulty ultrasonic |
| false | false | true | false | Only light detected = faulty photoresistor |
| false | false | true | true | Light + movement but no reed switch = reed switch is faulty, letter was added (80%-90%) |
| false | true | false | false | Only flap switch = flap tapped, nothing was inserted |
| false | true | false | true | Flap was opened, movement inside and no light = letter was added and GL5528 is faulty |
| false | true | true | false | Flap was opened + No movement in the box = IF very bright -> box was opened IF not as bright -> flap was opened but nothing was inserted |
| false | true | true | true | Flap was opened + Movement in the box + light detected = Letter was added (100%) |
| true | false | false | false | Only box switch = faulty box switch |
| true | false | false | true | box switch + movement in the box = box opened, faulty photoresistor |
| true | false | true | false | box switch + light = box opened (100%) |
| true | false | true | true | box switch + light + movement in the box = box opened (100%) |
| true | true | false | false | box switch + flap switch = box opened, faulty photoresistor |
| true | true | false | true | box switch + flap switch + movement in the box = box opened, faulty photoresistor |
| true | true | true | false | box switch + flap switch + light = box opened only |
| true | true | true | true | box switch + flap switch + light + movement in the box = box opened + something added/removed |

**Summarized**

| Box reed switch | Flap reed switch | Photoresistor | Ultrasonic sensor | Meaning |
|---|---|---|---|---|
| false | false | false | false | nothing |
| false | false | false | true | nothing |
| false | false | true | false | nothing |
| false | false | true | true | flap opened (80%-90%) |
| false | true | false | false | flap opened |
| false | true | false | true | flap opened |
| false | true | true | false | flap opened |
| false | true | true | true | flap opened |
| true | false | false | false | box opened |
| true | false | false | true | box opened |
| true | false | true | false | box opened |
| true | false | true | true | box opened |
| true | true | false | false | box opened |
| true | true | false | true | box opened |
| true | true | true | false | box opened |
| true | true | true | true | box opened |

Notes:
- `true` from a N/C reed switch is reliable. It means the magnet was physically there and then left (flap opened).
- row `false | true | true | false` could rely on duration instead of brightness. Box opened sustains high light for more seconds.

### structure
- P2P LoRa: the LilyGo boards talk directly to each other with no infrastructure in between.
- ESP32 receiver -> homelab with MQTT: the receiver will publish to a topic, like envelope/event, and the homelab side subscribes to it. If the app restarts, messages can be buffered. The ESP32 doesn't need to know or care what's on the other end.
- SQLite x Redis: redis is a cache first db, persistence is opt-in. SQLite is the appropriate tool here since it is a single file with append-only inserts and can handle thousands of rows trivially without the need of an extra container.
- Homelab service: I can create one small container that will subscribe to my Mosquitto broker, write incoming data to SQLite and serve a simple HTML table. At this scale I don't need separate backend/frontend services.
- Traefik will be used to route to envelope.kaoshome.dev.
- It is possible to integrate with Home Assistant. I have to point HA's MQTT integration at my Mosquitto broker and define a sensor. Once that's done I can manage state history, use Lovelace history cards to give me a timeline view and implement push notifications and automations. I can look into this after implementing my custom service.

Structure:

```
envelope/
├── hub/            # Mosquitto + app + UI
├── receiver/       # PlatformIO
├── shared/         # common packet definitions
└── transmitter/    # PlatformIO
```

### MQTT
Mosquitto is the MQTT broker sitting in the middle. The receiver is an MQTT client that publishes to `envelope/event` and the hub will have an event handler that is another MQTT client that subscribes to that topic.

Design decisions:
- Receiver side:
  - The receiver is always connected to wifi
  - If the broker is unreachable (during a homelab reboot or network blip for example), the receiver must queue the event in-memory
  - The receiver must use JSON as the payload format - this is easier to consume on the hub side and easier to debug
- Topic structure: The topic will be `envelope/event` for now - more topics can be added later if needed
- Hub side:
  - The hub app subscribes to the topic, parses the payload, writes to SQLite and serves the webpage
  - The hub app will live in its own container
  - The QoS level will be QoS 1 (at least once)
  - The receiver needs the MQTT broker's IP address to establish a connection. When testing, I'll be using my computer's IP address and in production, my homelab's (RPI) IP address. To achieve IP stability, I have to make sure that I configure DHCP reservation for both devices so their IPs don't change.
  - Healthcheck:
    - Receiver: Since the receiver is always connected to Wifi/MQTT, I can use MQTT's built in last will testament (LWT). For that, I configure a "will" message when connecting (`envelope/status/receiver` -> `offline`) that mosquitto publishes automatically if the receiver disconnects unexpectedly. The receiver also publishes `online` on connect. The UI just reads this topic.
    - Transmitter: it can't be pinged since it's in deepsleep. I can add a periodic heartbeat wakeup with `esp_sleep_enable_timer_wakeup()` alongside the existing reed switch wakeup. The transmitter wakes up every 30 minutes, sends a small "i'm alive" packet to the receiver and goes back to sleep. The receiver forwards this to `envelope/status/transmitter` with a timestamp and the UI shows "last seen X minutes ago". 

**Payload schema**:

```json
{
  "seqNumber": 1,
  "wasFlapOpened": true,
  "wasBoxOpened": true,
  "lightLevel": 30,
  "distanceCm": 13,
  "flapMagnetPresent": true,
  "boxMagnetPresent": true,
  "batteryVoltage": 4.2,
  "rssi": -87,
  "timestamp": "2026-06-17T10:25:00Z" // ISO 8601
}
```

**Logic in main.cpp**:
- Setup:
  - connect to wifi
  - sync NTP time
  - connect to mqtt broker
- Loop:
  - check for incoming lora packet
  - if incoming packet: build payload, attempt publish / queue if broker is down
  - maintain connections - wifi and mqtt can drop independently
  - drain the queue when connectivity is restored

**Interfaces**:
- WiFi:
  - connect() - connect to wifi
  - syncTime() - ntp sync, called once after connect
  - getTimestamp() - returns the current iso 8601 string
  - isConnected() - needed so MQTTClient knows whether to recconnect with broker
- MQTTClient:
  - connect() - connect to broker
  - publish() - publishes to topic
  - loop() - called every iteration of main.cpp's loop, drains the queue when connection is restored and maintains the broker connection

#### code architecture
- Shared struct with no methods for the transmitted/received packet format.
- Sensor classes encapsulating pin config, thresholds and reading logic.
- Manager class to handle operations on the transmitter.

```
shared/
  packet.h
  lora_config.h     <--- LoRa boilerplate (frequency, bandwith, spreading factor etc)

transmitter/
  sensors/
    ReedSwitch.h/.cpp
    Photoresistor.h/.cpp
    Ultrasonic.h/.cpp
  LoRaRadio.h/.cpp
  EventManager.h/.cpp
  main.cpp

receiver/
  LoRaRadio.h/.cpp
  MQTTClient.h/.cpp
  main.cpp
```

## steps
1. Start ESP32 project (should I have one repository storing the transmitter and receiver code or one repository for each?)
2. Validate components:
   - Make ESPs talk to each other ✓
   - Validate TCRT5000 (infrared reflective sensor) ✓
   - Validate N/C reed switch ✓
   - Validate GL5528 (photoresistor) ✓
   - Validate HC-SR04 (ultrasonic sensor) ✓
3. Design code architecture
   - Deep sleep - deep sleep wakeup only monitors voltage levels on digital GPIO lines. it has no ADC, so the reed switch is the only option. i can use the ext1 mode and have 2 reed switches: one on the flap and one on the box.
   - packet struct?
   - LoRa ACK - transmitter sends packet, waits ~1s for an ACK and retries up to 3 times if it doesn't get one; if all fail, log the error.