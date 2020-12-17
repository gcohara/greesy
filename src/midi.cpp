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

    // This is gross and hacky. Surely there's a better way?
    void wait_for_next_byte(MidiBuffer& midi_buffer) noexcept {
        while(midi_buffer.buffer_empty());
    }
}

namespace MIDI {
    ParserReturnType parse_midi_data(MidiBuffer& midi_buffer) noexcept {
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
            // Need to wait for the next value in buffer
            wait_for_next_byte(midi_buffer);
            output.instruction_data.note_data.note_number = midi_buffer.read();
            wait_for_next_byte(midi_buffer);
            output.instruction_data.note_data.velocity = midi_buffer.read();
        }
        else if (is_note_off(first_byte)) {
            output.instruction_type = InstructionType::NoteOff;
            wait_for_next_byte(midi_buffer);
            output.instruction_data.note_data.note_number = midi_buffer.read();
            wait_for_next_byte(midi_buffer);
            output.instruction_data.note_data.velocity = midi_buffer.read();
        }
        return output;
    }
}
