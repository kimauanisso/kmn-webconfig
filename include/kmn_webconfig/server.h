#pragma once

// Webconfig Configuration

#ifndef SSID
#define SSID "KMN_Hotspot"
#endif

#ifndef PASSWD
#define PASSWD "12345678"
#endif

#ifndef WEBCONFIG_PORT
#define WEBCONFIG_PORT 8000
#endif

// Methods

/// @brief Starts wifi hotspot
void Hostpot_Init();