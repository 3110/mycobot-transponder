#include "m5stack/EspNowReceiver.h"

EspNowReceiver::EspNowReceiver(const uint8_t channel) : ESP_NOW_BROADCAST_ADDRESS{
                                                            0xFF,
                                                            0xFF,
                                                            0xFF,
                                                            0xFF,
                                                            0xFF,
                                                            0xFF,
                                                        },
                                                        channel(channel)
{
}

EspNowReceiver::~EspNowReceiver(void)
{
}

bool EspNowReceiver::begin(esp_now_recv_cb_t cb)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    initEspNow();
    initPeer(ESP_NOW_BROADCAST_ADDRESS, broadcastPeer);
    if (esp_now_add_peer(&broadcastPeer) != ESP_OK)
    {
        return false;
    }
    if (esp_now_register_recv_cb(cb) != ESP_OK)
    {
        return false;
    }
    return true;
}

void EspNowReceiver::initEspNow(void)
{
    if (esp_now_init() != ESP_OK)
    {
        ESP.restart();
    }
}

void EspNowReceiver::initPeer(const uint8_t *addr, esp_now_peer_info_t &peer)
{
    memset(&peer, 0, sizeof(peer));
    memcpy(&(peer.peer_addr), addr, ESP_NOW_ETH_ALEN);
    peer.channel = this->channel;
    peer.encrypt = 0;
}