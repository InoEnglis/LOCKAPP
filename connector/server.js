const { SerialPort } = require('serialport');
const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const port = 5000;

// Create a new SerialPort instance
const serialPort = new SerialPort({
  path: 'COM4', // Change this to the correct port if necessary
  baudRate: 9600,
});

// Open the SerialPort and handle errors
serialPort.on('open', () => {
  console.log('Serial port opened successfully');
});

serialPort.on('error', (err) => {
  console.error('Error opening serial port: ', err.message);
});

app.use(bodyParser.json());

app.post('/unlock', (req, res) => {
  serialPort.write('U', (err) => {
    if (err) {
      console.error('Failed to send unlock command:', err.message);
      return res.status(500).json({ error: 'Failed to send unlock command' });
    }
    res.json({ message: 'Unlock command sent' });
  });
});

app.post('/lock', (req, res) => {
  serialPort.write('L', (err) => {
    if (err) {
      console.error('Failed to send lock command:', err.message);
      return res.status(500).json({ error: 'Failed to send lock command' });
    }
    res.json({ message: 'Lock command sent' });
  });
});

app.get('/status', (req, res) => {
  serialPort.write('S', (err) => {
    if (err) {
      console.error('Failed to send status command:', err.message);
      return res.status(500).json({ error: 'Failed to send status command' });
    }

    // Wait for the response from Arduino
    serialPort.once('data', (data) => {
      const status = data.toString().trim();
      res.json({ status });
    });
  });
});

app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
