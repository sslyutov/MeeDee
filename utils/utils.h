/*\!brief
 * \file utils.h
 * \date January 4, 2025
 * \author Sergey Slyutov
 */
#ifndef _UTILS_H

#define _UTILS_H

#include <cstdint>

namespace ns_midispecs
{
    const short noteoff         = 0x80; // Note Off
    const short noteon          = 0x90; // Note On
    const short polykeypressure = 0xA0; // "Polyphonic Key Pressure
    const short controlchange   = 0xB0; // see table below control change. has 3 bytes. status byte has code of control change 1011 in highest 4 bits. lowest 4 bits is a channel number 0-15
                                        // data byte 1 a control code
                                        // data byte 2 value
    const short programchange   = 0xC0; // Program Change. has 2 bytes. status byte has code of the program change in highest four bits 1100. lowest 4 bits is a channel number 0-15.
                                        // data byte 1 value 0-127 program number
    const short changepressure  = 0xD0; // "Channel Pressure
    const short pitchbend       = 0xE0; // pitch bend

    const uint8_t heartbit      = 0xFE; // system sent heart bit every 300 ms as long as connection is valid

    typedef struct tagmidimsg3b
    {
        uint8_t status; // hight 8 bits are message type, channel.
        uint8_t note; // note 0-127
        uint8_t velocity; // velocity 0-127
    }midimsg3b, *pmidimsg3b;

    typedef struct tagmidimsg3bts
    {
        long long ts_offset; // time offset in nanoseconds from previous note
        midimsg3b midi_msg;  // three bytes midi message
    }midimsg3bts, *pmidimsg3bts;

    typedef struct tagsrcchan
    {
        int src; // source index
        int chan; // channel index on the source, see src index
    }srcchan, *psrcchan;

//  control change - control codes
//  CC Number     Control Name                      Description
//  0           | Bank Select                   | Switches between different banks of sounds.
//  1           | Modulation Wheel              | Typically used for vibrato or other modulation effects.
//  2           | Breath Controller             | Often used with wind controllers.
//  4           | Foot Controller               | Used for effects like volume or wah with a foot pedal.
//  5           | Portamento Time               | Time for portamento (glide between notes).
//  7           | Channel Volume                | Adjusts the overall volume of the channel.
//  8           | Balance                       | Adjusts the balance between left and right.
//  10          | Pan                           | Controls stereo panning (0 = full left, 127 = full right).
//  11          | Expression Controller         | Fine control for volume.
//  64          | Sustain Pedal (On/Off)        | 0-63: Off, 64-127: On.
//  65          | Portamento (On/Off)           | 0-63: Off, 64-127: On.
//  66          | Sostenuto Pedal (On/Off)      | Holds only the notes that are pressed when activated.
//  67          | Soft Pedal (On/Off)           | Makes the notes softer.
//  71          | Resonance (Timbre)            | Adjusts resonance (filter effects).
//  74          | Brightness                    | Adjusts brightness of the sound.
//  84          | Portamento Control            | Specifies the note for portamento.
//  91          | Reverb Level                  | Controls the amount of reverb effect.
//  93          | Chorus Level                  | Controls the amount of chorus effect.
//  120         | All Sound Off                 | Mutes all sound on the channel.
//  121         | Reset All Controllers         | Resets all controllers to their default state.
//  123         | All Notes Off                 | Turns off all notes on the channel. 
};

#endif // _UTILS_H


