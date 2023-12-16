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

#include "NimBLEBeacon.h"
#include <NimBLEDevice.h>
#include "FS.h"

#ifndef BLESNIFFER_H
#define BLESNIFFER_H

class BLESniffer
{
private:
    BLEScan *pBLEScan;
public:
    BLESniffer(const char *filename, FS SD);
    ~BLESniffer();
    void sniff(int scanTime = 5);
    void clean();
    void stop();
};

#endif
