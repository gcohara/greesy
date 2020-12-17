#ifndef MIDI_HPP
#define MIDI_HPP

#include "main.hpp"
#include <cstdint>

namespace MIDI {
    enum class InstructionType { NoData, NoteOn, NoteOff };
    struct NoteEventData {
        std::uint8_t note_number;
        std::uint8_t velocity;
    };
    union InstructionData {
        NoteEventData note_data;
        bool no_data;
    };
    struct ParserReturnType {
        InstructionType instruction_type;
        InstructionData instruction_data;
    };

    // Function assumes that midi buffer receives valid and sensical data, e.g
    // every note on byte is followed by two non-status bytes and then a status byte
    ParserReturnType parse_midi_data(MidiBuffer& midi_buffer) noexcept;
}

#endif
