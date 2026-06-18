# Hub
## App
Workflow should be: init DB -> connect to MQTT -> on message: parse + insert -> Express serves the table.

## Mosquitto
### Setup
Before starting the container for the first time, a password has to be generated. Run this command:

```sh
docker run --rm -it -v $(pwd)/mosquitto/config:/mosquitto/config eclipse-mosquitto:2 mosquitto_passwd -c /mosquitto/config/passwd envelope
```

### Running the app
Run `docker compose up` to start the docker services.

We can test that the Mosquitto broker is working using the mosquitto package

Ubuntu:

```sh
sudo apt install mosquitto-clients
```

Mac:

```sh
brew install mosquitto
```

Then, with the container running, run the following command to subscribe to topic "Test":

```sh
mosquitto_sub -i mosq_sub1 -t "Test" -u <username> -P <password> -d
```

On another tab, run this command to publish to that topic:

```sh
for i in {1..10}; do (mosquitto_pub -i mosq_pub1 -t "Test" -m "Message with ID: $i" -u <username> -P <password> -d & sleep 1); done
```

You should see something like this:

```
Message with ID: 1
Client mosq_sub1 received PUBLISH (d0, q0, r0, m0, 'Test', ... (18 bytes))
Message with ID: 2
Client mosq_sub1 received PUBLISH (d0, q0, r0, m0, 'Test', ... (18 bytes))
Message with ID: 3
Client mosq_sub1 received PUBLISH (d0, q0, r0, m0, 'Test', ... (18 bytes))
```