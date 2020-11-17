Technical Specifications Sensors



**A)  Temperature and Humidity sensor – sensiron_SHT25**



\1. Communication interface (Hardware interface)

- Digital 2-wire interface, ![equation.pdf](blob:file:///7f5306e8-1eaa-44db-a7d0-c7ddf3b41dd2)C protocol
- Connection

 

| **Pin** | **Name** | **Comment**                 |
| ------- | -------- | --------------------------- |
| 1       | SDA      | Serial Data, bidirectional  |
| 2       | VSS      | Ground                      |
| 5       | VDD      | Supply Voltage              |
| 6       | SCL      | Serial Clock, bidirectional |
| 3,4     | NC       | Not connected               |



- Capacitor between GND and VDD, about C=100nF.
- Need external pull-up resistor between (SDA and VDD) and between (SCL and VDD), about 10KOhms.

\2. Power specifics

- The sensor is powered with DC supply voltage between 2.1V and 3.6V, typically 3.0V
- Electrical absolute max.

 

| **Parameter**                      | **min** | **max**   | **Unit** |
| ---------------------------------- | ------- | --------- | -------- |
| VDD to VSS                         | -0.3    | 5         | V        |
| Digital I/O Pins (SDA, SCL) to VSS | -0.3    | VDD + 0.3 | V        |
| Input Current on any Pin           | -100    | 100       | mA       |



Current consumption during start up is 350 ![equation_1.pdf](blob:file:///8394765e-0e36-40a4-ba3f-fa44da7e6336)A

\3. Measurement operation.

- Temperature range: -40 to 125 ![equation_2.pdf](blob:file:///f8d97e2a-5273-48f0-aab9-b2dc87a6e71c), Response time: 5s to 30s, Accuracy : ![equation_3.pdf](blob:file:///a436d7b3-0490-4083-bb3e-d5596594be5a) 
- Humidity range: 0 to 100 %RH, Response time: 8s, Accuracy : ![equation_4.pdf](blob:file:///2bcc8393-a350-4424-814d-216009538975)



\4. Software interface (Programming C++)

- mbed.h
- SHT25.h



**B)**  **![equation_5.pdf](blob:file:///ff7dfe60-4799-4dc7-8de0-f11d862f89bf)** **sensor from SenseAir AB – PSP110**



\1. Communication interface (Hardware interface)

- Digital 2-wire interface, ![equation_6.pdf](blob:file:///b5802aec-b896-41c7-a041-a2c6de4e7731)C protocol
- Connection

 

|         | **Pin Name**                                                 | **Comment** |
| ------- | ------------------------------------------------------------ | ----------- |
| DVCC    | Used for level conversion circuitry in some cases (may not be used) |             |
| G0      | Power “![equation_7.pdf](blob:file:///518a0542-6abd-474a-8c67-64f90ec080b3)”, Sensor’s ground (GND) |             |
| G+      | Power “+”                                                    |             |
| I2C_SCL | Serial Clock, bidirectional line                             |             |
| I2C_SDA | Serial Data, bidirectional line                              |             |





\2. Power specifics

- The sensor is powered with DC supply voltage between 4.5 and 14.0V, preferred 5.0 – 9.0V

With G+ referred to G0.

- Current consumption.

Average 40mA

< 150mA peak current (averaged during IR lamp ON, 120msec)

 < 300mA peak power (during IR lamp start-up, the first 50msec)

\3. Measurement.

- Range: 0 – 5000ppm, Response time: 20 sec diffusion time, Accuracy *:* *![equation_8.pdf](blob:file:///a0ff0b02-b098-42d0-a810-06d55a7dc113)* (**ppm = parts per million**, is the ratio of one gas to another)
- Rate of measurement: 0.5Hz

\4. Functional description.

- I2C parameter for sensor

- Data rate is 100Kbits/s
- Addressing mode is 7bit
- Addressing range 0x00 to 0x7F (in EEPROM size 128b for read/write)

\4. Software interface (Programming C++)









**Alternatively**.



- UART serial communication interface can be used.
- UART connector Connection



| **Pin name** | **Comment**         |
| ------------ | ------------------- |
| TxD          | Transmit data       |
| RxD          | Receive data        |
| 5V (G+)      | Sensor power supply |
| GND (G0)     | Sensor ground       |





**C) Dust particle sensor – GP2Y1010AU0F**



Optical dust (air quality) sensor is designed to sense dust particles.

\1. Communication interface (Hardware interface)

- Connection

 

| **Pin number** | **Pin name** | **Comment**                                              |
| -------------- | ------------ | -------------------------------------------------------- |
| 1              | V-LED        | Connect to 5V (or 3.3V) with 150 Ohm resistor in between |
| 2              | LED-GND      | Connect to GND                                           |
| 3              | LED          | Connect to any digital pin                               |
| 4              | S-GND        | Connect to GND                                           |
| 5              | Vo           | Connect to any analog pin                                |
| 6              | Vcc          | Connect to 5V or 3.3V                                    |



- Connect capacitor between LED-GND and V-LED, C=220uF.

\2. Power specifics

- Electrical absolute max. DC supply

 

| **Parameter** | **min** | **max** | **Unit** |
| ------------- | ------- | ------- | -------- |
| Vcc           | -0.3    | +7      | V        |
| V-LED         | -0.3    | Vcc     | V        |



- - Typical input operating supply voltage for LED input terminal is Vcc = 5![equation_9.pdf](blob:file:///e908b1d1-b02f-42a0-9850-d3b8f2583793)0.5 V (MAX. 7V).
  - Low current consumption (Typical: 11mA, MAX: 20 ![equation_10.pdf](blob:file:///a138efb9-cf99-49e8-a819-ef2373fbcb17)A)
  - Output voltage at no dust: Typical 0.9V, Min.: 0V, Max.: 1.5V





\3. LED measurement operation.

- Pulse Cycle (T): 10![equation_11.pdf](blob:file:///dceb9e42-6fca-4227-96d9-56fff082ff8d)1 ms
- Pulse Width (Pw): 0.32![equation_12.pdf](blob:file:///a077a3b4-cc7e-4769-807f-4f39a6798f8f)0.02 ms

\4. Operating temperature: -10 to +65 ![equation_13.pdf](blob:file:///5eb3bb01-737f-424e-a755-f893fd920fbc)

\5. Sensitivity: Typical: 0.5V/(0.1 mg/![equation_14.pdf](blob:file:///049aeb56-647f-4d4a-b483-64659e66b939)), Minimum: 0.35V/(0.1 mg/![equation_15.pdf](blob:file:///35e65804-30d0-4841-98c8-678d1e206c1d)), Max.: 0.65V/(0.1 mg/![equation_16.pdf](blob:file:///97354a51-3ee3-4008-9246-af2441d905a8))

\5. Software interface (Programming C++)