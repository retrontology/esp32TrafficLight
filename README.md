# esp32TrafficLight

## Description
A program I created to use an ESP32 to control a traffic light with relays using an HTML webserver

## Setup
- Change `ssid` and `password` to the proper values required to access the wireless access point.
- Change `red`, `yellow`, and `green` to the pins you will connect to the relays for each respective light
- Build and deploy to your ESP32

## Usage
Find the network IP of your ESP32 after it connects to the network and go to it in your web browser. You will be presented with an illustraion of a traffic light. Simply click on a light to turn it off or on.