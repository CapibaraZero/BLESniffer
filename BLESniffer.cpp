/*
 * This file is part of the Capibara zero project(https://capibarazero.github.io/).
 * Copyright (c) 2023 Andrea Canale.
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
#include "AdvertiseCallback.hpp"
#include "PCAP.hpp"

PCAP pcap = PCAP();

BLESniffer::BLESniffer(const char *filename, FS SD)
{
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); // create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedCallback());
    pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99); // less or equal setInterval value
    pcap.network = 251;      // Bluetooth type
    pcap.filename = filename;
    pcap.openFile(SD);
}

BLESniffer::~BLESniffer()
{
    pBLEScan->stop();
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    pcap.closeFile();
}

void BLESniffer::sniff(int scanTime)
{
    // put your main code here, to run repeatedly:
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial0.print("Devices found: ");
    Serial0.printf("%i", foundDevices.getCount());
    Serial0.println("Scan done!");
}

void BLESniffer::clean()
{
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory 
}

void BLESniffer::stop() {
    pBLEScan->stop();
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    pcap.closeFile(); 
}
