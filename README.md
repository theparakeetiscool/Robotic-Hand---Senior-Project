README

This repository is dedicated to storing the code I wrote for an Arduino UNO during my senior project at BASIS

If you'd like to use this code you will likely have to change out the pins according to what you're using (and what board you're using)

To utilize the keyboard control in the code, you need some sort of program that can send keystrokes as serial data (Arduino IDE doesn't have this as a feature).
I personally used coolterm but it's not super important. Unfortunately, doing it this way means that you can't write data to serial while using the serial terminal application
This is the main reason why encoder data isn't incorporated, because I could not find a good way of displaying it.

Hardware required:
2 L298N Motor Drivers
5 N20 DC Motors
External power source (preferably 2V and >2A)
Breaboard / Protoboard
Pin Wires
Connection Cable (In the case of an Arduino UNO, a USB A to USB B cable)

Overall, this project costed me ~$180 but you can probably do it for ~$160 if you buy good value products and aren't on a strict timeline.

For any questions or concerns, feel free to reach out at johnawells07@gmail.com
