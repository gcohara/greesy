#include "../inc/midi.hpp"
#include <cstdint>

namespace {
    bool is_status_byte(std::uint8_t byte) noexcept {
        return (byte & 0x80);
    }

    bool is_note_on(std::uint8_t byte) noexcept {
        return (byte == 0x90);
    }
    
    bool is_note_off(std::uint8_t byte) noexcept {
        return (byte == 0x80);
    }
}

namespace MIDI {
    ParserReturnType
    parse_midi_data
    (CircularBuffer<std::uint8_t, midi_buffer_size>& midi_buffer) noexcept {
        ParserReturnType output;
        // If the do fits, wear it
        std::uint8_t first_byte;
        do {
            if (midi_buffer.buffer_empty()) {
                output.instruction_type = InstructionType::NoData;
                output.instruction_data.no_data = true;
                break;
            }
            first_byte = midi_buffer.read();
        } while (!is_status_byte(first_byte));

        if (is_note_on(first_byte)) {
            output.instruction_type = InstructionType::NoteOn;
            output.instruction_data.note_data.note_number = midi_buffer.read();
            output.instruction_data.note_data.velocity = midi_buffer.read();
        }
        else if (is_note_off(first_byte)) {
            output.instruction_type = InstructionType::NoteOff;
            output.instruction_data.note_data.note_number = midi_buffer.read();
            output.instruction_data.note_data.velocity = midi_buffer.read();
        }
        return output;
    }
}
