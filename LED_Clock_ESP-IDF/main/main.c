#include "freertos/FreeRTOS.h"

#include "btstack_port_esp32.h"
#include "btstack_run_loop.h"
#include "hci_dump.h"

#include <stddef.h>

#include "clock.h"
#include "network.h"

#include "esp_log.h"
#include "esp_event.h"
#include "esp_sntp.h"

#include "nvs_flash.h"

#include "freertos/task.h"
#include "freertos/event_groups.h"

static EventGroupHandle_t s_wifi_event_group;

static const char *TAG = "main";

extern int btstack_main(int argc, const char *argv[]);

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Time has been synchronised");
    start_clock_ticking();
}

void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

void obtain_time(void)
{
    initialize_sntp();

    int retry = 0;
    const int retry_count = 10;
    
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) 
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    setup_clock();

    start_clock_ticking();

    // esp_err_t ret = nvs_flash_init();
    
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    // {
    //   ESP_ERROR_CHECK(nvs_flash_erase());
    //   ret = nvs_flash_init();
    // }

    // ESP_ERROR_CHECK(ret);

    // s_wifi_event_group = xEventGroupCreate();

    // start_wifi(s_wifi_event_group);

    // EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    // if (bits & WIFI_CONNECTED_BIT) 
    // {
    //   ESP_LOGI(TAG, "wifi is connected!");

    //   time_t now;
    //   struct tm timeinfo;
    //   time(&now);
    //   localtime_r(&now, &timeinfo);

    //   if (timeinfo.tm_year < (2016 - 1900)) 
    //   {
    //     ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
    //     obtain_time();
    //     time(&now);
    //   }
    // }
    // else if (bits & WIFI_FAIL_BIT) 
    // {

    // } 
    // else 
    // {

    // }

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