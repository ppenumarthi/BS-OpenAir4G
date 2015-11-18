/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_PRACH_Config_H_
#define	_PRACH_Config_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PRACH_ConfigInfo;

/* PRACH-Config */
typedef struct PRACH_Config {
	long	 rootSequenceIndex;
	struct PRACH_ConfigInfo	*prach_ConfigInfo	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PRACH_Config_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PRACH_Config;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PRACH-ConfigInfo.h"

#endif	/* _PRACH_Config_H_ */
#include <asn_internal.h>