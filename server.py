from flask import Flask, request, jsonify
from flask_cors import CORS
import serial
import logging


logging.basicConfig(level=logging.DEBUG)

app = Flask(__name__)
CORS(app)


ser = None
try:
    ser = serial.Serial('COM3', 9600, timeout=1)
    logging.info("Serial port opened successfully.")
except serial.SerialException as e:
    logging.error(f"Error opening serial port: {e}")


door_locked = True

@app.route('/door_status', methods=['GET'])
def get_door_status():
    try:
        status = 'Locked' if door_locked else 'Unlocked'
        logging.info(f"Door status requested: {status}")
        return jsonify({'status': status})
    except Exception as e:
        logging.error(f"Error getting door status: {e}")
        return jsonify({'error': str(e)}), 500

@app.route('/unlock', methods=['POST'])
def unlock_door():
    global door_locked
    logging.info("Unlock request received.")
    if ser is None or not ser.is_open:
        logging.error("Serial port is not open.")
        return jsonify({'error': 'Serial port not available'}), 500

    try:
        logging.info("Sending unlock command to Arduino.")
        ser.write(b'U') 
        door_locked = False
        logging.info("Unlock command sent successfully.")
        return jsonify({'message': 'Unlock command sent'})
    except Exception as e:
        logging.error(f"Error sending unlock command: {e}")
        return jsonify({'error': str(e)}), 500

@app.route('/lock', methods=['POST'])
def lock_door():
    global door_locked
    logging.info("Lock request received.")
    if ser is None or not ser.is_open:
        logging.error("Serial port is not open.")
        return jsonify({'error': 'Serial port not available'}), 500

    try:
        logging.info("Sending lock command to Arduino.")
        ser.write(b'L') 
        door_locked = True
        logging.info("Lock command sent successfully.")
        return jsonify({'message': 'Lock command sent'})
    except Exception as e:
        logging.error(f"Error sending lock command: {e}")
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=5000)
    except Exception as e:
        logging.error(f"Error starting Flask server: {e}")
