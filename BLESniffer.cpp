/*
 * This file is part of the Capibara zero project(https://capibarazero.github.io/).
 * Copyright (c) 2024 Andrea Canale.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "BLESniffer.hpp"
#include <NimBLEAdvertisedDevice.h>
#include "NimBLEEddystoneURL.h"
#include "NimBLEEddystoneTLM.h"
#include "PCAP.h"

PCAP pcap = PCAP();

BLESniffer::BLESniffer(const char *filename)
{
    pBLEScan = BLEDevice::getScan(); // create new scan
    scan_cb = new AdvertisedCallback();
    pBLEScan->setAdvertisedDeviceCallbacks(scan_cb);
    pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99); // less or equal setInterval value
    pcap.network = 251;      // Bluetooth type
    pcap.filename = filename;
}

BLESniffer::~BLESniffer()
{
    pBLEScan->stop();
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    pcap.closeFile();
}

void BLESniffer::sniff(FS sd)
{
    pcap.openFile(sd);
    BLEScanResults foundDevices = pBLEScan->start(0, false); 
}

void BLESniffer::sniff(FS sd, int scanTime)
{
    pcap.openFile(sd);
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
}

void BLESniffer::clean()
{
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    scan_cb->clear_sniffed();
}

void BLESniffer::stop() {
    pBLEScan->stop();
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    pcap.closeFile();
    sniffing_in_progress = false;
}
