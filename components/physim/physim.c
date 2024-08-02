#include "physim.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "esp_log.h"

#include "daemon.h"

#define TAG "physim"

void physim_setup()
{
    physimd_start();

    // Ensure we are connected to the simulation controller
    uint32_t magic = physim_get_config_word(PS_CFG_MAGIC);
    if (magic != PS_MAGIC)
    {
        ESP_LOGE(TAG, "Handshake failed -- aborting");
        abort();
    }

    // Bind links
    uint32_t link_count = physim_get_config_word(PS_CFG_ATTACHED_LINKS_COUNT);
    ESP_LOGI(TAG, "Read PS_CFG_ATTACHED_LINKS_COUNT=%lu", link_count);

    if (link_count > 15) {
        ESP_LOGE(TAG, "Refusing to bind more than 15 links (got %lu)", link_count);
        return;
    }

    if (link_count > 0) {
        channel_t *channels = malloc(sizeof(channel_t) * link_count);
        assert(channels != NULL);

        for (uint32_t i = 0; i < link_count; i++) {
            uint32_t link_id = physim_get_config_word(PS_CFG_ATTACHED_LINK(i));
            physimd_bind_channel(link_id, &channels[i]);
            ESP_LOGI(TAG, "Bound channel %lu", link_id);
        }
    }
}

uint32_t physim_get_config_word(uint32_t word_id)
{
    command_t get_word = {
        .header = {
            .command = PS_CMD_GET_WORD,
            .channel = PS_CHAN_CTRL,
            .payload_size = sizeof(uint32_t)},
        .payload = &word_id};

    physimd_send_command(&get_word);
    physimd_recv_command(PS_CHAN_CTRL, &get_word);

    assert(get_word.header.command == PS_CMD_GET_WORD);
    uint32_t result = *((uint32_t *)get_word.payload);
    free(get_word.payload);
    return result;
}

void physim_link_send(uint16_t link_id, const void *data, uint32_t data_sz)
{
    command_t link_send = {
        .header = {
            .command = PS_CMD_LINK_SEND,
            .channel = link_id,
            .payload_size = data_sz},
        .payload = (void*)data};

    physimd_send_command(&link_send);
}

uint8_t *physim_link_recv(uint16_t link_id, uint32_t *size)
{
    command_t cmd;
    if (!physimd_recv_command(link_id, &cmd))
        return NULL;

    assert(cmd.header.command == PS_CMD_LINK_RECV);
    *size = cmd.header.payload_size;
    return cmd.payload;
}