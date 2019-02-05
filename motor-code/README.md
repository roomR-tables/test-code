# Arduino motor + driver test.

Important steps for testing:
 - [Read this here documentation first!](https://www.pololu.com/product/2133)
 - Always, always, set the current limit resistor on the driver. Our motors are designed for max. 1.7A, I recommend 1.2 as maximum.
 - Always disconnect 12V power first, then the Arduino power.
 - Don't forget the capacitor for the 12V power source

## Connecting the driver and Arduino

![Connecting the driver.](https://a.pololu-files.com/picture/0J4232.600.png?f2f6269e0a80c41f0a5147915106aa55)
#### Pins on Arduino:
- Step:		pin 3
- Dir:		pin 4


##### Manufacturer warnings:
- Why to use the capacitor: 
```Warning: This carrier board uses low-ESR ceramic capacitors, which makes it susceptible to destructive LC voltage spikes, especially when using power leads longer than a few inches. Under the right conditions, these spikes can exceed the 45 V maximum voltage rating for the DRV8825 and permanently damage the board, even when the motor supply voltage is as low as 12 V. One way to protect the driver from such spikes is to put a large (at least 47 µF) electrolytic capacitor across motor power (VMOT) and ground somewhere close to the board.```
- Why not to disconnect the Arduino first:
```Warning: Connecting or disconnecting a stepper motor while the driver is powered can destroy the driver. (More generally, rewiring anything while it is powered is asking for trouble.)```


