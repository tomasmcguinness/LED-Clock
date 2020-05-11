/*
 * Copyright (C) 2014 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at 
 * contact@bluekitchen-gmbh.com
 *
 */

#define BTSTACK_FILE__ "gap_inquiry.c"

// *****************************************************************************
/* EXAMPLE_START(gap_inquiry): GAP Inquiry Example
 *
 * @text The Generic Access Profile (GAP) defines how Bluetooth devices discover
 * and establish a connection with each other. In this example, the application
 * discovers  surrounding Bluetooth devices and collects their Class of Device
 * (CoD), page scan mode, clock offset, and RSSI. After that, the remote name of
 * each device is requested. In the following section we outline the Bluetooth
 * logic part, i.e., how the packet handler handles the asynchronous events and
 * data packets.
 */
// *****************************************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btstack.h"

static uint8_t eir_buffer[256];

static const uint32_t GADGET_UUID_32 = 0xFE151510;
static const uint16_t AMAZON_SIG_VID = 0x0171;

static uint8_t gadget_uuid[16] = {0x60, 0x88, 0xd2, 0xb3, 0x98, 0x3a, 0x4e, 0xed, 0x9f, 0x94, 0x5a, 0xd1, 0x25, 0x68, 0x16, 0xb7};

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    UNUSED(channel);
    UNUSED(size);

    bd_addr_t addr;
    int i;
    int index;

    if (packet_type != HCI_EVENT_PACKET)
    {
        return;
    }

    uint8_t event = hci_event_packet_get_type(packet);
}

void build_eir(uint8_t *buffer, const char *name, uint16_t pid, uint16_t vid)
{
    uint8_t *ptr = buffer;

    // 0x09 Complete Local Name
    uint8_t name_len = strlen(name);
    *ptr++ = name_len + 1;
    *ptr++ = BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME;
    memcpy(ptr, name, name_len);
    ptr += name_len;

    // 0x06 Incomplete List of 128-bit Service Class UUIDs
    *ptr++ = 1 + 16; // 1 byte for DataType + 128 bit per each uuid
    *ptr++ = BLUETOOTH_DATA_TYPE_INCOMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS;
    for (int8_t i = 15; i >= 0; --i)
    {
        *ptr++ = gadget_uuid[i];
    }

    // 0xFF Manufacturer Specific Data
    *ptr++ = 1 + 10; // 1 byte for DataType + 10 for custom data
    *ptr++ = BLUETOOTH_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA;
    *ptr++ = vid & 0xff;
    *ptr++ = (vid >> 8) & 0xff;
    *ptr++ = pid & 0xff;
    *ptr++ = (pid >> 8) & 0xff;
    *ptr++ = AMAZON_SIG_VID & 0xff;
    *ptr++ = AMAZON_SIG_VID >> 8;
    *ptr++ = GADGET_UUID_32 & 0xff;
    *ptr++ = (GADGET_UUID_32 >> 8) & 0xff;
    *ptr++ = (GADGET_UUID_32 >> 16) & 0xff;
    *ptr++ = (GADGET_UUID_32 >> 24) & 0xff;
}

/* @section Main Application Setup
 *
 * @text Listing MainConfiguration shows main application code.
 * It registers the HCI packet handler and starts the Bluetooth stack.
 */

/* LISTING_START(MainConfiguration): Setup packet handler for GAP inquiry */
int btstack_main(int argc, const char *argv[]);
int btstack_main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;

    // enabled EIR
    hci_set_inquiry_mode(INQUIRY_MODE_RSSI_AND_EIR);

    // build EIR for Alexa Gadget
    memset(eir_buffer, 0, sizeof(eir_buffer));
    build_eir(eir_buffer, "LED CLOCK", 0x0000, 0xffff);
    gap_set_extended_inquiry_response(eir_buffer);

    // get the HCI callback handler
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // make this device discoverable
    gap_discoverable_control(1);

    // turn on!
    hci_power_control(HCI_POWER_ON);

    return 0;
}
/* LISTING_END */
/* EXAMPLE_END */
