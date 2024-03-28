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

#include "AdvertiseCallback.hpp"
#include <TimeLib.h>

#define CRC24_POLY 0x1864CFB
#define ENDIAN_CHANGE_U16(x) ((((x) & 0xFF00) >> 8) + (((x) & 0xFF) << 8))

// Ibeacon helper
#define IS_IBEACON(manufacturer_data) manufacturer_data.length() == 25 && manufacturer_data[0] == 0x4C && manufacturer_data[1] == 0x00

// Print definitions
#define DEVICE_NAME "device name"
#define SERVICE_UUID "ServiceUUID"

uint32_t calculate_crc24(const uint8_t *data, size_t length)
{
    uint32_t crc = 0x0;

    for (size_t i = 0; i < length; i++)
    {
        crc ^= ((uint32_t)data[i]) << 16;

        for (int j = 0; j < 8; j++)
        {
            crc <<= 1;
            if (crc & 0x1000000)
            {
                crc ^= CRC24_POLY;
            }
        }
    }

    return crc & 0xFFFFFF;
}

/* Callback code from: https://github.com/h2zero/NimBLE-Arduino/blob/release/1.4/examples/BLE_Beacon_Scanner/BLE_Beacon_Scanner.ino */
#ifndef NO_SERIAL_PRINT_BLESNIFFER
inline void AdvertisedCallback::print_ibeacon(BLEBeacon beacon) {
    SERIAL_DEVICE.printf("Found an iBeacon!\n");
    SERIAL_DEVICE.printf("iBeacon Frame\n");
    SERIAL_DEVICE.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", beacon.getManufacturerId(), ENDIAN_CHANGE_U16(beacon.getMajor()), ENDIAN_CHANGE_U16(beacon.getMinor()), beacon.getProximityUUID().toString().c_str(), beacon.getSignalPower());
}

inline void AdvertisedCallback::print_eddystoneurl_beacon(BLEEddystoneURL beacon, std::string data) {
    SERIAL_DEVICE.printf("Found an EddystoneURL beacon!\n");
    SERIAL_DEVICE.printf("EddystoneURL Frame\n");
    SERIAL_DEVICE.println("Found an EddystoneURL beacon!");
    std::string bareURL = beacon.getURL();
    if (bareURL[0] == 0x00)
    {
	SERIAL_DEVICE.printf("DATA-->");
        for (int idx = 0; idx < data.length(); idx++)
        {
	    SERIAL_DEVICE.printf("0x%08X ", data[idx]);
	}
        SERIAL_DEVICE.printf("\nInvalid Data");
        return;
    }
    SERIAL_DEVICE.printf("Found URL: %s\n", beacon.getURL());
    SERIAL_DEVICE.printf("Decoded URL: %s\n", beacon.getDecodedURL());
    SERIAL_DEVICE.printf("TX power: %d\n", beacon.getPower());
}

inline void AdvertisedCallback::print_eddystonetlm_beacon(BLEEddystoneTLM beacon, std::string serviceData) {
    SERIAL_DEVICE.printf("Reported battery voltage: %dmV\n", beacon.getVolt());
    SERIAL_DEVICE.printf("Reported temperature from TLM class: %.2fC\n", (double)beacon.getTemp());
    int temp = (int)serviceData[5] + (int)(serviceData[4] << 8);
    float calcTemp = temp / 256.0f;
    SERIAL_DEVICE.printf("Reported temperature from data: %.2fC\n", calcTemp);
    SERIAL_DEVICE.printf("Reported advertise count: %d\n", beacon.getCount());
    SERIAL_DEVICE.printf("Reported time since last reboot: %ds\n", beacon.getTime());
    SERIAL_DEVICE.printf("%s", beacon.toString()); 
}

void AdvertisedCallback::print_device(BLEAdvertisedDevice *advertisedDevice) {
        if (advertisedDevice->haveName())
        {
           print_device_data(DEVICE_NAME, advertisedDevice->getName().c_str());
        }

        if (advertisedDevice->haveServiceUUID())
        {
            BLEUUID devUUID = advertisedDevice->getServiceUUID(); 
	    print_device_data(SERVICE_UUID, devUUID.toString().c_str());
        }
        
            if (advertisedDevice->haveManufacturerData())
            {
		std::string strManufacturerData = advertisedDevice->getManufacturerData(); 

                if (IS_IBEACON(strManufacturerData))
                {
		    BLEBeacon oBeacon = BLEBeacon();
                    oBeacon.setData(strManufacturerData);
		    print_ibeacon(oBeacon); 
                }
            } 

        if (advertisedDevice->getServiceUUID().equals(eddyUUID))
        {
	    std::string serviceData = advertisedDevice->getServiceData(eddyUUID);
            if (serviceData[0] == 0x10)
            {
                BLEEddystoneURL foundEddyURL = BLEEddystoneURL();
                foundEddyURL.setData(serviceData);
		print_eddystoneurl_beacon(foundEddyURL, serviceData);  
            }
            else if (serviceData[0] == 0x20)
            {
                SERIAL_DEVICE.printf("Found an EddystoneTLM beacon!");
                BLEEddystoneTLM foundEddyURL = BLEEddystoneTLM();
                foundEddyURL.setData(serviceData);
		print_eddystonetlm_beacon(foundEddyURL, serviceData); 
            }
        }

}

#endif

void AdvertisedCallback::onResult(BLEAdvertisedDevice *advertisedDevice) {
        uint32_t timestamp = now();                                         // current timestamp
        uint32_t microseconds = (unsigned int)(micros() - millis() * 1000); // micro seconds offset (0 - 999)
 
        size_t size = advertisedDevice->getPayloadLength() + 11;	// Size of packet

        // Bluetooth packet in network order
        uint8_t packet[size] = {
            0xD6, 0xBE, 0x89, 0x8E, // Access Address 0x8E89BED6 in network order
            0x01,                   // PDU header
            0x00,                   // Length
            0x00, 0x00,             // PDU payload
            0x00, 0x00, 0x00        // CRC
        }; 

	// Fill Adv type of PDU header
        switch (advertisedDevice->getAdvType()) {
        case 0:	// 0000(Indirect advertising)
	    packet[4] >>= 1;
	// Case 1(direct advertising) is default 
	case 2:	// 0100(indirect scan response)
	    packet[4] <<= 2; 
            break;
        case 3:	// 0010(no-connection indirect advertising)
            packet[4] <<= 1;
            break;
	// Case 4(direct advertising) is default(0001)
            break;
        default:
            break;
        }

	packet[5] = advertisedDevice->getPayloadLength();	// Fill payload length
        
	// File PDU payload
	memcpy(&packet[6], advertisedDevice->getPayload(), advertisedDevice->getPayloadLength());
	
	// Calculate CRC and assign it
	uint16_t crc = calculate_crc24(advertisedDevice->getPayload(), advertisedDevice->getPayloadLength());
	packet[size - 3] = crc & 0xff;
	packet[size - 2] = (crc >> 8) & 0xff;
	packet[size - 1] = (crc >> 16) & 0xff;
	sniffed++;
        pcap.newPacketSD(timestamp, microseconds, sizeof(packet), packet);	// Save packet to SD card
	
	// Save file every 2 seconds(2000ms)
	if (millis() - last_save >= 2000)
        {
            pcap.flushFile();
            last_save = millis();
        }

#ifndef NO_SERIAL_PRINT_BLESNIFFER
        SERIAL_DEVICE.printf("%s\n", advertisedDevice->getAddress().toString());
	print_device(advertisedDevice);
#endif
}
