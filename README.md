# melodiphone

Max and Arduino code for the melodiphone, a digital attachment for a mellophone or other brass instrument.

More details on the melodiphone at [nathanpruyne.com/melodiphone](https://nathanpruyne.com/melodiphone)

* **melodiphone/melodiphone.ino**

Arduino code for sending sensor data over WiFi. Tested on Arduino UNO R4 WiFi.
Ports for sensors should be updated in the `sensors` array on line 19, and buttons on the `buttons` array on line 20.

* **melodiphone/arduino_secrets.h**

File for WiFi SSID and password

* **melodiphone.maxpat**

![image](https://github.com/user-attachments/assets/293f0540-cf54-444d-b1b5-2f2a0d393f63)

Max patch for polling Arduino, processing sensor data out, and playing data. Current configuration is for timbre transfer using DDSP and playing drum samples (`kick.wav` and `snare.wav` included).

* **hardsynth/**

Hard synthesizer patch for [DDSP-VST](https://magenta.tensorflow.org/ddsp-vst) that works nicely with the melodiphone.
