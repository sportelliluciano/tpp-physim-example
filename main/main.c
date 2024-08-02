/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"

#include "esp_err.h"
#include "esp_log.h"

#include "physim.h"

#include "freertos/FreeRTOS.h"

#define TAG "main"

#define SPI_IN  0x00000001  // arbitrary, defined in basic.py
#define SPI_OUT 0x00000002  // arbitrary, defined in basic.py

void app_main(void)
{
    // Establish connection with simulation controller
    physim_setup();

    uint32_t simulation_device_id = physim_get_config_word(PS_CFG_DEVICE_ID);
    ESP_LOGI(TAG, "I am device %lu", simulation_device_id);

    uint32_t spi_in_link_id = physim_get_config_word(SPI_IN);
    ESP_LOGI(TAG, "SPI_IN link id: %lu", spi_in_link_id);

    uint32_t spi_out_link_id = physim_get_config_word(SPI_OUT);
    ESP_LOGI(TAG, "SPI_OUT link id: %lu", spi_out_link_id);

    for (int i = 0; i < 10; i++) {
        if (simulation_device_id == 0)
        {
            ESP_LOGI(TAG, "Sending 'hello-0' through SPI_OUT");
            physim_link_send(spi_out_link_id, "hello-0\0", 8);
        }
        else
        {
            ESP_LOGI(TAG, "Sending 'hello-1' through SPI_OUT");
            physim_link_send(spi_out_link_id, "hello-1\0", 8);
        }

        ESP_LOGI(TAG, "Waiting for data from SPI_IN");
        uint32_t buffer_len = 0;
        uint8_t *buffer = physim_link_recv(spi_in_link_id, &buffer_len);
        if (buffer) {
            ESP_LOGI(TAG, "Received %lu bytes from SPI_IN: %s", buffer_len, buffer);
            free(buffer);
        } else {
            ESP_LOGW(TAG, "Could not receive from SPI_IN; is the link attached?");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}