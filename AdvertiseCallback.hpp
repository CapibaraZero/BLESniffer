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

#include "PCAP.h"
#include <NimBLEAdvertisedDevice.h>
#include <NimBLEDevice.h>
#include "NimBLEBeacon.h"
#include "NimBLEEddystoneURL.h"
#include "NimBLEEddystoneTLM.h"

#ifndef BLEADVERTISECALLBACK_H
#define BLEADVERTISECALLBACK_H

// Inherit it from BLESniffer
extern PCAP pcap;

#ifdef ARDUINO_NANO_ESP32
#define SERIAL_DEVICE Serial
#else
#define SERIAL_DEVICE Serial0
#endif

class AdvertisedCallback : public BLEAdvertisedDeviceCallbacks {
    private:
#ifndef NO_SERIAL_PRINT_BLESNIFFER
	void print_device(BLEAdvertisedDevice *advertisedDevice);
	inline void print_device_data(String data_type, const char *detail) { 
	    SERIAL_DEVICE.printf("Found %s: %s", data_type, detail);
	};
	inline void print_ibeacon(BLEBeacon beacon);
	inline void print_eddystoneurl_beacon(BLEEddystoneURL beacon, std::string data);
	inline void print_eddystonetlm_beacon(BLEEddystoneTLM beacon, std::string serviceData);
#endif
	void onResult(BLEAdvertisedDevice *advertisedDevice);
	unsigned long int last_save = millis();
	const BLEUUID eddyUUID = (uint16_t)0xfeaa;
	int sniffed = 0;
    public:
	int get_sniffed() { return sniffed; };
	void clear_sniffed() { sniffed = 0; };
};

#endif
