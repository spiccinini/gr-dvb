/*
 * Copyright (c) 2010 Edmund Tse
 *               2015 Santiago Piccinini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "rs_encoder_pp_impl.h"

namespace gr {
  namespace dvb {

    rs_encoder_pp::sptr
    rs_encoder_pp::make()
    {
      return gnuradio::get_initial_sptr
        (new rs_encoder_pp_impl());
    }

    /*
     * The private constructor
     */
    rs_encoder_pp_impl::rs_encoder_pp_impl()
      : gr::sync_block("rs_encoder_pp",
              gr::io_signature::make(1, 1, sizeof(mpeg_ts_packet)),
              gr::io_signature::make(1, 1, sizeof(dvb_packet_rs_encoded)))
    {}

    /*
     * Our virtual destructor.
     */
    rs_encoder_pp_impl::~rs_encoder_pp_impl()
    {
    }

    int
    rs_encoder_pp_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const mpeg_ts_packet *in = (const mpeg_ts_packet *) input_items[0];
        dvb_packet_rs_encoded *out = (dvb_packet_rs_encoded *) output_items[0];

        for (int i = 0; i < noutput_items; ++i)
        {
            // Check sync byte
            assert(in[i].data[0] == MPEG_TS_SYNC_BYTE || in[i].data[0] == MPEG_TS_SYNC_BYTE_INV);

            reed_solomon.encode(out[i], in[i]);
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dvb */
} /* namespace gr */

