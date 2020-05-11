/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.9.2 at Sun Mar  3 23:12:08 2019. */

#include "alertsSetAlertDirectivePayload.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t alerts_SetAlertDirectivePayloadProto_fields[9] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, alerts_SetAlertDirectivePayloadProto, token, token, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto, type, token, 0),
    PB_FIELD(  3, STRING  , SINGULAR, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto, scheduledTime, type, 0),
    PB_FIELD(  4, MESSAGE , REPEATED, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto, assets, scheduledTime, &alerts_SetAlertDirectivePayloadProto_Assets_fields),
    PB_FIELD(  5, STRING  , REPEATED, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto, assetPlayOrder, assets, 0),
    PB_FIELD(  6, STRING  , SINGULAR, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto, backgroundAlertAsset, assetPlayOrder, 0),
    PB_FIELD(  7, INT32   , SINGULAR, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto, loopCount, backgroundAlertAsset, 0),
    PB_FIELD(  8, INT32   , SINGULAR, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto, loopPauseInMilliSeconds, loopCount, 0),
    PB_LAST_FIELD
};

const pb_field_t alerts_SetAlertDirectivePayloadProto_Assets_fields[3] = {
    PB_FIELD(  1, STRING  , SINGULAR, STATIC  , FIRST, alerts_SetAlertDirectivePayloadProto_Assets, assetId, assetId, 0),
    PB_FIELD(  2, STRING  , SINGULAR, STATIC  , OTHER, alerts_SetAlertDirectivePayloadProto_Assets, url, assetId, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(alerts_SetAlertDirectivePayloadProto, assets[0]) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_alerts_SetAlertDirectivePayloadProto_alerts_SetAlertDirectivePayloadProto_Assets)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(alerts_SetAlertDirectivePayloadProto, assets[0]) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_alerts_SetAlertDirectivePayloadProto_alerts_SetAlertDirectivePayloadProto_Assets)
#endif


/* @@protoc_insertion_point(eof) */
