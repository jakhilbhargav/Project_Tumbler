/**
 * Copyright (c) 2015 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "sdk_common.h"
#if NRF_MODULE_ENABLED(ANT_BSC)

#include "ant_bsc_page_0.h"
#include "ant_bsc_utils.h"

#define NRF_LOG_MODULE_NAME ant_bcs_page_0
#if ANT_BSC_PAGE_0_LOG_ENABLED
#define NRF_LOG_LEVEL       ANT_BSC_PAGE_0_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  ANT_BSC_PAGE_0_INFO_COLOR
#else // ANT_BSC_PAGE_0_LOG_ENABLED
#define NRF_LOG_LEVEL       0
#endif // ANT_BSC_PAGE_0_LOG_ENABLED
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

/**@brief BSC page 0 data layout structure. */
typedef struct
{
    uint8_t reserved[3];
    uint8_t bsc_evt_time_LSB;
    uint8_t bsc_evt_time_MSB;
    uint8_t bsc_rev_count_LSB;
    uint8_t bsc_rev_count_MSB;
}ant_bsc_page0_data_layout_t;

/**@brief Function for printing speed or cadence page0 data. */
static void page0_data_log(ant_bsc_page0_data_t const * p_page_data)
{
    NRF_LOG_INFO("Revolution count:          %u", (unsigned int)p_page_data->rev_count);

    NRF_LOG_INFO("BSC event time:            %u.%03us",
              (unsigned int)ANT_BSC_EVENT_TIME_SEC(p_page_data->event_time),
              (unsigned int)ANT_BSC_EVENT_TIME_MSEC(p_page_data->event_time));

//    NRF_LOG_INFO("%03us", (unsigned int)ANT_BSC_EVENT_TIME_MSEC(p_page_data->event_time));
}

void ant_bsc_page_0_encode(uint8_t * p_page_buffer, ant_bsc_page0_data_t const * p_page_data)
{
    ant_bsc_page0_data_layout_t * p_outcoming_data = (ant_bsc_page0_data_layout_t *) p_page_buffer;
    uint16_t                      event_time       = p_page_data->event_time;
    uint16_t                      rev_count        = p_page_data->rev_count;

    p_outcoming_data->reserved[0]       = UINT8_MAX;
    p_outcoming_data->reserved[1]       = UINT8_MAX;
    p_outcoming_data->reserved[2]       = UINT8_MAX;
    p_outcoming_data->bsc_evt_time_LSB  = (uint8_t)(event_time & UINT8_MAX);
    p_outcoming_data->bsc_evt_time_MSB  = (uint8_t)((event_time >> 8) & UINT8_MAX);
    p_outcoming_data->bsc_rev_count_LSB = (uint8_t)(rev_count & UINT8_MAX);
    p_outcoming_data->bsc_rev_count_MSB = (uint8_t)((rev_count >> 8) & UINT8_MAX);

    page0_data_log(p_page_data);
}

void ant_bsc_page_0_decode(uint8_t const * p_page_buffer, ant_bsc_page0_data_t * p_page_data)
{
    ant_bsc_page0_data_layout_t const * p_incoming_data = (ant_bsc_page0_data_layout_t *)p_page_buffer;

    uint16_t event_time = (uint16_t)((p_incoming_data->bsc_evt_time_MSB << 8)
                          + p_incoming_data->bsc_evt_time_LSB);

    uint16_t revolution_count = (uint16_t) ((p_incoming_data->bsc_rev_count_MSB << 8)
                                + p_incoming_data->bsc_rev_count_LSB);

    p_page_data->event_time = event_time;
    p_page_data->rev_count  = revolution_count;

    page0_data_log(p_page_data);
}

#endif // NRF_MODULE_ENABLED(ANT_BSC)
