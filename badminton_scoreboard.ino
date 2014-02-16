/**
 * Badminton Score Board
 *
 * Uses the Adafruit Simple RF M4 Receiver - 315MHz Momentary Type,
 * and associated keyfobs, and the Adafruit 1.2" 4-Digit 7-Segment Display w/I2C Backpack.
 *
 * Controls:
 *
 * Keyfob A button: left player + 1
 * Keyfob B button: right player + 1
 * Keyfob C button: left player - 1
 * Keyfob D button: right player - 1
 *
 * When the game is over the display will flash. Pressing any
 * key will reset the score to 0-0 ready for the next game.
 *
 */

#include <Bounce2.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

// The dot types should be defined in Adafuit_LEDBackpack.h but aren't.
typedef enum DotType {
  CENTER_COLON_DOT_TYPE = 1 << 1,
  TOP_LEFT_DOT_TYPE = 1 << 2,
  BOTTOM_LEFT_DOT_TYPE = 1 << 3,
  BETWEEN_DIGITS_3_AND_4_DOT_TYPE = 1 << 4
} DotType;

typedef enum BlinkRate {
  BLINK_RATE_NONE = 0,
  BLINK_RATE_FAST = 1,
  BLINK_RATE_MEDIUM = 2,
  BLINK_RATE_SLOW = 3
} BlinkRate;

static const uint8_t DISPLAY_I2C_ADDRESS = 0x70;
static const uint8_t MAXIMUM_BRIGHTNESS = 15;

static const uint8_t LEFT_PLAYER_TENS_DIGIT = 0;
static const uint8_t LEFT_PLAYER_ONES_DIGIT = 1;
static const uint8_t DOTS_DIGIT = 2;
static const uint8_t RIGHT_PLAYER_TENS_DIGIT = 3;
static const uint8_t RIGHT_PLAYER_ONES_DIGIT = 4;

static const uint8_t A_PIN = 5; // M4 D3
static const uint8_t B_PIN = 4; // M4 D2
static const uint8_t C_PIN = 3; // M4 D1
static const uint8_t D_PIN = 2; // M4 d0

// Use debouncing of the keyfob pins via Bounce.
static Bounce a;
static Bounce b;
static Bounce c;
static Bounce d;

static Adafruit_7segment matrix;

static uint8_t left_player_score = 0;
static uint8_t right_player_score = 0;
static bool game_over = false;

void setup() {
  init_display();
  init_buttons();
}


void loop() {
  handle_button_press();
}

void init_display() {
  matrix.begin(DISPLAY_I2C_ADDRESS);
  matrix.setBrightness(MAXIMUM_BRIGHTNESS);
  matrix.writeDigitRaw(DOTS_DIGIT, CENTER_COLON_DOT_TYPE);

  update_display();
}

void init_buttons() {
  pinMode(A_PIN, INPUT);
  pinMode(B_PIN, INPUT);
  pinMode(C_PIN, INPUT);
  pinMode(D_PIN, INPUT);

  a.attach(A_PIN);
  b.attach(B_PIN);
  c.attach(C_PIN);
  d.attach(D_PIN);
}


/**
 * Look for a keyfob button press and handle it.
 */
void handle_button_press() {
  // Ask the debouncers whether the buttons have changed from
  // not being pressed to being pressed (we don't care
  // about the change from pressed to not pressed).
  const bool a_pressed = a.update() && (a.read() == HIGH);
  const bool b_pressed = b.update() && (b.read() == HIGH);
  const bool c_pressed = c.update() && (c.read() == HIGH);
  const bool d_pressed = d.update() && (d.read() == HIGH);

  const bool any_pressed = a_pressed || b_pressed || c_pressed || d_pressed;

  if (game_over) {
    // The game is over. Any key press will reset the score back to 0-0.
    if (any_pressed) {
      // Reset the scores (game_over will be recomputed by update_game_over).
      left_player_score = 0;
      right_player_score = 0;
    }
  } else {
    // In a game, so each key press has its own meaning.
    if (a_pressed) {
      left_player_score ++;
    }

    if (b_pressed) {
      right_player_score ++;
    }

    if (c_pressed) {
      if (left_player_score > 0) {
        left_player_score --;
      }
    }

    if (d_pressed) {
      if (right_player_score > 0) {
        right_player_score --;
      }
    }
  }

  if (any_pressed) {
    update_game_over();
    update_display();
  }
}


/**
 * Update the display to reflect the current score.
 * If the game is over make the display blink.
 */
void update_display() {
  matrix.writeDigitNum(LEFT_PLAYER_TENS_DIGIT, left_player_score / 10);
  matrix.writeDigitNum(LEFT_PLAYER_ONES_DIGIT, left_player_score % 10);

  matrix.writeDigitNum(RIGHT_PLAYER_TENS_DIGIT, right_player_score / 10);
  matrix.writeDigitNum(RIGHT_PLAYER_ONES_DIGIT, right_player_score % 10);

  matrix.blinkRate(game_over ? BLINK_RATE_FAST : BLINK_RATE_NONE);

  matrix.writeDisplay();
}

/**
 * Determine whether the game is now over.
 *
 * From http://www.bwfbadminton.org/page.aspx?id=14914
 *
 * SCORING SYSTEM
 * 1. A match consists of the best of 3 games of 21 points.
 * 2. Every time there is a serve – there is a point scored.
 * 3. The side winning a rally adds a point to its score.
 * 4. At 20 all, the side which gains a 2 point lead first, wins that game.
 * 5. At 29 all, the side scoring the 30th point, wins that game.
 * 6. The side winning a game serves first in the next game.
 *
 */
void update_game_over() {
  game_over =
    ((left_player_score == 21) && (right_player_score < 20)) ||
    ((right_player_score == 21) && (left_player_score < 20)) ||
    ((left_player_score >= 20) && (right_player_score >= 20) && (abs(left_player_score - right_player_score) == 2)) ||
    (left_player_score == 30) ||
    (right_player_score == 30);
}

