/**
*	ŠeíƒCƒ“ƒNƒ‹[ƒh
*	Copyright (c) 2024 Junnichi Tomaru (junneko/‚i‚t‚m”L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	<stdio.h>
#include	<string.h>
#include	<inttypes.h>
#include	<time.h>
#include	<sys/time.h>

#include	"freertos/FreeRTOS.h"
#include	"freertos/task.h"
#include	"freertos/event_groups.h"

#include	"esp_system.h"
#include	"esp_mac.h"
#include	"esp_wifi.h"
#include	"esp_netif.h"
#include	"esp_log.h"
#include	"spi_flash_mmap.h"

#include	"esp_event.h"
#include	"nvs_flash.h"

#include	"driver/spi_master.h"
#include	"driver/gpio.h"

#include	"lwip/err.h"
#include	"lwip/sys.h"

#include	"esp_sntp.h"
#include	"esp_http_server.h"

#include	"define.h"
#include	"userdef.h"
#include	"struct_std.h"
#include	"struct_user.h"
#include	"prototype.h"

#include	"gloval.h"

/* end of main.h */
