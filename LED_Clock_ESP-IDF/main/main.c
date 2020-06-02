#include "btstack_port_esp32.h"
#include "btstack_run_loop.h"
#include "hci_dump.h"

#include <stddef.h>

#include "clock.h"
#include "network.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static EventGroupHandle_t s_wifi_event_group;

extern int btstack_main(int argc, const char *argv[]);

void app_main(void)
{
    setup_clock();

    start_wifi(s_wifi_event_group);

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) 
    {
        
    }
    else if (bits & WIFI_FAIL_BIT) 
    {

    } 
    else 
    {

    }

    //ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    //ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    //vEventGroupDelete(s_wifi_event_group);

    // Configure BTstack for ESP32 VHCI Controller
    //btstack_init();

    // Setup example
    //btstack_main(0, NULL);

    // Enter run loop (forever)
    //btstack_run_loop_execute();
}
