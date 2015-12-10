/*
 * =====================================================================================
 *       Filename:  sendData.c 
 *
 *    Description:  send packets 
 *        Version:  1.0
 *
 *         Author:  Yaxiong Xie 
 *         Email :  <xieyaxiongfly@gmail.com>
 *   Organization:  WANS group @ Nanyang Technological University 
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * =====================================================================================
 */
 
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <unistd.h>
#include <errno.h>

/* Define the defult destination MAC address */
char default_mac_addr[] = "00:03:7F:B0:20:20";
 
#define DEFAULT_IF	        "wlan0"
#define BUF_SIZ	            2048	
 
int main(int argc, char *argv[])
{
	int     sockfd;
	struct  ifreq if_idx;
	struct  ifreq if_mac;
	char    buf[BUF_SIZ];
	unsigned int dst_addr[6];
	struct  ether_header *eh = (struct ether_header *) buf;
	struct  iphdr *iph = (struct iphdr *) (buf + sizeof(struct ether_header));
	struct  sockaddr_ll socket_address;
	char    ifname[IFNAMSIZ];
	
    if (argc == 1)
    {
        printf("Usage:   %s ifname dst_mac_address packet_len count\n", argv[0]);
        printf("Example: %s wlan0 00:7F:5D:3E:4A 100 1000\n", argv[0]);
        exit(0);
    }

	/* Get interface name */
	if (argc > 1)
		strcpy(ifname, argv[1]);
	else
		strcpy(ifname, DEFAULT_IF);

    //dst address seperated by :, example: 00:7F:5D:3E:4A
    if(argc > 2)
    {
        sscanf(argv[2], "%x:%x:%x:%x:%x:%x", &dst_addr[0], &dst_addr[1], &dst_addr[2], &dst_addr[3], &dst_addr[4], &dst_addr[5]);
    }else
    {
       sscanf(default_mac_addr, "%x:%x:%x:%x:%x:%x", &dst_addr[0], &dst_addr[1], &dst_addr[2], &dst_addr[3], &dst_addr[4], &dst_addr[5]);
    }

    int packet_len = 10;
    if(argc > 3)
       packet_len  = atoi(argv[3]);
    else
        packet_len = 10;
	
	int count = 10;
	if(argc > 4)
	{
		count = atoi(argv[4]);
	}

	int interval = 100;
	if(argc > 5)
	{
		interval = atoi(argv[5]);
	}
 	
	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) 
	{
	    perror("socket");
	}
 
	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifname, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifname, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");
 
	/* Construct the Ethernet header */
	memset(buf, 0, BUF_SIZ);
	/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = dst_addr[0];
	eh->ether_dhost[1] = dst_addr[1];
	eh->ether_dhost[2] = dst_addr[2];
	eh->ether_dhost[3] = dst_addr[3];
	eh->ether_dhost[4] = dst_addr[4];
	eh->ether_dhost[5] = dst_addr[5];

    /* Ethertype field */
	eh->ether_type = htons(ETH_P_IP);

	int header_len = sizeof(struct ether_header);
	int tx_len = header_len  + packet_len;
 	
	/* Packet data 
     * We just set it to 0xaa you send arbitrary payload you like*/
    for(int i = 1; i < packet_len; i++)
    {    
	    buf[header_len + i] = i % 255;
    } 
    printf("packet packet_len: %d, count: %d\n", packet_len, count); 
	
    /* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
    /* RAW communication*/
    socket_address.sll_family   = PF_PACKET;    
    /* we don't use a protocoll above ethernet layer
     *   ->just use anything here*/
    socket_address.sll_protocol = htons(ETH_P_IP);  
    
    /* ARP hardware identifier is ethernet*/
    socket_address.sll_hatype   = ARPHRD_ETHER;
        
    /* target is another host*/
    socket_address.sll_pkttype  = PACKET_OTHERHOST;
    
    /* address packet_len*/
    socket_address.sll_halen    = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = dst_addr[0];
	socket_address.sll_addr[1] = dst_addr[1];
	socket_address.sll_addr[2] = dst_addr[2];
	socket_address.sll_addr[3] = dst_addr[3];
	socket_address.sll_addr[4] = dst_addr[4];
	socket_address.sll_addr[5] = dst_addr[5];
 
	/* Send packet */
    while( count-- )
    {
        
        if (usleep(interval) == -1)
        {
            printf("sleep failed\n");
        }

        if ( sendto(sockfd, buf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
        {
            printf("Send failed\n");
        }else
        {
        	printf("%d packets left\n", count);
        }
    }
	
	return 0;
}
