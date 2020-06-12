#include "freertos/FreeRTOS.h"

#include "btstack_port_esp32.h"
#include "btstack_run_loop.h"
#include "hci_dump.h"

#include <stddef.h>

#include "clock.h"
#include "network.h"

#include "esp_log.h"
#include "esp_event.h"

#include "nvs_flash.h"

#include "freertos/task.h"
#include "freertos/event_groups.h"

static EventGroupHandle_t s_wifi_event_group;

static const char *TAG = "main";

extern int btstack_main(int argc, const char *argv[]);

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    setup_clock();

    s_wifi_event_group = xEventGroupCreate();

    start_wifi(s_wifi_event_group);

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) 
    {
        ESP_LOGI(TAG, "wifi is connected");
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
