import React from 'react';
import { View, Text, StyleSheet, TouchableOpacity, Image } from 'react-native';
import axios from 'axios';

const serverUrl = 'http://192.168.43.139:5000';

export default function App() {
  const sendUnlockCommand = async () => {
    try {
      const response = await axios.post(`${serverUrl}/unlock`);
      setTimeout(async () => {
        try {
          await axios.post(`${serverUrl}/lock`);
        } catch (error) {
          console.error('Error locking door:', error);
        }
      }, 3000); // Wait for 3 seconds before locking the door again
    } catch (error) {
      console.error('Error unlocking door:', error);
    }
  };

  return (
    <View style={styles.container}>
      <Image
        source={{ uri: 'https://example.com/your-image-url.jpg' }} // Replace with your image URL
        style={styles.image}
      />
      <Text style={styles.title}>GremLOCK Control</Text>
      <View style={styles.card}>
        <TouchableOpacity style={styles.button} onPress={sendUnlockCommand}>
          <Text style={styles.buttonText}>Unlock Door</Text>
        </TouchableOpacity>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#121212', // Dark background for a modern look
  },
  image: {
    width: '50%',
    height: 150,
    resizeMode: 'contain',
    marginBottom: 30,
    borderRadius: 8,
  },
  title: {
    fontSize: 28,
    fontWeight: 'bold',
    color: '#BB86FC', // Light purple for accent
    marginBottom: 20,
  },
  card: {
    width: '80%',
    backgroundColor: '#1E1E1E', // Darker card background
    borderRadius: 12,
    padding: 20,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 4 },
    shadowOpacity: 0.3,
    shadowRadius: 6,
    elevation: 5,
    alignItems: 'center',
  },
  button: {
    backgroundColor: '#03DAC6', // Teal for buttons
    borderRadius: 8,
    paddingVertical: 14,
    paddingHorizontal: 28,
    marginVertical: 10,
    width: '100%',
    alignItems: 'center',
  },
  buttonText: {
    color: '#121212', // Dark text for contrast
    fontSize: 18,
    fontWeight: '600',
  },
});
