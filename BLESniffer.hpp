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

#include "NimBLEBeacon.h"
#include <NimBLEDevice.h>
#include "AdvertiseCallback.hpp"
#include "FS.h"

#ifndef BLESNIFFER_H
#define BLESNIFFER_H

class BLESniffer
{
private:
    BLEScan *pBLEScan;
    bool sniffing_in_progress = true;
    AdvertisedCallback *scan_cb;
public:
    BLESniffer(const char *filename);
    ~BLESniffer();
    void sniff(FS sd);
    void sniff(FS sd, int scanTime);
    void clean();
    void stop();
    bool is_sniffing() { return sniffing_in_progress; };
    int get_sniffed_packets() { return scan_cb->get_sniffed(); };
};

#endif
