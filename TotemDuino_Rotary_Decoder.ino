// ------------------------------------------------------------------------
// Program  : TotemDuino_Rotary_Decoder.ino
// Author   : Gerard Wassink
// Date     : December, 2017
//
// Function : count from 0-255 with an 8 bit counter in LED's
//            use a Rotary Encoder to speed things up or slow them down
//            Used for explanation of the Totem Side Panel 1
//
// ------------------------------------------------------------------------
//            GNU LICENSE CONDITIONS
// ------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// ------------------------------------------------------------------------
//        Copyright (C) 2017 Gerard Wassink
// ------------------------------------------------------------------------

int num = 0;                          // Number will count up (0-255)
int LED[8] = {9,8,7,6,5,4,3,2};       // Pin numbers used for LEDs

unsigned long now = millis();         // Keep track
unsigned long prv = millis();         //   of time
long wait_time = 100;                 // initial speed

int contactA = 16;                    // Input pin Rotate A
int contactB = 17;                    // Input pin Rotate B
int A_val = 0;                        // Input value A
int B_val = 0;                        // Input value B
int cur_val = 3;                      // Current value
int prev_val = 3;                     // Previous value

// -------------------------------------------------------------------------------------
// the setup function runs only once when you press reset or power the board
// -------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);                 // Set up Serial
  Serial.println("TotemDuino_Rotary_Encoder");  // Greet our public
  
  for (int i=0; i<=7; i++) {          // Set
    pinMode(LED[i], OUTPUT);          //   all LEDs 
  } // for                            //    to output
  pinMode(A2, INPUT_PULLUP);          // A2 = input, with pull-up activated
  pinMode(A3, INPUT_PULLUP);          // A3 = input, with pull-up activated
} // setup

// -------------------------------------------------------------------------------------
// the loop function runs over and over again, forever
// -------------------------------------------------------------------------------------
void loop() {
  // ---
  // --- Code for the eight bit binary counter on output LEDs
  // ---
  now = millis();                     // Determine time
  if ((now - prv) >= wait_time) {     // if time passed
    int n = num;                      // work with n to leave num intact
    for (int i=0; i<=7; i++) {        // For all LEDs
      digitalWrite(LED[i], n & 1);    // Put value of rightmost bit to LED
      n >>= 1;                        // Shift right, next bit
    } // for
    if (++num > 255) num = 0;         // Increment and roll over when over 255
    prv += wait_time;                 // Save the time
  } // if
  
  // ---
  // --- Code to handle the Rotary Encoder clicks
  // ---
  A_val = digitalRead(contactA);      // read Rotate A
  B_val = digitalRead(contactB);      // read Rotate B
  cur_val = B_val<<1 | A_val;         // calculate current value
  if (prev_val == 3) {                // previous value 3?
    if (cur_val == 1) {               // clockwise?
      wait_time -= 10;                // speed up (decrease wait time)
      Serial.print("wait_time: ");    //   display new value
      Serial.print(wait_time);        //    of the
      Serial.println("");             //     wait-time
    } else if (cur_val == 2) {        // counter-clockwise?
      wait_time += 10;                // speed down (increase wait time)
      Serial.print("wait_time: ");    //   display new value
      Serial.print(wait_time);        //    of the
      Serial.println("");             //     wait-time
    } // if
    if (wait_time < 0) {              // make sure
      wait_time = 0;                  // wait time doesn't get under 10
    } // if
  } // if
  prev_val = cur_val;                 // save current to previous value
  
} // loop

