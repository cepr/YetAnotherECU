#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Trigger Wheel Signal Generator
# Generated: Tue Jun 25 20:08:19 2019
##################################################


from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser


class trigger_wheel_signal_generator(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Trigger Wheel Signal Generator")

        ##################################################
        # Variables
        ##################################################
        self.teeth = teeth = 36
        self.bits = bits = 45

        ##################################################
        # Blocks
        ##################################################
        self.blocks_vector_source_x_0_0 = blocks.vector_source_b([0]*bits+[1]*bits, True, 1, [])
        self.blocks_vector_source_x_0 = blocks.vector_source_b([1], True, 1, [])
        self.blocks_stream_mux_0_0 = blocks.stream_mux(gr.sizeof_char*1, ((teeth-1)*2*bits, 2*bits))
        self.blocks_head_0 = blocks.head(gr.sizeof_char*1, bits*72*100)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, 'trigger_wheel_signal.bin', False)
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_head_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.blocks_stream_mux_0_0, 0), (self.blocks_head_0, 0))
        self.connect((self.blocks_vector_source_x_0, 0), (self.blocks_stream_mux_0_0, 1))
        self.connect((self.blocks_vector_source_x_0_0, 0), (self.blocks_stream_mux_0_0, 0))

    def get_teeth(self):
        return self.teeth

    def set_teeth(self, teeth):
        self.teeth = teeth

    def get_bits(self):
        return self.bits

    def set_bits(self, bits):
        self.bits = bits
        self.blocks_vector_source_x_0_0.set_data([0]*self.bits+[1]*self.bits, [])
        self.blocks_head_0.set_length(self.bits*72*100)


def main(top_block_cls=trigger_wheel_signal_generator, options=None):

    tb = top_block_cls()
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
