/**
*	IP�A�h���X�Œ�
*	Copyright (c) 2024 Junnichi Tomaru (junneko/�i�t�m�L)
*	Released under the MIT license
*	https://opensource.org/licenses/mit-license.php
*/
#include	"main.h"

static const char	*TAG = "static_ip";

/**
*	DNS�T�[�o�o�^
*	 in	
*	out	�G���[�X�e�[�^�X
*/
esp_err_t	set_dns_server( esp_netif_t *netif, uint32_t addr, esp_netif_dns_type_t type )
{
	if( addr && ( addr != IPADDR_NONE )){
		esp_netif_dns_info_t	dns;

		dns.ip.u_addr.ip4.addr = addr;
		dns.ip.type = IPADDR_TYPE_V4;
		ESP_ERROR_CHECK( esp_netif_set_dns_info( netif, type, &dns ));
	}

	return ESP_OK;
}

/**
*	�X�^�e�B�b�N�A�h���X�ݒ�
*	 in	
*	out	�Ȃ�
*/
void	set_static_ip( esp_netif_t *netif )
{
	esp_netif_ip_info_t	ip;

	if( esp_netif_dhcpc_stop( netif ) != ESP_OK ){	/* DHCP�@�\�X�g�b�v */
		ESP_LOGE( TAG, "Failed to stop dhcp client" );
		return;
	}
	memset( &ip, 0 , sizeof( esp_netif_ip_info_t ));
	ip.ip.addr = ipaddr_addr( staticip );
	ip.netmask.addr = ipaddr_addr( staticnm );
	ip.gw.addr = ipaddr_addr( staticgw );
	if( esp_netif_set_ip_info( netif, &ip ) != ESP_OK ){
		ESP_LOGE( TAG, "Failed to set ip info" );
		return;
	}
	ESP_LOGD( TAG, "Success to set static ip: %s, netmask: %s, gw: %s", staticip, staticnm, staticgw );
	ESP_ERROR_CHECK( set_dns_server( netif, ipaddr_addr( staticdns1 ), ESP_NETIF_DNS_MAIN ));
	ESP_ERROR_CHECK( set_dns_server( netif, ipaddr_addr( staticdns2 ), ESP_NETIF_DNS_BACKUP ));
}

/* end of file */
