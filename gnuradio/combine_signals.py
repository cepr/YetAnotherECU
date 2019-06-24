#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Combine Signals
# Generated: Wed Jun 19 22:28:13 2019
##################################################


from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser


class combine_signals(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Combine Signals")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000

        ##################################################
        # Blocks
        ##################################################
        self.blocks_unpacked_to_packed_xx_0 = blocks.unpacked_to_packed_bb(1, gr.GR_LSB_FIRST)
        self.blocks_stream_mux_0 = blocks.stream_mux(gr.sizeof_char*1, (1, 1, 1, 1, 1, 1, 1, 1))
        self.blocks_null_source_0 = blocks.null_source(gr.sizeof_char*1)
        self.blocks_file_source_2 = blocks.file_source(gr.sizeof_char*1, 'coil2_signal.bin', False)
        self.blocks_file_source_1 = blocks.file_source(gr.sizeof_char*1, 'coil1_signal.bin', False)
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*1, 'trigger_wheel_signal.bin', False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, 'all_signals.bin', False)
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_file_source_0, 0), (self.blocks_stream_mux_0, 0))
        self.connect((self.blocks_file_source_1, 0), (self.blocks_stream_mux_0, 1))
        self.connect((self.blocks_file_source_2, 0), (self.blocks_stream_mux_0, 2))
        self.connect((self.blocks_null_source_0, 0), (self.blocks_stream_mux_0, 3))
        self.connect((self.blocks_null_source_0, 0), (self.blocks_stream_mux_0, 4))
        self.connect((self.blocks_null_source_0, 0), (self.blocks_stream_mux_0, 5))
        self.connect((self.blocks_null_source_0, 0), (self.blocks_stream_mux_0, 6))
        self.connect((self.blocks_null_source_0, 0), (self.blocks_stream_mux_0, 7))
        self.connect((self.blocks_stream_mux_0, 0), (self.blocks_unpacked_to_packed_xx_0, 0))
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.blocks_file_sink_0, 0))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate


def main(top_block_cls=combine_signals, options=None):

    tb = top_block_cls()
    tb.start()
    tb.wait()


if __name__ == '__main__':
    main()
