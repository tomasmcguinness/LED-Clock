#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"

#include "alexa.h"
#include "alexa_gadgets_toolkit.h"

#include "btstack.h"
#include "classic/sdp_util.h"
#include "sdp.h"

#define TAG "LEDCLOCK"

static uint8_t eir_buffer[256];
static uint8_t sdp_buffer_ota[512];
static uint8_t sdp_buffer_data[512];
static uint8_t sdp_buffer_gadget[512];

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    UNUSED(channel);
    UNUSED(size);

    switch (packet_type)
    {
    case HCI_EVENT_PACKET:
        ESP_LOGI(TAG, "Got HCI packet");
        break;

    case RFCOMM_DATA_PACKET:
        ESP_LOGI(TAG, "Got RFCOMM data on channel %d", channel);
        break;

    default:
        ESP_LOGI(TAG, "Unknown packet type: %d", packet_type);
    }
}

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

    l2cap_init();
    rfcomm_init();

    rfcomm_register_service(packet_handler, RFCOMM_DATA_CHANNEL, 0xffff);

    // setup the SDP
    sdp_init();

    memset(sdp_buffer_ota, 0, sizeof(sdp_buffer_ota));
    sdp_create_ota(sdp_buffer_ota, RFCOMM_OTA_HANDLE, RFCOMM_OTA_CHANNEL, RFCOMM_OTA_NAME);
    sdp_register_service(sdp_buffer_ota);

    memset(sdp_buffer_data, 0, sizeof(sdp_buffer_data));
    sdp_create_data(sdp_buffer_data, RFCOMM_DATA_HANDLE, RFCOMM_DATA_CHANNEL, RFCOMM_DATA_NAME);
    sdp_register_service(sdp_buffer_data);

    memset(sdp_buffer_gadget, 0, sizeof(sdp_buffer_gadget));
    sdp_create_gadget(sdp_buffer_gadget, RFCOMM_GADGET_HANDLE, RFCOMM_GADGET_CHANNEL, RFCOMM_GADGET_NAME);
    sdp_register_service(sdp_buffer_gadget);

    // make this device discoverable
    gap_discoverable_control(1);

    // turn on!
    hci_power_control(HCI_POWER_ON);

    return 0;
}
/* LISTING_END */
/* EXAMPLE_END */
