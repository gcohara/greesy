#include "catch.hpp"
#include "../inc/synth.hpp"
#include "../inc/main.hpp"
#include "../inc/midi.hpp"
#include "../inc/peripherals/usart.hpp"
#include <cstdint>

TEST_CASE( "USART enable first test" ) {
    USART::USART<USART::USARTNumber::UnitTest> test_usart;
    REQUIRE(USART::unit_test_registers[0] == 0);
    
    USART::Config test_config{
        .word_len{ USART::WordLength::Nine },
        .stop_bits{ USART::StopBits::OneAndHalf },
        .baud_rate{ 31250 },
        .receive_enable{ true },
        .receive_DMA{ true },
    };
    test_usart.enable(test_config);
    REQUIRE(USART::unit_test_registers[0] == 0x00001005);
    REQUIRE(USART::unit_test_registers[1] == 0x00003000);
    REQUIRE(USART::unit_test_registers[2] == 0x00000040);
    REQUIRE(USART::unit_test_registers[3] == 2304);   
}

TEST_CASE( "Circular buffer works correctly") {
    CircularBuffer<int, 4> cirbuf;
    REQUIRE(cirbuf.buffer_empty() == true);
    REQUIRE(cirbuf.data_loss == false);
    for (auto x : {0, 1, 2, 3}){
        cirbuf.write(x);
    }
    REQUIRE(cirbuf.buffer_empty() == false);
    REQUIRE(cirbuf.data_loss == false);
    REQUIRE(cirbuf.element_count() == 4);
    cirbuf.write(4);
    REQUIRE(cirbuf.data_loss == true);
    REQUIRE(cirbuf.element_count() == 4);
    REQUIRE(cirbuf.read() == 4);
    REQUIRE(cirbuf.element_count() == 3);
    REQUIRE(cirbuf.read() == 1);
    REQUIRE(cirbuf.element_count() == 2);
    REQUIRE(cirbuf.read() == 2);
    REQUIRE(cirbuf.element_count() == 1);
    REQUIRE(cirbuf.read() == 3);
    REQUIRE(cirbuf.element_count() == 0);
    REQUIRE(cirbuf.buffer_empty() == true);
    REQUIRE(cirbuf.data_loss == true);
    REQUIRE(cirbuf.element_count() == 0);
}

TEST_CASE("MIDI parser working") {
    CircularBuffer<std::uint8_t, midi_buffer_size> fake_midi;
    REQUIRE(MIDI::parse_midi_data(fake_midi).instruction_type == MIDI::InstructionType::NoData);
    REQUIRE(MIDI::parse_midi_data(fake_midi).instruction_data.no_data == true);

    fake_midi.write(0x90);
    fake_midi.write(0x45);
    fake_midi.write(0x34);
    auto note_on_test{ MIDI::parse_midi_data(fake_midi) };
    REQUIRE(note_on_test.instruction_type == MIDI::InstructionType::NoteOn);
    REQUIRE(note_on_test.instruction_data.note_data.note_number == 0x45);
    REQUIRE(note_on_test.instruction_data.note_data.velocity == 0x34);

    fake_midi.write(0x80);
    fake_midi.write(0x79);
    fake_midi.write(0x67);
    auto note_off_test{ MIDI::parse_midi_data(fake_midi) };
    REQUIRE(note_off_test.instruction_type == MIDI::InstructionType::NoteOff);
    REQUIRE(note_off_test.instruction_data.note_data.note_number == 0x79);
    REQUIRE(note_off_test.instruction_data.note_data.velocity == 0x67);
    
}
