# C3-PR-MQTT
C3 PresenceRobot rewritten to utilize MQTT

This is a refactor/rewrite of https://github.com/c3d2/C3-PR .
The aim is to switch controling the robot via mqtt messages, which also opens the path to a feedback channel from the robot to the controler.
Further more its it easier to control multiple robots via one mqtt client. The Server controlling the robots is currently rewritten in Rust after the very hacky JS solution for RC3 prooved to unstable.
