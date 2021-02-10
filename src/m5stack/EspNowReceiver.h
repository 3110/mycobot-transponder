#ifndef _ESP_NOW_RECEIVER_H
#define _ESP_NOW_RECEIVER_H

#include <esp_now.h>
#include <WiFi.h>

class EspNowReceiver
{
public:
    EspNowReceiver(const uint8_t channel);
    virtual ~EspNowReceiver(void);

    bool begin(esp_now_recv_cb_t cb);

protected:
    void initEspNow(void);
    void initPeer(const uint8_t *addr, esp_now_peer_info_t &peer);

private:
    const uint8_t ESP_NOW_BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN];
    const uint8_t channel;
    esp_now_peer_info_t broadcastPeer;
};
#endif