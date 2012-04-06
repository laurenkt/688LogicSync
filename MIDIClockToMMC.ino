/*
 
 The circuit:
 * digital in 1 connected to MIDI jack pin 5
 * MIDI jack pin 2 connected to ground
 * MIDI jack pin 4 connected to +5V through 220-ohm resistor
 Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

 */

#include "MIDIClockToMMC.h"

const int deviceId = 127; // Logic only seems to support 127
const long int midiBaudRate = 31250;
const int smpteFramerate = 30;
boolean isPlaying = false;

int tempo = 120;

void setup() {
  Serial.begin(midiBaudRate);
}

void loop() {
  smpte* time;

  // while there is data in queue to be read...
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case MIDISongPositionPointer:
        if (!isPlaying) {
          time = smpteFromSongPosition(readSongPosition());
          mmcGoto(deviceId, time);
          free(time);
        }
        break;
      case MIDIClock:
        if (!isPlaying) {
          isPlaying = true;
          mmcPlay(deviceId);
        }
        break;
      case MIDIStart:
        isPlaying = true;
        mmcStop(deviceId);
        mmcStop(deviceId); // double-stop resets to 0
        mmcPlay(deviceId);
        break;
      case MIDIContinue:
        isPlaying = true;
        mmcPlay(deviceId);
        break;
      case MIDIStop:
        isPlaying = false;
        mmcStop(deviceId);
        break;
    }
  }
}

uint16_t readSongPosition() {
  while (Serial.available() < 2) {}
  return ((uint16_t)Serial.read() << 7) + (uint16_t)Serial.read();
}

smpte* smpteFromSongPosition(uint16_t songPosition) {
  float beatsPer = (songPosition/4.0f)/tempo;
  
  smpte* timeCode = (smpte*)malloc(sizeof(smpte));
  timeCode->mn = (int)(beatsPer);
  timeCode->hr = (int)(beatsPer/60.0f) + 1;
  timeCode->sc = (int)(beatsPer*=60);
  timeCode->fr = (int)(beatsPer*=smpteFramerate);
  timeCode->ff = (int)(beatsPer*100);
  return timeCode;
}

// F0 7F deviceID 06 command F7
void mmcWriteBuffer(int deviceId, int* buffer, size_t length) {
  Serial.write(MIDISysEx);
  Serial.write(SysExRealtime);
  Serial.write(deviceId);
  Serial.write(SysExMMCCommand);
  
  while(length--) {
    Serial.write(*buffer);
    buffer++;
  }
  
  Serial.write(SysExEnd);
}

void mmcWriteByte(int deviceId, int value) {
  mmcWriteBuffer(deviceId, (int[1]){value}, 1);
}

void mmcStop(int deviceId) {
  mmcWriteByte(deviceId, MMCStop);
}

void mmcPlay(int deviceId) {
  mmcWriteByte(deviceId, MMCPlay);
}

void mmcPause(int deviceId) {
  mmcWriteByte(deviceId, MMCPause);
}

// F0 7F deviceID 06 44 06 01 hr mn sc fr ff F7
void mmcGoto(int deviceId, smpte* time) {
  mmcWriteBuffer(deviceId, (int[8]){
    MMCLocate,       // command
    0x06,            // following byte count
    MMCLocateTarget, // TARGET sub-command
    time->hr, time->mn, time->sc, time->fr, time->ff}, 8);
}
