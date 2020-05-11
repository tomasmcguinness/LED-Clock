#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"

#define GAP_TAG "GAP"

static const uint32_t GADGET_UUID_32 = 0xFE151510;
static const uint16_t AMAZON_SIG_VID = 0x0171;

static uint8_t gadget_uuid[16] = {0x60, 0x88, 0xd2, 0xb3, 0x98, 0x3a, 0x4e, 0xed, 0x9f, 0x94, 0x5a, 0xd1, 0x25, 0x68, 0x16, 0xb7};
static uint8_t eir_buffer[28];

/*
 * Come from https://developer.amazon.com/en-US/docs/alexa/alexa-gadgets-toolkit/classic-bluetooth-settings.html
 */
void build_eir(uint8_t *buffer, const char *name, uint16_t pid, uint16_t vid)
{
    uint8_t *ptr = buffer;

    // 0x06 Incomplete List of 128-bit Service Class UUIDs
    //
    *ptr++ = 1 + 16; // 1 byte for DataType + 128 bit per each uuid
    *ptr++ = ESP_BT_EIR_TYPE_INCMPL_128BITS_UUID;
    for (int8_t i = 15; i >= 0; --i)
    {
        *ptr++ = gadget_uuid[i];
    }

    // 0xFF Manufacturer Specific Data
    //
    *ptr++ = 1 + 10; // 1 byte for DataType + 10 for custom data
    *ptr++ = ESP_BT_EIR_TYPE_MANU_SPECIFIC;
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

void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event)
    {
    case ESP_BT_GAP_CONFIG_EIR_DATA_EVT:
        ESP_LOGI(GAP_TAG, "EIR Configuration updated: %d", event);
        break;
    case ESP_BT_GAP_CFM_REQ_EVT:
        ESP_LOGI(GAP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", param->cfm_req.num_val);
        esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        ESP_LOGI(GAP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
        break;
    case ESP_BT_GAP_KEY_REQ_EVT:
        ESP_LOGI(GAP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;
    case ESP_BT_GAP_RMT_SRVC_REC_EVT:
    default:
    {
        ESP_LOGI(GAP_TAG, "event: %d", event);
        break;
    }
    }

    return;
}

void bt_app_gap_start_up(void)
{
    ESP_LOGI(GAP_TAG, "Building EIR...");

    memset(eir_buffer, 0, sizeof(eir_buffer));

    build_eir(eir_buffer, "LED CLOCK", 0x0000, 0xffff);

    /* Configure the EIR for Alexa Gadget */
    esp_bt_eir_data_t eir_data = {0};

    eir_data.flag = ESP_BT_EIR_FLAG_GEN_DISC;
    eir_data.manufacturer_len = sizeof(eir_buffer);
    eir_data.p_manufacturer_data = eir_buffer;

    // ESP_LOGI(GAP_TAG, "Setting EIR...");
    // esp_err_t ret = esp_bt_gap_config_eir_data(&eir_data);

    // if (ret != ESP_OK)
    // {
    //     ESP_LOGE(GAP_TAG, "%s failed to set EIR data: %s\n", __func__, esp_err_to_name(ret));
    // }

    ESP_LOGI(GAP_TAG, "Setting Name...");

    char *dev_name = "LED CLOCK";
    esp_bt_dev_set_device_name(dev_name);

    ESP_LOGI(GAP_TAG, "Making discoverable...");

    /* set discoverable and connectable mode, wait to be connected */
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

    /* register GAP callback function */
    esp_bt_gap_register_callback(bt_app_gap_cb);

    /* set the EIR data for the gadget */
    esp_bt_gap_config_eir_data(&eir_data);

    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_NONE;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));

    /* start to discover nearby Bluetooth devices */
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}

void app_main()
{
    ESP_LOGI(GAP_TAG, "Starting bluetooth...!");

    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK)
    {
        ESP_LOGE(GAP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)

    {
        ESP_LOGE(GAP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK)
    {
        ESP_LOGE(GAP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK)
    {
        ESP_LOGE(GAP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    bt_app_gap_start_up();

    ESP_LOGI(GAP_TAG, "Ready!");
}