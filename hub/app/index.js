import dotenv from 'dotenv'
dotenv.config({ path: '.env' })

import Database from 'better-sqlite3'
import mqtt from 'mqtt'
import express from 'express'

/**
 * @typedef {Object} Payload
 * @property {number} seqNumber
 * @property {boolean} wasFlapOpened
 * @property {boolean} wasBoxOpened
 * @property {number} lightLevel
 * @property {number} distanceCm
 * @property {boolean} flapMagnetPresent
 * @property {boolean} boxMagnetPresent
 * @property {number} batteryVoltage
 * @property {number} rssi
 * @property {string} timestamp
 */

const db = new Database('/db/envelope.db')
db.exec(`
  CREATE TABLE IF NOT EXISTS events (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    seq_number INTEGER,
    was_flap_opened INTEGER,
    was_box_opened INTEGER,
    light_level INTEGER,
    distance_cm INTEGER,
    flap_magnet_present INTEGER,
    box_magnet_present INTEGER,
    battery_voltage REAL,
    rssi INTEGER,
    timestamp TEXT,
    received_at TEXT DEFAULT (datetime('now'))
  )
`)

const mqttBrokerAddress = process.env.MQTT_BROKER
const mqttBrokerPort = process.env.MQTT_PORT ?? 1883

if (!mqttBrokerAddress) {
	throw new Error('Missing MQTT_BROKER as env var')
}

const mqttOptions = {
	clientId: 'envelope-app',
	username: process.env.MQTT_USERNAME,
	password: process.env.MQTT_PASSWORD,
}

const mqttClient = mqtt.connect(
	`mqtt://${mqttBrokerAddress}:${mqttBrokerPort}`,
	mqttOptions,
)

const eventTopic = 'envelope/event'

mqttClient.subscribe(eventTopic)

mqttClient.on('message', (topic, message) => {
	// console.log(`message: ${message}, topic: ${topic}`)

	/** @type {Payload} */
	const payload = JSON.parse(message.toString())

	db.prepare(
		`
	  INSERT INTO events (
	    seq_number,
	    was_flap_opened,
	    was_box_opened,
	    light_level,
	    distance_cm,
	    flap_magnet_present,
	    box_magnet_present,
	    battery_voltage,
	    rssi,
	    timestamp
	  ) VALUES (
	    ?,
	    ?,
	    ?,
	    ?,
	    ?,
	    ?,
	    ?,
	    ?,
	    ?,
	    ?
	  )
	`,
	).run(
		payload.seqNumber,
		payload.wasFlapOpened ? 1 : 0,
		payload.wasBoxOpened ? 1 : 0,
		payload.lightLevel,
		payload.distanceCm,
		payload.flapMagnetPresent ? 1 : 0,
		payload.boxMagnetPresent ? 1 : 0,
		payload.batteryVoltage,
		payload.rssi,
		payload.timestamp,
	)

	console.log(`Added event with seqNumber ${payload.seqNumber} to db`)

	const newEvent = {
		seq_number: payload.seqNumber,
		was_flap_opened: payload.wasFlapOpened ? 1 : 0,
		was_box_opened: payload.wasBoxOpened ? 1 : 0,
		distance_cm: payload.distanceCm,
		battery_voltage: payload.batteryVoltage,
		rssi: payload.rssi,
		timestamp: payload.timestamp,
	}

	for (const client of sseClients) {
		client.write(`data: ${JSON.stringify(newEvent)}\n\n`)
	}
})

const sseClients = new Set()

const app = express()

app.get('/', (req, res) => {
	const events = db.prepare('SELECT * FROM events ORDER BY id DESC').all()
	res.send(renderPage(events))
})

app.get('/events', (req, res) => {
	res.setHeader('Content-Type', 'text/event-stream')
	res.setHeader('Cache-Control', 'no-cache')
	res.setHeader('Connection', 'keep-alive')
	res.flushHeaders()

	sseClients.add(res)
	req.on('close', () => sseClients.delete(res))
})

app.listen(3000, () => console.log('Listening on http://localhost:3000'))

function renderPage(events) {
	const rows = events
		.map(
			(e) => `
        <tr>
          <td data-timestamp="${e.timestamp}"></td>
          <td>${e.was_flap_opened ? 'Yes' : 'No'}</td>
          <td>${e.was_box_opened ? 'Yes' : 'No'}</td>
          <td>${e.distance_cm} cm</td>
          <td>${e.battery_voltage.toFixed(2)} V</td>
          <td>${e.rssi} dBm</td>
          <td>${e.seq_number}</td>
        </tr>
      `,
		)
		.join('')

	return `<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>Envelope</title>
  <link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg'><text y='32' font-size='32'>📨</text></svg>">
	<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/@picocss/pico@2/css/pico.min.css">
</head>
<body>
	<main class="container">
		<h1>Mailbox Events</h1>
		<table>
			<thead>
				<tr>
					<th>Timestamp</th>
					<th>Flap opened</th>
					<th>Box opened</th>
					<th>Distance</th>
					<th>Battery</th>
					<th>RSSI</th>
					<th>Seq #</th>
				</tr>
			</thead>
			<tbody>
				${rows}
			</tbody>
		</table>
	</main>
<script>
	function formatDate(timestamp) {
		return new Date(timestamp).toLocaleString('en-GB', {
			timeZone: 'Europe/Zurich',
			day: '2-digit', month: '2-digit', year: 'numeric',
			hour: '2-digit', minute: '2-digit', second: '2-digit',
			hour12: false,
		}).replace(', ', ' - ')
	}

	document.querySelectorAll('[data-timestamp]').forEach(td => {
		td.textContent = formatDate(td.dataset.timestamp)
	})

	const source = new EventSource('/events')
	source.onmessage = (event) => {
		const e = JSON.parse(event.data)
		const row = document.createElement('tr')
		row.innerHTML = \`
			<td>\${formatDate(e.timestamp)}</td>
			<td>\${e.was_flap_opened ? 'Yes' : 'No'}</td>
			<td>\${e.was_box_opened ? 'Yes' : 'No'}</td>
			<td>\${e.distance_cm} cm</td>
			<td>\${e.battery_voltage.toFixed(2)} V</td>
			<td>\${e.rssi} dBm</td>
			<td>\${e.seq_number}</td>
		\`
		document.querySelector('tbody').prepend(row)
	}
</script>
</body>
</html>`
}
