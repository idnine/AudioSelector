Audio Selector
====

Audio Input Selector
 * Audio Multiplexer : TS3A5017 (TI, 16pin)
 * Controller : MSP430-G2-553 (TI, 20pin)
 * Coding ENV. : Code Composer Studio
 * Code by Koo Jonghoe (idnine@gmail.com, AudioCookie)
 * Feb. 5, 2014

MSP430 SETUP
 * pin 01 : VCC
 * pin 20 : GND
 * pin 16 : RST (pull-up)
 * pin  5 (P1.3) : CH1 LED (Active High)
 * pin  6 (P1.4) : CH2 LED (Active High)
 * pin  7 (P1.5) : CH3 LED (Active High)
 * pin 14 (P1.6) : Selector Switch (pull-up, Active Low)
 * pin 15 (P1.7) : Mute Switch (pull-up, Active Low)

MSP430 and TS3A5017 CONNECTIONS
 * pin  9 (P2.0) : TS3A5017 EN1(1), EN2(15)
 * pin  9 (P2.1) : TS3A5017 IN1 (14)
 * pin 10 (P2.2) : TS3A5017 IN2 (2)

MSP430 -> TS3A5017 signal (EN, IN1, IN2)
 * CH1  : L, L, L  (LED 1 ON)
 * CH2  : L, H, L  (LED 2 ON)
 * CH3  : L, L, H  (LED 3 ON)
 * MUTE : H, x, x  (LED Blink)

