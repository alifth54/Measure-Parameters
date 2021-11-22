# Measure-Parameters
In this project i want to measure folowing parameters with an Arduino-uno board:

1) Voltage (INA 219)
2) Current (INA 219)
3) humidity (DHT11)
4) motion (SW-180)
5) pressure (BMP 180)
6) Temperature (DHT11)

At firs user should set duration time of measurement. after 5 min if user  doesnt send any thing on serial port duration time will set as difualt(30 min).

i use rtc module in this project to finout time and date and use micro SD card reader to save results of measurements in text file. this text file will be change every day.
