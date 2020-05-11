/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9.2 at Sun Mar  3 23:12:09 2019. */

#ifndef PB_ALEXAGADGETMUSICDATA_ALEXAGADGETMUSICDATATEMPODIRECTIVEPAYLOAD_PB_H_INCLUDED
#define PB_ALEXAGADGETMUSICDATA_ALEXAGADGETMUSICDATATEMPODIRECTIVEPAYLOAD_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData {
    int32_t value;
    int32_t startOffsetInMilliSeconds;
/* @@protoc_insertion_point(struct:alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData) */
} alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData;

typedef struct _alexaGadgetMusicData_TempoDirectivePayloadProto {
    int32_t playerOffsetInMilliSeconds;
    pb_size_t tempoData_count;
    alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData tempoData[1];
/* @@protoc_insertion_point(struct:alexaGadgetMusicData_TempoDirectivePayloadProto) */
} alexaGadgetMusicData_TempoDirectivePayloadProto;

/* Default values for struct fields */

/* Initializer values for message structs */
#define alexaGadgetMusicData_TempoDirectivePayloadProto_init_default {0, 0, {alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_init_default}}
#define alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_init_default {0, 0}
#define alexaGadgetMusicData_TempoDirectivePayloadProto_init_zero {0, 0, {alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_init_zero}}
#define alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_init_zero {0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_startOffsetInMilliSeconds_tag 2
#define alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_value_tag 1
#define alexaGadgetMusicData_TempoDirectivePayloadProto_playerOffsetInMilliSeconds_tag 1
#define alexaGadgetMusicData_TempoDirectivePayloadProto_tempoData_tag 2

/* Struct field encoding specification for nanopb */
extern const pb_field_t alexaGadgetMusicData_TempoDirectivePayloadProto_fields[3];
extern const pb_field_t alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_fields[3];

/* Maximum encoded size of messages (where known) */
#define alexaGadgetMusicData_TempoDirectivePayloadProto_size 36
#define alexaGadgetMusicData_TempoDirectivePayloadProto_TempoData_size 22

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define ALEXAGADGETMUSICDATATEMPODIRECTIVEPAYLOAD_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
