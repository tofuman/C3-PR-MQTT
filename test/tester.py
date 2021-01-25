#!/usr/bin/env python3
import paho.mqtt.client as mqtt
import time
 
client = mqtt.Client()
client.connect("192.168.178.44", 1883, 60)
client.loop_start()
while (True):
    client.publish("action/robo1", "links")
    time.sleep(10)
client.disconnect()
