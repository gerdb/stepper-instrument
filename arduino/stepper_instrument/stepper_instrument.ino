/**
 * Project Stepper instrument
 * @author Gerd Bartelt - www.sebulli.com
 *
 * @copyright GPL3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

// define a hystereses to quieten the stepper
#define HYSTERESIS 1


// Stepper motor outputs
int stepper_1A = 2; 
int stepper_2A = 3;
int stepper_1B = 4;
int stepper_2B = 5;

// Reference switch input
int reference_pin = 6;

// Analog voltage input
int analog_in = A0;

// Stepper and position counter
int steper = 0;
int pos = 0;

// Reference value to detect an edge
int ref;
int ref_old;

// The analog value
int analog_value;

// the setup routine
void setup() {  

  // initialize the digital pin as an output.
  pinMode(stepper_1A, OUTPUT);     
  pinMode(stepper_1B, OUTPUT);  
  pinMode(stepper_2A, OUTPUT);  
  pinMode(stepper_2B, OUTPUT); 

  // Reference switch input
  pinMode(reference_pin, INPUT); 

  // Read the reference switch
  ref = digitalRead(reference_pin);
  ref_old = ref;


  // Reference run until an edge is detected
  do {
    steper --;
    update_stepper();

    delay (2); // Stepper speed
    ref_old = ref;
    ref = digitalRead(reference_pin);
  } 
  while (!((ref == 0) && (ref_old == 1)));


}

// The main loop
void loop() {
  int i;

  // Mean 16 values. It acts as a filter
  analog_value = 0;
  for (i = 0; i< 16; i++)
    analog_value += analogRead(analog_in);
  analog_value /= 32;

  // Step left or right ?
  if (analog_value > pos+HYSTERESIS) {
    steper ++;
    pos ++;
  }
  else if (analog_value < pos-HYSTERESIS) {
    steper --;
    pos --;
  }

  update_stepper();


  //delay (5); // not needed, because the analogRead function is so slow.

}

// Control the stepper motor
void update_stepper() {
  switch (steper & 0x03) {
  case 0:
    digitalWrite(stepper_1A, LOW);
    digitalWrite(stepper_1B, HIGH);
    digitalWrite(stepper_2A, LOW);
    digitalWrite(stepper_2B, HIGH);
    break;
  case 1:
    digitalWrite(stepper_1A, HIGH);
    digitalWrite(stepper_1B, LOW);
    digitalWrite(stepper_2A, LOW);
    digitalWrite(stepper_2B, HIGH);
    break;
  case 2:
    digitalWrite(stepper_1A, HIGH);
    digitalWrite(stepper_1B, LOW);
    digitalWrite(stepper_2A, HIGH);
    digitalWrite(stepper_2B, LOW);
    break;
  case 3:
    digitalWrite(stepper_1A, LOW);
    digitalWrite(stepper_1B, HIGH);
    digitalWrite(stepper_2A, HIGH);
    digitalWrite(stepper_2B, LOW);
    break;

  }
}
