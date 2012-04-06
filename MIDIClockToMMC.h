#include <stdlib.h>
#include <stdint.h>

enum MIDI {
    MIDISysEx               = 0xF0,
    MIDISongPositionPointer = 0xF2,
    MIDIClock               = 0xF8,
    MIDITick                = 0xF9,
    MIDIStart               = 0xFA,
    MIDIContinue            = 0xFB,
    MIDIStop                = 0xFC
};

enum SysEx {
   SysExEnd         = 0xF7,
   SysExNonRealtime = 0x7E,
   SysExRealtime    = 0x7F,
   SysExMMCCommand  = 0x06,
   SysExMMCResponse = 0x07
};

enum MMC {
    MMCStop         = 0x01,
    MMCPlay         = 0x02,
    MMCDeferredPlay = 0x03,
    MMCFastForward  = 0x04,
    MMCRewind       = 0x05,
    MMCRecordStrobe = 0x06,
    MMCRecordExit   = 0x07,
    MMCRecordReady  = 0x08,
    MMCPause        = 0x09,
    MMCEject        = 0x0A,
    MMCChase        = 0x0B,
    MMCReset        = 0x0D,
    MMCWrite        = 0x40,
    MMCLocate       = 0x44,
    MMCLocateTarget = 0x01  // sub-command
};

typedef struct {
  int hr;
  int mn;
  int sc;
  int fr;
  int ff;
} smpte;

