#include <Arduino.h>
#include "BLESniffer.hpp"

void setup()
{
    Serial.begin(9600);
}

BLESniffer ble_sniffer = BLESniffer("/bluetooth.pcap", get_current_fs());
void loop()
{
    ble_sniffer.sniff(60);
    delay(5000);
}