# ESP32 BLE Sniffer

A bluetooth sniffer for ESP32 based on NimBLE-Arduino.

It saves packets to SD card in a PCAP file readable with Wireshark. You can choose filename from code.

It displays when found Ibeacon or eddystone beacon with some additional data.

## Current limitation
- Sometimes packet is malformed
- CRC is wrong