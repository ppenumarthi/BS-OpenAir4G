/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2014 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is
   included in this distribution in the file called "COPYING". If not,
   see <http://www.gnu.org/licenses/>.

  Contact Information
  OpenAirInterface Admin: openair_admin@eurecom.fr
  OpenAirInterface Tech : openair_tech@eurecom.fr
  OpenAirInterface Dev  : openair4g-devel@eurecom.fr

  Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06904 Biot Sophia Antipolis cedex, FRANCE

*******************************************************************************/

/*
                                enb_app.c
                             -------------------
  AUTHOR  : Laurent Winckel, Sebastien ROUX, Lionel GAUTHIER, Navid Nikaein
  COMPANY : EURECOM
  EMAIL   : Lionel.Gauthier@eurecom.fr and Navid Nikaein
*/

#include <string.h>
#include <stdio.h>

#include "enb_app.h"
#include "enb_config.h"
#include "assertions.h"

#include "log.h"
#if defined(OAI_EMU)
# include "OCG.h"
# include "OCG_extern.h"
#endif

#if defined(ENABLE_ITTI)
# include "intertask_interface.h"
# include "timer.h"
# if defined(ENABLE_USE_MME)
#   include "s1ap_eNB.h"
#   include "sctp_eNB_task.h"
#   include "gtpv1u_eNB_task.h"
# endif

/*Added by nfvepc for generqating sgi traffic 
//#include <tasks_def.h> */
//#include "../COMMON/gtpv1_u_messages_def.h"  

# include "intertask_interface.h"
/*#include <../../common/utils/itti/intertask_interface_init.h>  
#include <../COMMON/gtpv1_u_messages_types.h>  
//#include <../COMMON/gtpv1_u_messages_def.h>  
#include <../../openair-cn/COMMON/gtpv1_u_messages_types.h>  
//#include <thread> */
#include <sched.h>
extern unsigned char NB_eNB_INST;
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//#include "../../openair-cn/COMMON/gtpv1_u_messages_types.h"
#include <time.h>

#if defined(ENABLE_ITTI)

/*------------------------------------------------------------------------------*/
# if defined(ENABLE_USE_MME)
#   define ENB_REGISTER_RETRY_DELAY 10
# endif

#define P_GTPV1_MESSAGE_TYPE 19 //Stopped using later, need to clean up
#define SGW_IP_ADDRESS "192.168.1.2" 

typedef enum { false, true } bool;
	static int sock;
	static struct sockaddr_in server;
	static bool connected = false;

	char mmeip_address[20] = "130.245.144.31";
	char message[1000];
	MessageDef *msg_pt           = NULL;
	unsigned char *message_payload_p = NULL; 
	unsigned char data_pP[50] = "testing for gtpv1 data in tunneled packets";  
	int packet_sizeP = sizeof("testing for gtpv1 data in tunneled packets");
	gtpv1u_enb_tunnel_data_req_t *gtpv1u_tunnel_data_req_p;

/*--------------------------------------------------------------------*/
int a = 1;
static void create_tunnel(int i)
{
	if(a != 1) 
		return;
       	LOG_I(ENB_APP, "In create_tunnel function\n");
	//char *SGW_IP_ADDRESS = "192.168.1.2";
	MessageDef *msg_p           = NULL;

        //gtpv1u_enb_tunnel_data_req_t *gtpv1u_tunnel_data_req_p;
        gtpv1u_enb_create_tunnel_req_t *create_tunnel_req_pP;

        msg_p = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_ENB_CREATE_TUNNEL_REQ);
        //msg_p = itti_alloc_new_message(TASK_ENB_APP, UDP_DATA_IND);

	msg_p->ittiMsgHeader.messageId = GTPV1U_ENB_CREATE_TUNNEL_REQ; //UDP_DATA_IND;
        msg_p->ittiMsgHeader.originTaskId = TASK_ENB_APP;
        msg_p->ittiMsgHeader.destinationTaskId = TASK_GTPV1_U;
        msg_p->ittiMsgHeader.ittiMsgSize= 42;
        

        create_tunnel_req_pP = &msg_p->ittiMsg.Gtpv1uCreateTunnelReq;
	create_tunnel_req_pP->num_tunnels = 2;
        create_tunnel_req_pP->ue_index = 1;

	//create_tunnel_req_pP->sgw_addr[i].buffer = { ntohl(SGW_IP_ADDRESS), ntohl(SGW_IP_ADDRESS) >> 8, ntohl(SGW_IP_ADDRESS) >> 16, ntohl(SGW_IP_ADDRESS) >> 24 };
//	create_tunnel_req_pP->sgw_addr[i].buffer =  (uint8 *)SGW_IP_ADDRESS);
	/*create_tunnel_req_pP->sgw_addr[i].buffer[0] =  htonl(SGW_IP_ADDRESS);
	create_tunnel_req_pP->sgw_addr[i].buffer[1] =  htonl(SGW_IP_ADDRESS) >> 8;
	create_tunnel_req_pP->sgw_addr[i].buffer[2] =  htonl(SGW_IP_ADDRESS) >> 16;
	create_tunnel_req_pP->sgw_addr[i].buffer[3] =  htonl(SGW_IP_ADDRESS) >> 24; */

	char arr[] = "192.168.1.2"; 
	sscanf(arr, "%hu.%hu.%hu.%hu", &create_tunnel_req_pP->sgw_addr[i].buffer[0], &create_tunnel_req_pP->sgw_addr[i].buffer[1], &create_tunnel_req_pP->sgw_addr[i].buffer[2], &create_tunnel_req_pP->sgw_addr[i].buffer[3]);
       	LOG_I(ENB_APP, "create_tunnel_req_pP->sgw_addr[i].buffer[0]: %d \n",create_tunnel_req_pP->sgw_addr[i].buffer[0]);
       	LOG_I(ENB_APP, "create_tunnel_req_pP->sgw_addr[i].buffer[1]: %d \n",create_tunnel_req_pP->sgw_addr[i].buffer[1]);
       	LOG_I(ENB_APP, "create_tunnel_req_pP->sgw_addr[i].buffer[2]: %d \n",create_tunnel_req_pP->sgw_addr[i].buffer[2]);
       	LOG_I(ENB_APP, "create_tunnel_req_pP->sgw_addr[i].buffer[3]: %d \n",create_tunnel_req_pP->sgw_addr[i].buffer[3]);

	//char arr[] = "192.168.1.2"; 
	//sscanf(arr, "%hu.%hu.%hu.%hu", &create_tunnel_req_pP->sgw_addr[i].buffer[0], &create_tunnel_req_pP->sgw_addr[i].buffer[1], &create_tunnel_req_pP->sgw_addr[i].buffer[2], &create_tunnel_req_pP->sgw_addr[i].buffer[3]);

/*	memcpy(create_tunnel_req_pP->sgw_addr[i].buffer,
                     "192.168.1.2",
                      sizeof (in_addr_t)); */
	//create_tunnel_req_pP->sgw_addr[i].length = 8  * sizeof (in_addr_t); 
	create_tunnel_req_pP->sgw_addr[i].length = 8 * 4; // NFVEPC Project Reportedly, this value is not being set correctly only when given in multiples of 16. Hence, a fi exists in gtpv1u_enb.c to keep this value as 32 since we plan to use IPv4 only.
	//create_tunnel_req_pP->sgw_addr[i].length = strlen(arr); -- currently setting a constant: ff:ff:ff:ff -- ffffffff. Hence 8
	//create_tunnel_req_pP->sgw_addr[i].length = 8; -- should be in bits; so  should be 8*4 = 32
	//create_tunnel_req_pP->sgw_addr[i].length = 32; //looks liked . is also included in string
	//create_tunnel_req_pP->sgw_addr[i].length = 56; 
	//create_tunnel_req_pP->sgw_addr[i].length = 128; 

       	LOG_I(ENB_APP, "creating itti message to TASK_GTPV1U\n");
	itti_send_msg_to_task(TASK_GTPV1_U, INSTANCE_DEFAULT, msg_p);//TASK_FW_IP =3;;; TASK_GTPV1U=2 or 11
	a = 100;
}

//int create_tunnel_sample_variable =0;
static void connect_to_send_msg()
{
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		LOG_I(ENB_APP,"Could not create socket");
	}
	LOG_I(ENB_APP,"Server Socket created");
     
	//server.sin_addr.s_addr = inet_addr("130.245.144.31");
	server.sin_addr.s_addr = inet_addr(mmeip_address);
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
 
	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		LOG_I(ENB_APP,"connect failed. Error");
        	return 1;
	}
     
	LOG_I(ENB_APP,"Connected to EPC module\n");
	connected = true;

	message_payload_p = (char *)malloc(50); 
        msg_pt = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_ENB_TUNNEL_DATA_REQ);

	msg_pt->ittiMsgHeader.messageId = GTPV1U_ENB_TUNNEL_DATA_REQ;
        msg_pt->ittiMsgHeader.originTaskId = TASK_ENB_APP;
        msg_pt->ittiMsgHeader.destinationTaskId = TASK_GTPV1_U;
        msg_pt->ittiMsgHeader.ittiMsgSize = packet_sizeP;

        message_payload_p = itti_malloc(TASK_ENB_APP,TASK_GTPV1_U, packet_sizeP);
        memcpy(message_payload_p, data_pP, packet_sizeP);
        gtpv1u_tunnel_data_req_p = &msg_pt->ittiMsg.Gtpv1uTunnelDataReq;
	gtpv1u_tunnel_data_req_p->rab_id = 1;
        gtpv1u_tunnel_data_req_p->ue_index = 1;
        gtpv1u_tunnel_data_req_p->length = packet_sizeP;
        gtpv1u_tunnel_data_req_p->buffer = message_payload_p;
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->rabid: %d\n",msg_pt->ittiMsg.Gtpv1uTunnelDataReq.rab_id);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->ue_index: %d\n",msg_pt->ittiMsg.Gtpv1uTunnelDataReq.ue_index);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->length: %d",msg_pt->ittiMsg.Gtpv1uTunnelDataReq.length);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->buffer: %s\n",msg_pt->ittiMsg.Gtpv1uTunnelDataReq.buffer);
}

static void send_msg_added()
{
	if(!connected)
		connect_to_send_msg();	

	int countNum = 0;
/*	MessageDef *msg_p           = NULL;
	unsigned char *message_payload_p = NULL; 
	message_payload_p = (char *)malloc(50); 
	unsigned char data_pP[50] = "testing for gtpv1 data in tunneled packets";  
	int packet_sizeP = sizeof("testing for gtpv1 data in tunneled packets");

	gtpv1u_enb_tunnel_data_req_t *gtpv1u_tunnel_data_req_p;

        msg_p = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_ENB_TUNNEL_DATA_REQ);

	msg_p->ittiMsgHeader.messageId = GTPV1U_ENB_TUNNEL_DATA_REQ;
        msg_p->ittiMsgHeader.originTaskId = TASK_ENB_APP;
        msg_p->ittiMsgHeader.destinationTaskId = TASK_GTPV1_U;
        msg_p->ittiMsgHeader.ittiMsgSize = packet_sizeP;

        message_payload_p = itti_malloc(TASK_ENB_APP,TASK_GTPV1_U, packet_sizeP);
        memcpy(message_payload_p, data_pP, packet_sizeP);
        gtpv1u_tunnel_data_req_p = &msg_p->ittiMsg.Gtpv1uTunnelDataReq;
	gtpv1u_tunnel_data_req_p->rab_id = 1;
        gtpv1u_tunnel_data_req_p->ue_index = 1;
        gtpv1u_tunnel_data_req_p->length = packet_sizeP;
        gtpv1u_tunnel_data_req_p->buffer = message_payload_p;
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->rabid: %d\n",msg_p->ittiMsg.Gtpv1uTunnelDataReq.rab_id);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->ue_index: %d\n",msg_p->ittiMsg.Gtpv1uTunnelDataReq.ue_index);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->length: %d",msg_p->ittiMsg.Gtpv1uTunnelDataReq.length);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->buffer: %s\n",msg_p->ittiMsg.Gtpv1uTunnelDataReq.buffer);

/*
********* NFVEPC Project *************
*/	

	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);              
	LOG_I(ENB_APP, "sending at %.6lf \n", t1 );
//	n = write(sockfd,(void *)msg_p,sizeof(MessageDef)); 		
	//if( send(sock , message , strlen(message) , 0) < 0)
	if( send(sock , (void *)msg_pt, sizeof(MessageDef) , 0) < 0)
	{
        	LOG_I(ENB_APP,"Send failed");
        	return 1;
	}

        //itti_free (ITTI_MSG_ORIGIN_ID(msg_p), msg_p);  
	gettimeofday(&tim, NULL);
	t1=tim.tv_sec+(tim.tv_usec/1000000.0);              
	//LOG_I(ENB_APP, "SEND %.6lf %s %d %ld\n", t1, "GTPV1U_ENB_TUNNEL_DATA_REQ", sizeof(*msg_p), ++countNum );
	LOG_I(ENB_APP, "SEND %.6lf %s %d %ld\n", t1, "GTPV1U_ENB_TUNNEL_DATA_REQ", sizeof(message), ++countNum );
         
        //close(sock);
	
	return;
}



static void send_msg_added1()
{
	long countNum =0;
	struct sockaddr_in serverAddr;
	int sockfd, portno, n;
	socklen_t addr_size;
	portno = 2152;

	//sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
 	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(portno);                                        
	/* Set IP address to localhost */                                           
	serverAddr.sin_addr.s_addr = inet_addr("130.245.144.31");                   
	/* Set all bits of the padding field to 0 */                           
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	/*---- Connect the socket to the server using the address struct ----*/
	addr_size = sizeof serverAddr;

	if (connect(sockfd,(struct sockaddr *)&serverAddr,addr_size) < 0)
        	LOG_I(ENB_APP, "ERROR connecting to server socket\n");
	
	MessageDef *msg_p           = NULL;
	unsigned char *message_payload_p = NULL; 
	message_payload_p = (char *)malloc(50); 
	unsigned char data_pP[50] = "testing for gtpv1 data in tunneled packets";  
	int packet_sizeP = sizeof("testing for gtpv1 data in tunneled packets");

	gtpv1u_enb_tunnel_data_req_t *gtpv1u_tunnel_data_req_p;

        msg_p = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_ENB_TUNNEL_DATA_REQ);

	msg_p->ittiMsgHeader.messageId = GTPV1U_ENB_TUNNEL_DATA_REQ;
        msg_p->ittiMsgHeader.originTaskId = TASK_ENB_APP;
        msg_p->ittiMsgHeader.destinationTaskId = TASK_GTPV1_U;
        msg_p->ittiMsgHeader.ittiMsgSize = packet_sizeP;

        message_payload_p = itti_malloc(TASK_ENB_APP,TASK_GTPV1_U, packet_sizeP);
        memcpy(message_payload_p, data_pP, packet_sizeP);
        gtpv1u_tunnel_data_req_p = &msg_p->ittiMsg.Gtpv1uTunnelDataReq;
	gtpv1u_tunnel_data_req_p->rab_id = 1;
        gtpv1u_tunnel_data_req_p->ue_index = 1;
        gtpv1u_tunnel_data_req_p->length = packet_sizeP;
        gtpv1u_tunnel_data_req_p->buffer = message_payload_p;
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->rabid: %d\n",msg_p->ittiMsg.Gtpv1uTunnelDataReq.rab_id);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->ue_index: %d\n",msg_p->ittiMsg.Gtpv1uTunnelDataReq.ue_index);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->length: %d",msg_p->ittiMsg.Gtpv1uTunnelDataReq.length);	
        LOG_I(ENB_APP, "gtpv1u_tunnel_data_req_p->buffer: %s\n",msg_p->ittiMsg.Gtpv1uTunnelDataReq.buffer);	

/*
********* NFVEPC Project *************
*/	

	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);              
	LOG_I(ENB_APP, "SEND %.6lf %s %d %ld\n", t1, "GTPV1U_ENB_TUNNEL_DATA_REQ", sizeof(*msg_p), ++countNum );
	n = write(sockfd,(void *)msg_p,sizeof(MessageDef)); 		
	if (n < 0)
		LOG_I(ENB_APP, "ERROR writing to socket\n");	
	
	return;
}
/*--------------------------------------------------------------------*/




/*------------------------------------------------------------------------------*/
static void configure_phy(uint32_t enb_id, const Enb_properties_array_t *enb_properties)
{
    MessageDef *msg_p;
    int CC_id;

    msg_p = itti_alloc_new_message (TASK_ENB_APP, PHY_CONFIGURATION_REQ);
    for (CC_id=0;CC_id<MAX_NUM_CCs;CC_id++) {
      PHY_CONFIGURATION_REQ (msg_p).frame_type[CC_id]              = enb_properties->properties[enb_id]->frame_type[CC_id];
      PHY_CONFIGURATION_REQ (msg_p).prefix_type[CC_id]             = enb_properties->properties[enb_id]->prefix_type[CC_id];
      PHY_CONFIGURATION_REQ (msg_p).downlink_frequency[CC_id]      = enb_properties->properties[enb_id]->downlink_frequency[CC_id];
      PHY_CONFIGURATION_REQ (msg_p).uplink_frequency_offset[CC_id] = enb_properties->properties[enb_id]->uplink_frequency_offset[CC_id];
      PHY_CONFIGURATION_REQ (msg_p).nb_antennas_tx[CC_id]          = enb_properties->properties[enb_id]->nb_antennas_tx[CC_id];
      PHY_CONFIGURATION_REQ (msg_p).nb_antennas_rx[CC_id]          = enb_properties->properties[enb_id]->nb_antennas_rx[CC_id];
      PHY_CONFIGURATION_REQ (msg_p).tx_gain[CC_id]                 = enb_properties->properties[enb_id]->tx_gain[CC_id];
      PHY_CONFIGURATION_REQ (msg_p).rx_gain[CC_id]                 = enb_properties->properties[enb_id]->rx_gain[CC_id];
    }
    itti_send_msg_to_task (TASK_PHY_ENB, enb_id, msg_p);
}

/*------------------------------------------------------------------------------*/
static void configure_rrc(uint32_t enb_id, const Enb_properties_array_t *enb_properties)
{
    MessageDef *msg_p = NULL;
    int CC_id;

    msg_p = itti_alloc_new_message (TASK_ENB_APP, RRC_CONFIGURATION_REQ);

    RRC_CONFIGURATION_REQ (msg_p).cell_identity =   enb_properties->properties[enb_id]->eNB_id;
    RRC_CONFIGURATION_REQ (msg_p).tac =             enb_properties->properties[enb_id]->tac;
    RRC_CONFIGURATION_REQ (msg_p).mcc =             enb_properties->properties[enb_id]->mcc;
    RRC_CONFIGURATION_REQ (msg_p).mnc =             enb_properties->properties[enb_id]->mnc;
    RRC_CONFIGURATION_REQ (msg_p).mnc_digit_length = enb_properties->properties[enb_id]->mnc_digit_length;
    for (CC_id=0;CC_id<MAX_NUM_CCs;CC_id++) {
      RRC_CONFIGURATION_REQ (msg_p).frame_type[CC_id]                               = enb_properties->properties[enb_id]->frame_type[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).tdd_config[CC_id]                               = enb_properties->properties[enb_id]->tdd_config[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).tdd_config_s[CC_id]                             = enb_properties->properties[enb_id]->tdd_config_s[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).eutra_band[CC_id]                               = enb_properties->properties[enb_id]->eutra_band[CC_id];

      // RACH-Config
      RRC_CONFIGURATION_REQ (msg_p).rach_numberOfRA_Preambles[CC_id]                = enb_properties->properties[enb_id]->rach_numberOfRA_Preambles[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_preamblesGroupAConfig[CC_id]               = enb_properties->properties[enb_id]->rach_preamblesGroupAConfig[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_sizeOfRA_PreamblesGroupA[CC_id]            = enb_properties->properties[enb_id]->rach_sizeOfRA_PreamblesGroupA[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_messageSizeGroupA[CC_id]                   = enb_properties->properties[enb_id]->rach_messageSizeGroupA[CC_id]; 
      RRC_CONFIGURATION_REQ (msg_p).rach_messagePowerOffsetGroupB[CC_id]            = enb_properties->properties[enb_id]->rach_messagePowerOffsetGroupB[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_powerRampingStep[CC_id]                    = enb_properties->properties[enb_id]->rach_powerRampingStep[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_preambleInitialReceivedTargetPower[CC_id]  = enb_properties->properties[enb_id]->rach_preambleInitialReceivedTargetPower[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_preambleTransMax[CC_id]                    = enb_properties->properties[enb_id]->rach_preambleTransMax[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_raResponseWindowSize[CC_id]                = enb_properties->properties[enb_id]->rach_raResponseWindowSize[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_macContentionResolutionTimer[CC_id]        = enb_properties->properties[enb_id]->rach_macContentionResolutionTimer[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).rach_maxHARQ_Msg3Tx[CC_id]                      = enb_properties->properties[enb_id]->rach_maxHARQ_Msg3Tx[CC_id];

  // BCCH-Config
      RRC_CONFIGURATION_REQ (msg_p).bcch_modificationPeriodCoeff[CC_id]             = enb_properties->properties[enb_id]->bcch_modificationPeriodCoeff[CC_id];

  // PCCH-Config
      RRC_CONFIGURATION_REQ (msg_p).pcch_defaultPagingCycle[CC_id]                  = enb_properties->properties[enb_id]->pcch_defaultPagingCycle[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pcch_nB[CC_id]                                  = enb_properties->properties[enb_id]->pcch_nB[CC_id];

  // PRACH-Config
      RRC_CONFIGURATION_REQ (msg_p).prach_root[CC_id]                               = enb_properties->properties[enb_id]->prach_root[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).prach_config_index[CC_id]                       = enb_properties->properties[enb_id]->prach_config_index[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).prach_high_speed[CC_id]                         = enb_properties->properties[enb_id]->prach_high_speed[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).prach_zero_correlation[CC_id]                   = enb_properties->properties[enb_id]->prach_zero_correlation[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).prach_freq_offset[CC_id]                        = enb_properties->properties[enb_id]->prach_freq_offset[CC_id];

  // PDSCH-Config
      RRC_CONFIGURATION_REQ (msg_p).pdsch_referenceSignalPower[CC_id]               = enb_properties->properties[enb_id]->pdsch_referenceSignalPower[CC_id];  
      RRC_CONFIGURATION_REQ (msg_p).pdsch_p_b[CC_id]                                = enb_properties->properties[enb_id]->pdsch_p_b[CC_id];  

  // PUSCH-Config
      RRC_CONFIGURATION_REQ (msg_p).pusch_n_SB[CC_id]                               = enb_properties->properties[enb_id]->pusch_n_SB[CC_id];  
      RRC_CONFIGURATION_REQ (msg_p).pusch_hoppingMode[CC_id]                        = enb_properties->properties[enb_id]->pusch_hoppingMode[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pusch_hoppingOffset[CC_id]                      = enb_properties->properties[enb_id]->pusch_hoppingOffset[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pusch_enable64QAM[CC_id]                        = enb_properties->properties[enb_id]->pusch_enable64QAM[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pusch_groupHoppingEnabled[CC_id]                = enb_properties->properties[enb_id]->pusch_groupHoppingEnabled[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pusch_groupAssignment[CC_id]                    = enb_properties->properties[enb_id]->pusch_groupAssignment[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pusch_sequenceHoppingEnabled[CC_id]             = enb_properties->properties[enb_id]->pusch_sequenceHoppingEnabled[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pusch_nDMRS1[CC_id]                             = enb_properties->properties[enb_id]->pusch_nDMRS1[CC_id];

  // PUCCH-Config

      RRC_CONFIGURATION_REQ (msg_p).pucch_delta_shift[CC_id]                        = enb_properties->properties[enb_id]->pucch_delta_shift[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pucch_nRB_CQI[CC_id]                            = enb_properties->properties[enb_id]->pucch_nRB_CQI[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pucch_nCS_AN[CC_id]                             = enb_properties->properties[enb_id]->pucch_nCS_AN[CC_id];
#ifndef Rel10
      RRC_CONFIGURATION_REQ (msg_p).pucch_n1_AN[CC_id]                              = enb_properties->properties[enb_id]->pucch_n1_AN[CC_id];
#endif

  // SRS Config
      RRC_CONFIGURATION_REQ (msg_p).srs_enable[CC_id]                               = enb_properties->properties[enb_id]->srs_enable[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).srs_BandwidthConfig[CC_id]                      = enb_properties->properties[enb_id]->srs_BandwidthConfig[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).srs_SubframeConfig[CC_id]                       = enb_properties->properties[enb_id]->srs_SubframeConfig[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).srs_ackNackST[CC_id]                            = enb_properties->properties[enb_id]->srs_ackNackST[CC_id]; 
      RRC_CONFIGURATION_REQ (msg_p).srs_MaxUpPts[CC_id]                             = enb_properties->properties[enb_id]->srs_MaxUpPts[CC_id];

  // uplinkPowerControlCommon

      RRC_CONFIGURATION_REQ (msg_p).pusch_p0_Nominal[CC_id]                         = enb_properties->properties[enb_id]->pusch_p0_Nominal[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pucch_p0_Nominal[CC_id]                         = enb_properties->properties[enb_id]->pucch_p0_Nominal[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pusch_alpha[CC_id]                              = enb_properties->properties[enb_id]->pusch_alpha[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pucch_deltaF_Format1[CC_id]                     = enb_properties->properties[enb_id]->pucch_deltaF_Format1[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pucch_deltaF_Format1b[CC_id]                    = enb_properties->properties[enb_id]->pucch_deltaF_Format1b[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pucch_deltaF_Format2[CC_id]                     = enb_properties->properties[enb_id]->pucch_deltaF_Format2[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).pucch_deltaF_Format2a[CC_id]                    = enb_properties->properties[enb_id]->pucch_deltaF_Format2a[CC_id];    
      RRC_CONFIGURATION_REQ (msg_p).pucch_deltaF_Format2b[CC_id]                    = enb_properties->properties[enb_id]->pucch_deltaF_Format2b[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).msg3_delta_Preamble[CC_id]                      = enb_properties->properties[enb_id]->msg3_delta_Preamble[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).ul_CyclicPrefixLength[CC_id]                    = enb_properties->properties[enb_id]->ul_CyclicPrefixLength[CC_id];

  // UE Timers and Constants

      RRC_CONFIGURATION_REQ (msg_p).ue_TimersAndConstants_t300[CC_id]               = enb_properties->properties[enb_id]->ue_TimersAndConstants_t300[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).ue_TimersAndConstants_t301[CC_id]               = enb_properties->properties[enb_id]->ue_TimersAndConstants_t301[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).ue_TimersAndConstants_t310[CC_id]               = enb_properties->properties[enb_id]->ue_TimersAndConstants_t310[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).ue_TimersAndConstants_n310[CC_id]               = enb_properties->properties[enb_id]->ue_TimersAndConstants_n310[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).ue_TimersAndConstants_t311[CC_id]               = enb_properties->properties[enb_id]->ue_TimersAndConstants_t311[CC_id];
      RRC_CONFIGURATION_REQ (msg_p).ue_TimersAndConstants_n311[CC_id]               = enb_properties->properties[enb_id]->ue_TimersAndConstants_n311[CC_id];
    }
    itti_send_msg_to_task (TASK_RRC_ENB, enb_id, msg_p);
}

/*------------------------------------------------------------------------------*/
# if defined(ENABLE_USE_MME)
static uint32_t eNB_app_register(uint32_t enb_id_start, uint32_t enb_id_end, const Enb_properties_array_t *enb_properties)
{
    uint32_t         enb_id;
    uint32_t         mme_id;
    MessageDef      *msg_p;
    uint32_t         register_enb_pending = 0;
    char            *str                  = NULL;
    struct in_addr   addr;

#   if defined(OAI_EMU)

#   endif

    for (enb_id = enb_id_start; (enb_id < enb_id_end) ; enb_id++)
    {
#   if defined(OAI_EMU)
        if (oai_emulation.info.cli_start_enb[enb_id] == 1)
#   endif
        {
            s1ap_register_enb_req_t *s1ap_register_eNB;

            /* note:  there is an implicit relationship between the data structure and the message name */
            msg_p = itti_alloc_new_message (TASK_ENB_APP, S1AP_REGISTER_ENB_REQ);

            s1ap_register_eNB = &S1AP_REGISTER_ENB_REQ(msg_p);

            /* Some default/random parameters */
            s1ap_register_eNB->eNB_id           = enb_properties->properties[enb_id]->eNB_id;
            s1ap_register_eNB->cell_type        = enb_properties->properties[enb_id]->cell_type;
            s1ap_register_eNB->eNB_name         = enb_properties->properties[enb_id]->eNB_name;
            s1ap_register_eNB->tac              = enb_properties->properties[enb_id]->tac;
            s1ap_register_eNB->mcc              = enb_properties->properties[enb_id]->mcc;
            s1ap_register_eNB->mnc              = enb_properties->properties[enb_id]->mnc;
            s1ap_register_eNB->mnc_digit_length = enb_properties->properties[enb_id]->mnc_digit_length;
            s1ap_register_eNB->default_drx      = enb_properties->properties[enb_id]->pcch_defaultPagingCycle[0];

            s1ap_register_eNB->nb_mme =         enb_properties->properties[enb_id]->nb_mme;
            AssertFatal (s1ap_register_eNB->nb_mme <= S1AP_MAX_NB_MME_IP_ADDRESS, "Too many MME for eNB %d (%d/%d)!", enb_id, s1ap_register_eNB->nb_mme, S1AP_MAX_NB_MME_IP_ADDRESS);

            for (mme_id = 0; mme_id < s1ap_register_eNB->nb_mme; mme_id++)
            {
                s1ap_register_eNB->mme_ip_address[mme_id].ipv4 = enb_properties->properties[enb_id]->mme_ip_address[mme_id].ipv4;
                s1ap_register_eNB->mme_ip_address[mme_id].ipv6 = enb_properties->properties[enb_id]->mme_ip_address[mme_id].ipv6;
                strncpy (s1ap_register_eNB->mme_ip_address[mme_id].ipv4_address,
                         enb_properties->properties[enb_id]->mme_ip_address[mme_id].ipv4_address,
                         sizeof(s1ap_register_eNB->mme_ip_address[0].ipv4_address));
                strncpy (s1ap_register_eNB->mme_ip_address[mme_id].ipv6_address,
                         enb_properties->properties[enb_id]->mme_ip_address[mme_id].ipv6_address,
                         sizeof(s1ap_register_eNB->mme_ip_address[0].ipv6_address));
		// -- NFVEPC Specific Modification
                strncpy (mmeip_address, s1ap_register_eNB->mme_ip_address[mme_id].ipv4_address,
//                         enb_properties->properties[enb_id]->mme_ip_address[mme_id].ipv4_address,
                         sizeof(s1ap_register_eNB->mme_ip_address[0].ipv4_address));
            }

            s1ap_register_eNB->enb_ip_address.ipv6 = 0;
            s1ap_register_eNB->enb_ip_address.ipv4 = 1;
            addr.s_addr = enb_properties->properties[enb_id]->enb_ipv4_address_for_S1_MME;
            str = inet_ntoa(addr);
            strcpy(s1ap_register_eNB->enb_ip_address.ipv4_address, str);

            itti_send_msg_to_task (TASK_S1AP, enb_id, msg_p);

            register_enb_pending++;
        }
    }
    
    return register_enb_pending;
}
# endif
#endif

/* Additions by nfvepc starts */
void *generate_sgi_traffic(void *args_p)
{
    LOG_I(ENB_APP, "Entered into the generate_sgi_traffic function\n");
    long statistic_timer_id;
    //yield();
    //pthread_yield();
    //usleep(1);
    //LOG_I(ENB_APP, "Exited the sleep\n");
    //sched_yield(); //--provides executable, but does not yield processor to next threads.
/*    while(1) 
    {
	MessageDef *msg_p           = NULL;
	msg_p = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_TUNNEL_DATA_IND);
	itti_send_msg_to_task (TASK_FW_IP, INSTANCE_DEFAULT, msg_p);
        result = itti_free (ITTI_MSG_ORIGIN_ID(msg_p), msg_p);
    } */

  /* sigset_t mask;
	sigset_t orig_mask;
	struct timespec timeout;
 
	sigemptyset (&mask);
//	sigaddset (&mask, SIGCHLD);
 
	if (sigprocmask(SIG_BLOCK, &mask, &orig_mask) < 0) {
		perror ("sigprocmask");
		return 1;
	}
 
 
	timeout.tv_sec = 5;
	timeout.tv_nsec = 0;
 
	sigtimedwait(&mask, NULL, &timeout);
    LOG_I(ENB_APP, "Done with sigtimedwait\n");  */
       
/*    while(1) 
    {
	MessageDef *msg_p           = NULL;
	msg_p = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_TUNNEL_DATA_IND);
	itti_send_msg_to_task (TASK_GTPV1_U, INSTANCE_DEFAULT, msg_p);
        result = itti_free (ITTI_MSG_ORIGIN_ID(msg_p), msg_p);
    } */


     /* Request for periodic timer */
    statistic_timer_id = 0x12345678910;
        //statistic_timer_id = 10;
    //if (timer_setup(5, 10, TASK_ENB_APP, INSTANCE_DEFAULT,TIMER_PERIODIC, NULL, &statistic_timer_id) < 0)
    //if (timer_setup(0, 50, TASK_ENB_APP, INSTANCE_DEFAULT,TIMER_PERIODIC, NULL, &statistic_timer_id) < 0)
    if (timer_setup(0, 500, TASK_ENB_APP, INSTANCE_DEFAULT,TIMER_PERIODIC, NULL, &statistic_timer_id) < 0)
    {
        LOG_I(ENB_APP,"Failed to request new timer for statistics with %ds "
        "of periocidity\n", statistic_timer_id);
        statistic_timer_id = 0;
    }

    //itti_exit_task ();
    return NULL;
}
/* Additions by nfvepc ends */

/*------------------------------------------------------------------------------*/
void *eNB_app_task(void *args_p)
{
    const Enb_properties_array_t   *enb_properties_p  = NULL;
#if defined(ENABLE_ITTI)
    uint32_t                        enb_nb = 1; /* Default number of eNB is 1 */
    uint32_t                        enb_id_start = 0;
    uint32_t                        enb_id_end = enb_id_start + enb_nb;
# if defined(ENABLE_USE_MME)
    uint32_t                        register_enb_pending;
    uint32_t                        registered_enb;
    long                            enb_register_retry_timer_id;
# endif
    uint32_t                        enb_id;
    MessageDef                     *msg_p           = NULL;
    const char                     *msg_name        = NULL;
    instance_t                      instance;
    int                             result;

    itti_mark_task_ready (TASK_ENB_APP);

# if defined(ENABLE_USE_MME)
#   if defined(OAI_EMU)
    enb_nb =        oai_emulation.info.nb_enb_local;
    enb_id_start =  oai_emulation.info.first_enb_local;
    enb_id_end =    oai_emulation.info.first_enb_local + enb_nb;

    AssertFatal (enb_id_end <= NUMBER_OF_eNB_MAX,
                 "Last eNB index is greater or equal to maximum eNB index (%d/%d)!",
                 enb_id_end, NUMBER_OF_eNB_MAX);

//New timer created to generate traffic at BS periodically -- NFVEPC Project
    long statistic_timer_id = 0x12345678910;
    //if (timer_setup(5, 5, TASK_ENB_APP, INSTANCE_DEFAULT,TIMER_PERIODIC, NULL, &statistic_timer_id) < 0)
    //if (timer_setup(0, 2500, TASK_ENB_APP, INSTANCE_DEFAULT,TIMER_PERIODIC, NULL, &statistic_timer_id) < 0)
    if (timer_setup(5, 0, TASK_ENB_APP, INSTANCE_DEFAULT,TIMER_PERIODIC, NULL, &statistic_timer_id) < 0)
    //if (timer_setup(0,150000, TASK_ENB_APP, INSTANCE_DEFAULT,TIMER_PERIODIC, NULL, &statistic_timer_id) < 0)
    {
      LOG_I(ENB_APP,"Failed to request new timer for statistics with %ds of periocidity\n", statistic_timer_id);
      statistic_timer_id = 0;
    }
#   endif
# endif

    enb_properties_p = enb_config_get();

    AssertFatal (enb_nb <= enb_properties_p->number,
                 "Number of eNB is greater than eNB defined in configuration file (%d/%d)!",
                 enb_nb, enb_properties_p->number);

    for (enb_id = enb_id_start; (enb_id < enb_id_end) ; enb_id++)
    {
        configure_phy(enb_id, enb_properties_p);
        configure_rrc(enb_id, enb_properties_p);
    }

# if defined(ENABLE_USE_MME)
    /* Try to register each eNB */
    registered_enb = 0;
    register_enb_pending = eNB_app_register (enb_id_start, enb_id_end, enb_properties_p);
# else
    /* Start L2L1 task */
    msg_p = itti_alloc_new_message(TASK_ENB_APP, INITIALIZE_MESSAGE);
    itti_send_msg_to_task(TASK_L2L1, INSTANCE_DEFAULT, msg_p);
# endif

    do
    {
        // Wait for a message
        itti_receive_msg (TASK_ENB_APP, &msg_p);

/*
 *  * Only for statistics 
 *   * NFVEPC Project
 *    */
  /*time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );*/
struct timeval tim;
gettimeofday(&tim, NULL);
double t1=tim.tv_sec+(tim.tv_usec/1000000.0);              

/*  LOG_I(ENB_APP, "-------------------------------\n" );
  LOG_I(ENB_APP, "Current local time and date: %.6lf \n", t1 );
  LOG_I(ENB_APP, "Received message as %d with size as: %d\n", ITTI_MSG_ID(msg_p), sizeof( *msg_p) );
  LOG_I(ENB_APP, "TIMER_HAS_EXPIRED: %d, S1AP_REGISTER_ENB_CNF: %d,\n", TIMER_HAS_EXPIRED, S1AP_REGISTER_ENB_CNF); */
  if(ITTI_MSG_ID(msg_p) == 11)
  LOG_I(ENB_APP, "ABCD %.6lf %s %d", t1, "TIMER_HAS_EXPIRED", sizeof(*msg_p) );
  else if(ITTI_MSG_ID(msg_p) == 102)
  LOG_I(ENB_APP, "ABCD %.6lf %s %d", t1, "S1AP_REGISTER_ENB_CNF", sizeof(*msg_p) );
//  LOG_I(ENB_APP, "-------------------------------\n");
/*
 *  * Done with statistics
 *   */

        msg_name = ITTI_MSG_NAME (msg_p);
        instance = ITTI_MSG_INSTANCE (msg_p);

        switch (ITTI_MSG_ID(msg_p))
        {
            case TERMINATE_MESSAGE:
		//send_msg_added();
                itti_exit_task ();
                break;

            case MESSAGE_TEST:
                LOG_I(ENB_APP, "Received %s\n", ITTI_MSG_NAME(msg_p));	
		//send_msg_added();
                break;

# if defined(ENABLE_USE_MME)
            case S1AP_REGISTER_ENB_CNF:
                LOG_I(ENB_APP, "[eNB %d] Received %s: associated MME %d\n", instance, msg_name,
                      S1AP_REGISTER_ENB_CNF(msg_p).nb_mme);

		//send_msg_added();
                DevAssert(register_enb_pending > 0);
                register_enb_pending--;

                /* Check if at least eNB is registered with one MME */
                if (S1AP_REGISTER_ENB_CNF(msg_p).nb_mme > 0)
                {
                    registered_enb++;
                }

                /* Check if all register eNB requests have been processed */
                if (register_enb_pending == 0)
                {
                    if (registered_enb == enb_nb)
                    {
                        /* If all eNB are registered, start L2L1 task */
                        MessageDef *msg_init_p;

                        msg_init_p = itti_alloc_new_message (TASK_ENB_APP, INITIALIZE_MESSAGE);
                        itti_send_msg_to_task (TASK_L2L1, INSTANCE_DEFAULT, msg_init_p);

#   if defined(OAI_EMU)
                        /* Also inform all NAS UE tasks */
                        for (instance = NB_eNB_INST + oai_emulation.info.first_ue_local;
                            instance < (NB_eNB_INST + oai_emulation.info.first_ue_local + oai_emulation.info.nb_ue_local); instance ++)
                        {
                            msg_init_p = itti_alloc_new_message (TASK_ENB_APP, INITIALIZE_MESSAGE);
                            itti_send_msg_to_task (TASK_NAS_UE, instance, msg_init_p);
                        }
#   endif
                    }
                    else
                    {
                        uint32_t not_associated = enb_nb - registered_enb;

                        LOG_W(ENB_APP, " %d eNB %s not associated with a MME, retrying registration in %d seconds ...\n",
                              not_associated, not_associated > 1 ? "are" : "is", ENB_REGISTER_RETRY_DELAY);

                        /* Restart the eNB registration process in ENB_REGISTER_RETRY_DELAY seconds */
                        if (timer_setup (ENB_REGISTER_RETRY_DELAY, 0, TASK_ENB_APP, INSTANCE_DEFAULT, TIMER_ONE_SHOT,
                                         NULL, &enb_register_retry_timer_id) < 0)
                        {
                            LOG_E(ENB_APP, " Can not start eNB register retry timer, use \"sleep\" instead!\n");

                            sleep(ENB_REGISTER_RETRY_DELAY);
                            /* Restart the registration process */
                            registered_enb = 0;
                            register_enb_pending = eNB_app_register (enb_id_start, enb_id_end, enb_properties_p);
                        }
                    }
                }
                break;

            case S1AP_DEREGISTERED_ENB_IND:
		//send_msg_added();
                LOG_W(ENB_APP, "[eNB %d] Received %s: associated MME %d\n", instance, msg_name,
                      S1AP_DEREGISTERED_ENB_IND(msg_p).nb_mme);

                /* TODO handle recovering of registration */
                break;

            case TIMER_HAS_EXPIRED:
                LOG_I(ENB_APP, " Received %s: timer_id %d\n", msg_name, TIMER_HAS_EXPIRED(msg_p).timer_id);
		send_msg_added();
                if (TIMER_HAS_EXPIRED (msg_p).timer_id == enb_register_retry_timer_id)
                {
                    /* Restart the registration process */
                    registered_enb = 0;
                    register_enb_pending = eNB_app_register (enb_id_start, enb_id_end, enb_properties_p);
                }
                break;
# endif

            default:
		//send_msg_added();
                LOG_E(ENB_APP, "Received unexpected message %s\n", msg_name);
                break;
        }

        LOG_I(ENB_APP, " ABout to call itti_free() \n");
//	if(ITTI_MSG_ORIGIN_ID(msg_p) != TIMER_HAS_EXPIRED)
           result = itti_free (ITTI_MSG_ORIGIN_ID(msg_p), msg_p);
        AssertFatal (result == EXIT_SUCCESS, "Failed to free memory (%d)!\n", result); 
    } while (1);
#endif

    return NULL;
}