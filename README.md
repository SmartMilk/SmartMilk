# Summary
This is an automated system that notifies your smartphone when milk has been left out of fridge / about to expire

# Hardware

1. Raspberry Pi (Model B Rev 2)
2. DS18B20 Waterproof Temperature Sensor
3. 4.7k pullup resistor
4. Custom PCB (connecting the sensor to the raspberry pi)
5. Bluetooth Module (nano USB Bluetooth Dongle 2.0) 
6. iPhone running custom ios app
7. Milk container with milk

# Purpose of the Project
This project is for all those milk-connoisseurs out there who have careless family members/flatmates who leave the milk outside the fridge. The DS18b20 waterproof sensor's probe is placed inside the user's milk carton (ideally this would be a re-usable bottle/container) and monitors the temperature of the milk. The sensor communicates with the raspberry pi (located externally) using Dallas 1-wire communication protocol. Once the temperature of the milk is approaching room temperature, the raspberry pi will, after a certain period of time, alert the user by sending a notification to their smartphone (using blutooth communication), notifying them on a custom ios app that the milk has reached unsafe temperatures and are at risk of spoiling their milk. As time further passes, different alerts will be sent out, eventually telling the user that drinking the milk is not recommended. The ios app will also allow the user to enter the expiry date for their milk, which the app will then automatically track and notify the user once the expiry date is nearing. 

Additional graphical output will be sent to a PC monitor which displays the temperature of the milk in real time, and can be configured to display temperature in celsius, farenheit and kelvin. 

