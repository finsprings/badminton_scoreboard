badminton-scoreboard
====================

An Arduino sketch for keeping score in a game of badminton.

Uses the Adafruit Simple RF M4 Receiver - 315MHz Momentary Type (http://www.adafruit.com/products/1096), and associated keyfobs, and the Adafruit 1.2" 4-Digit 7-Segment Display w/I2C Backpack (http://www.adafruit.com/products/1268).

Controls:
* Keyfob A button: left player + 1
* Keyfob B button: right player + 1
* Keyfob C button: left player - 1
* Keyfob D button: right player - 1

When the game is over the display will flash. Pressing any key will reset the score to 0-0 ready for the next game.
