# envelope

## project description
I live on the first floor of my building and, approximately 50 meters away from my office's window is my mailbox. The mailbox is a closed metal box with a space on its front part for letters/envelopes to be inserted. When the mailman arrives, he pushes this opening, inserting the letter. The box's dimensions are:
- Internal compartment: 25cm x 40cm
- Opening: 35cm x 3cm

The objective of this project, called "envelope", is to build a smart detection system that sits inside the mailbox. When something is inserted in it, the system detects it and sends a signal to a receiver.

## available components
- 3x Lilygo T3S3 ESP32-S3 Dev Board LoRa Module (868 MHz)
- 30x circular magnet (10x3)
- 3x LCD1602 display module
- 5x TCRT5000 Infrared Reflective Sensor IR Photoelectric Switch Barrier
- 4x 18650 battery
- 10x N/O reed switch 2*14mm
- 10x N/C reed switch 2*14mm
- 20x photoresistor GL5528
- soldering equipment: soldering iron, soldering wire, copper welding desoldering wire, iron tip refresher, pcb boards, tip cleaners

## solution design
### Multi-sensor fusion
I can make the three sensors complement each other:
  - Reed switch on the flap: will detect when something is pushed through the slot. Detects the mechanical event directly, regardless of lighting conditions.
  - Photoresistor: detects the light spike when the slot opens.
  - TCRT5000: detects physical presence of an object inside the box. Can be used as a confirmation that something actually landed, not just that the flap was touched.

I can use the reed switch + photoresistor at insertion time and then TCRT5000 to confirm when something is in the box.

### Eliminating false positives
A false positive can happen in three ways: when something disturbs one of the sensors, making it seem that the slot was opened/a letter was inserted; when someone opens the slot with their hand but nothing is inserted; and when we open the box with a key to collect anything that's inside. To avoid this, we can follow a time/threshold approach in which we classify events by two dimensions: duration and intensity. A brief, partial light event is mail arriving. A prolonged, full intensity event across all sensors simultaneously is the box being opened with a key.

### N/O x N/C, which type to use?
For a flap that rests closed and briefly opens:
  - N/C means the circuit breaks when all the flap opens, so we detect the absence of the magnet. This is the more reliable choice because a dead battery or broken wire looks the same as "no event", not a false trigger
  - N/O would trigger on any wire fault, which could cause false alerts

### architecture
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

## steps
1. Start ESP32 project (should I have one repository storing the transmitter and receiver code or one repository for each?)
2. Validate components:
   1. Make ESPs talk to each other
   2. Validate infrared reflective sensor
   3. Validate N/O reed switch
   4. Validate N/C reed switch
   5. Validate photoresistor
3. ...