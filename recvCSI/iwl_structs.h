#ifndef __IWL_STRUCTS_H
#define __IWL_STRUCTS_H
#include <stdint.h>

struct iwl_notify
{
	uint64_t tstamp;         /* h/w assigned time stamp */
    
    uint16_t channel;        /* wireless channel (represented in Hz)*/
    uint8_t  bandwidth;      /* channel bandwidth (0->20MHz,1->40MHz)*/

    uint8_t  rate;           /* transmission rate*/
    uint8_t  Nrx;             /* number of receiving antenna*/
    uint8_t  Ntx;             /* number of transmitting antenna*/
    uint8_t  n_tones;         /* number of tones (subcarriers) */
    uint8_t  noise;           /* noise floor (to be updated)*/

    uint8_t  error_code;      /* phy error code (set to 0 if correct)*/

    uint8_t    rssi;          /*  rx frame RSSI */
    uint8_t    rssiA;         /*  rx frame RSSI [ctl, chain 0] */
    uint8_t    rssiB;         /*  rx frame RSSI [ctl, chain 1] */
    uint8_t    rssiC;         /*  rx frame RSSI [ctl, chain 2] */

    uint16_t   packet_len;    /*  packet length (bytes) */
    uint16_t   csi_len;       /*  csi data length (bytes) */
    uint16_t   buf_len;       /*  data length in buffer */
    uint8_t    payload[0];    /*  csi payload           */
};


#endif 
