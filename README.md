badminton_scoreboard
====================

An Arduino sketch for keeping score in a game of badminton.

Uses:
* Simple RF M4 Receiver - 315MHz Momentary Type (http://www.adafruit.com/products/1096).
* Keyfob 4-Button RF Remote Control - 315MHz (http://www.adafruit.com/products/1095).
* 1.2" 4-Digit 7-Segment Display w/I2C Backpack (http://www.adafruit.com/products/1268).

Controls:
* Keyfob A button: left player + 1
* Keyfob B button: right player + 1
* Keyfob C button: left player - 1
* Keyfob D button: right player - 1

When the game is over the display will flash. Pressing any key will reset the score to 0-0 ready for the next game.
