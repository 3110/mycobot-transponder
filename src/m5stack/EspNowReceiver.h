#ifndef _ESP_NOW_RECEIVER_H
#define _ESP_NOW_RECEIVER_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

extern const uint8_t MYCOBOT_CHANNEL;
extern void onDataRecv(const uint8_t *, const uint8_t *, const int);

class EspNowReceiver
{
public:
    EspNowReceiver(const uint8_t channel);
    virtual ~EspNowReceiver(void);

    bool begin();

protected:
    void initEspNow(void);
    void initPeer(const uint8_t *addr, esp_now_peer_info_t &peer);

private:
    const uint8_t ESP_NOW_BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN];
    const uint8_t channel;
    esp_now_peer_info_t broadcastPeer;
};
#endif