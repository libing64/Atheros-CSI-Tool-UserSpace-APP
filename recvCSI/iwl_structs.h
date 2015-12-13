#ifndef __IWL_STRUCTS_H
#define __IWL_STRUCTS_H
#include <stdint.h>

 u_int64_t   tstamp;      /* h/w assigned timestamp */
u_int16_t   csi_len;     /* csi length */
u_int16_t   channel;     /* receiving channel frequency */
u_int8_t    phyerr;      /* phy error code */

u_int8_t    noise;       /* noise floor */
u_int8_t    rate;        /* h/w receive rate index */
u_int8_t    ChanBW;      /* receiving channel bandwidth */
u_int8_t    num_tones;   /* number of tones (subcarriers) */
u_int8_t    nr;          /* number of receiving antennas */
u_int8_t    nc;          /* number of transmitting anteannas */


u_int8_t    rssi;        /* rx frame RSSI */
u_int8_t    rssi_ctl0;   /* rx frame RSSI [ctl, chain 0] */
u_int8_t    rssi_ctl1;   /* rx frame RSSI [ctl, chain 1] */
u_int8_t    rssi_ctl2;   /* rx frame RSSI [ctl, chain 2] */



struct iwl_notify
{
	uint64_t tstamp;         /* h/w assigned time stamp */
    uint16_t csi_len;       /*  csi data length (bytes) */
    uint16_t channel;        /* wireless channel (represented in Hz)*/
    uint8_t  error_code;      /* phy error code (set to 0 if correct)*/
    uint8_t  noise;           /* noise floor (to be updated)*/
    uint8_t  rate;           /* transmission rate*/
    uint8_t  bandwidth;      /* channel bandwidth (0->20MHz,1->40MHz)*/

    uint8_t  n_tones;         /* number of tones (subcarriers) */
    uint8_t  Nrx;             /* number of receiving antenna*/
    uint8_t  Ntx;             /* number of transmitting antenna*/

    uint8_t    rssi;          /*  rx frame RSSI */
    uint8_t    rssiA;         /*  rx frame RSSI [ctl, chain 0] */
    uint8_t    rssiB;         /*  rx frame RSSI [ctl, chain 1] */
    uint8_t    rssiC;         /*  rx frame RSSI [ctl, chain 2] */
    
    ///uint16_t   payload_len;    /*  packet length (bytes) */
    //uint16_t   buf_len;       /*  data length in buffer */
    //uint8_t    payload[0];    /*  csi payload           */
};


#endif 
