/*
   Based on example code linked below by Arturo Guadalupi, 4/10/15
   
   Sparkfun Pro Micro board was used.
   
   http://www.arduino.cc/en/Tutorial/MidiDevice
*/

#include "MIDIUSB.h"
#include "PitchToNote.h"
#define NUM_BUTTONS  16

//Define pinout of Arduino Pro micro

const uint16_t button1 = 2; // pin number
const uint16_t button2 = 3;
const uint16_t button3 = 4;
const uint16_t button4 = 5;
const uint16_t button5 = 6;
const uint16_t button6 = 7;
const uint16_t button7 = 8;
const uint16_t button8 = 9;
const uint16_t button9 = 10;
const uint16_t button10 = 14; 
const uint16_t button11 = 15; 
const uint16_t button12 = 16; 
const uint16_t button13 = 18; //A0
const uint16_t button14 = 19; //A1
const uint16_t button15 = 20; //A2
const uint16_t button16 = 21; //A3

//note mapping

const uint16_t buttons[NUM_BUTTONS] = {button1, button2, button3, button4, button5, button6, button7, button8, button9, button10, button11, button12, button13, button14, button15, button16};
const byte notePitches[NUM_BUTTONS] = {pitchC2, pitchD2b, pitchD2, pitchE2b, pitchE2, pitchF2, pitchG2b, pitchG2, pitchA2b, pitchA2, pitchB2b, pitchB2, pitchC3, pitchD3b, pitchD3, pitchE3b};

uint16_t notesTime[NUM_BUTTONS];
uint16_t pressedButtons = 0x00;
uint16_t previousButtons = 0x00;
uint16_t intensity;

void setup() {
  for (int i = 0; i < NUM_BUTTONS; i++)
    pinMode(buttons[i], INPUT_PULLUP);
}


void loop() {
  readButtons();
  playNotes();
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void readButtons()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (digitalRead(buttons[i]) == LOW)
    {
      bitWrite(pressedButtons, i, 1);
      delay(2);
    }
    else
      bitWrite(pressedButtons, i, 0);
  }
}

void playNotes()
{
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    if (bitRead(pressedButtons, i) != bitRead(previousButtons, i))
    {
      if (bitRead(pressedButtons, i))
      {
        bitWrite(previousButtons, i , 1);
        noteOn(0, notePitches[i], 100);
        MidiUSB.flush();
      }
      else
      {
        bitWrite(previousButtons, i , 0);
        noteOff(0, notePitches[i], 0);
        MidiUSB.flush();
      }
    }
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
