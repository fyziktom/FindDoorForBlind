# Find Door For Blind People

This is simple detector of the door for the blind people in strange environment.

[Here you can find the video with the demonstration of the prototype.](https://youtu.be/Z6-WHQ-ywQA)

## Principle 

If you need to find door, just raise your hand and the ultrasound detector will detect your hand and module will produce the sound.

The code converts the measured distance to the frequency of the sound. As closer you get the lower sound is produced. 

## Firmware

The code is build in the Arduino IDE. You will need just the [M5StickC library](https://github.com/m5stack/M5StickC/)

## Hardware

The Hardware is pretty simple. I used M5StickC with [battery module](https://shop.m5stack.com/products/m5stickc-18650), [Speaker HAT](https://shop.m5stack.com/products/m5stickc-speaker-hat) and common Arduino Ultrasound detector.

![IMG_20231022_154359_340](https://github.com/fyziktom/FindDoorForBlind/assets/78320021/9ed2fbbf-6b59-42c7-b377-11b00068da21)

I have used Grove connector to get power supply for ultrasound detector and the two pins to connect Triger (pin 33) and Echo (pin 32) pins of the detector. 

## Disadvantages of actual HW solution

The ultrasound detector does not reach so far distance. It would be better to use some microwave detector or any other detection principle which can measure far distances. I have built it as example so please do not judge the exact prototype :)
