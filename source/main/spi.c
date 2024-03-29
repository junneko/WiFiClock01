/**
*	SPI関係
*	Copyright (c) 2024 Junnichi Tomaru (junneko/ＪＵＮ猫)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

/**
*	SPI初期化
*	 in	なし
*	out	なし
*/
void	spi_initialize( void )
{
	spi_bus_config_t	buscfg = {	/* ピン定義 */
		.miso_io_num = PIN_NUM_MISO,
		.mosi_io_num = PIN_NUM_MOSI,
		.sclk_io_num = PIN_NUM_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 16		// byte
	};
	spi_device_interface_config_t	devcfg = {
		.clock_speed_hz = 20000000,	// Clock out at 20 MHz
		.mode = 0,			// SPI mode 0
		.spics_io_num = PIN_NUM_CS,	// CS pin
		.queue_size = 8,		// 
		.pre_cb = NULL
	};

	// SPIバス初期化
	esperr = spi_bus_initialize( HSPI_HOST, &buscfg, 0 );
	ESP_ERROR_CHECK( esperr );
	// SPIバス接続
	esperr = spi_bus_add_device( HSPI_HOST, &devcfg, &spi );
	ESP_ERROR_CHECK( esperr );
}

/* end of spi.c */
