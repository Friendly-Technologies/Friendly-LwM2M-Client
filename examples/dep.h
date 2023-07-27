/*
 * dep.h
 *
 *  Created on: 11 Jul 2023
 *      Author: valentin
 */

#ifndef TMP_DEP_H
#define TMP_DEP_H

#include <cstdint>
#include <time.h>

#define COAP_NO_ERROR                   (uint8_t)0x00
#define COAP_IGNORE                     (uint8_t)0x01
#define COAP_RETRANSMISSION             (uint8_t)0x02

#define COAP_201_CREATED                (uint8_t)0x41
#define COAP_202_DELETED                (uint8_t)0x42
#define COAP_204_CHANGED                (uint8_t)0x44
#define COAP_205_CONTENT                (uint8_t)0x45
#define COAP_231_CONTINUE               (uint8_t)0x5F
#define COAP_400_BAD_REQUEST            (uint8_t)0x80
#define COAP_401_UNAUTHORIZED           (uint8_t)0x81
#define COAP_402_BAD_OPTION             (uint8_t)0x82
#define COAP_404_NOT_FOUND              (uint8_t)0x84
#define COAP_405_METHOD_NOT_ALLOWED     (uint8_t)0x85
#define COAP_406_NOT_ACCEPTABLE         (uint8_t)0x86
#define COAP_408_REQ_ENTITY_INCOMPLETE  (uint8_t)0x88
#define COAP_412_PRECONDITION_FAILED    (uint8_t)0x8C
#define COAP_413_ENTITY_TOO_LARGE       (uint8_t)0x8D
#define COAP_500_INTERNAL_SERVER_ERROR  (uint8_t)0xA0
#define COAP_501_NOT_IMPLEMENTED        (uint8_t)0xA1
#define COAP_503_SERVICE_UNAVAILABLE    (uint8_t)0xA3

#define LWM2M_MAX_ID   ((uint16_t)0xFFFF)

#define LWM2M_URI_IS_SET_OBJECT(uri) ((uri)->objectId != LWM2M_MAX_ID)
#define LWM2M_URI_IS_SET_INSTANCE(uri) ((uri)->instanceId != LWM2M_MAX_ID)
#define LWM2M_URI_IS_SET_RESOURCE(uri) ((uri)->resourceId != LWM2M_MAX_ID)
#ifndef LWM2M_VERSION_1_0
#define LWM2M_URI_IS_SET_RESOURCE_INSTANCE(uri) ((uri)->resourceInstanceId != LWM2M_MAX_ID)
#endif

typedef enum
{
    STATE_INITIAL = 0,
    STATE_BOOTSTRAP_REQUIRED,
    STATE_BOOTSTRAPPING,
    STATE_REGISTER_REQUIRED,
    STATE_REGISTERING,
    STATE_READY
} lwm2m_client_state_t;

typedef enum
{
    LWM2M_CONTENT_TEXT       = 0,        // Also used as undefined
    LWM2M_CONTENT_LINK       = 40,
    LWM2M_CONTENT_OPAQUE     = 42,
    LWM2M_CONTENT_TLV_OLD    = 1542,     // Keep old value for backward-compatibility
    LWM2M_CONTENT_TLV        = 11542,
    LWM2M_CONTENT_JSON_OLD   = 1543,     // Keep old value for backward-compatibility
    LWM2M_CONTENT_JSON       = 11543,
    LWM2M_CONTENT_SENML_JSON = 110
} lwm2m_media_type_t;

typedef struct
{
    uint16_t    objectId;
    uint16_t    instanceId;
    uint16_t    resourceId;
#ifndef LWM2M_VERSION_1_0
    uint16_t    resourceInstanceId;
#endif
} lwm2m_uri_t;

typedef struct _lwm2m_context_ lwm2m_context_t;

typedef enum
{
    LWM2M_WRITE_PARTIAL_UPDATE,     // Write should add or update resources and resource instances.
    LWM2M_WRITE_REPLACE_RESOURCES,  // Write should replace resources entirely.
    LWM2M_WRITE_REPLACE_INSTANCE,   // Write should replace the entire instance.
} lwm2m_write_type_t;

typedef struct _lwm2m_list_t
{
    struct _lwm2m_list_t * next;
    uint16_t    id;
} lwm2m_list_t;

lwm2m_list_t * lwm2m_list_add(lwm2m_list_t * head,
                              lwm2m_list_t * node);


lwm2m_list_t * lwm2m_list_find(lwm2m_list_t * head,
                               uint16_t id);


lwm2m_list_t * lwm2m_list_remove(lwm2m_list_t * head,
                                 uint16_t id,
                                 lwm2m_list_t ** nodeP);

uint16_t lwm2m_list_newId(lwm2m_list_t * head);

void lwm2m_list_free(lwm2m_list_t * head);

#define LWM2M_LIST_ADD(H,N) lwm2m_list_add((lwm2m_list_t *)H, (lwm2m_list_t *)N);
#define LWM2M_LIST_RM(H,I,N) lwm2m_list_remove((lwm2m_list_t *)H, I, (lwm2m_list_t **)N);
#define LWM2M_LIST_FIND(H,I) lwm2m_list_find((lwm2m_list_t *)H, I)
#define LWM2M_LIST_FREE(H) lwm2m_list_free((lwm2m_list_t *)H)


typedef struct _lwm2m_transaction_ lwm2m_transaction_t;

typedef void (*lwm2m_transaction_callback_t) (lwm2m_context_t * contextP, lwm2m_transaction_t * transacP, void * message);

struct _lwm2m_transaction_
{
    lwm2m_transaction_t * next;  // matches lwm2m_list_t::next
    uint16_t              mID;   // matches lwm2m_list_t::id
    void *                peerH;
    uint8_t               ack_received; // indicates, that the ACK was received
    time_t                response_timeout; // timeout to wait for response, if token is used. When 0, use calculated acknowledge timeout.
    uint8_t  retrans_counter;
    time_t   retrans_time;
    void * message;
    uint16_t buffer_len;
    uint8_t * buffer;
    size_t
        payload_len;  // the length of the entire payload, message payload might be smaller in case of a block1 transfer
    uint8_t *payload; // carries the entire payload across multiple transactions in case of a block 1 transfer
    lwm2m_transaction_callback_t callback;
    void * userData;
};

#define LWM2M_CLIENT_MODE
struct _lwm2m_context_
{
#ifdef LWM2M_CLIENT_MODE
    lwm2m_client_state_t state;
    char *               endpointName;
    char *               msisdn;
    char *               altPath;
//    lwm2m_server_t *     bootstrapServerList;
//    lwm2m_server_t *     serverList;
//    lwm2m_object_t *     objectList;
//    lwm2m_observed_t *   observedList;
#endif
#if defined(LWM2M_SERVER_MODE) || defined(LWM2M_BOOTSTRAP_SERVER_MODE)
    lwm2m_client_t *        clientList;
#endif
#ifdef LWM2M_SERVER_MODE
    lwm2m_result_callback_t monitorCallback;
    void *                  monitorUserData;
#endif
#ifdef LWM2M_BOOTSTRAP_SERVER_MODE
    lwm2m_bootstrap_callback_t bootstrapCallback;
    void *                     bootstrapUserData;
#endif
    uint16_t                nextMID;
    lwm2m_transaction_t *   transactionList;
    void *                  userData;
};


typedef enum
{
    LWM2M_TYPE_UNDEFINED = 0,
    LWM2M_TYPE_OBJECT,
    LWM2M_TYPE_OBJECT_INSTANCE,
    LWM2M_TYPE_MULTIPLE_RESOURCE,

    LWM2M_TYPE_STRING,
    LWM2M_TYPE_OPAQUE,
    LWM2M_TYPE_INTEGER,
    LWM2M_TYPE_UNSIGNED_INTEGER,
    LWM2M_TYPE_FLOAT,
    LWM2M_TYPE_BOOLEAN,

    LWM2M_TYPE_OBJECT_LINK,
    LWM2M_TYPE_CORE_LINK
} lwm2m_data_type_t;

typedef struct _lwm2m_data_t lwm2m_data_t;

struct _lwm2m_data_t
{
    lwm2m_data_type_t type;
    uint16_t    id;
    union
    {
        bool        asBoolean;
        int64_t     asInteger;
        uint64_t    asUnsigned;
        double      asFloat;
        struct
        {
            size_t    length;
            uint8_t * buffer;
        } asBuffer;
        struct
        {
            size_t         count;
            lwm2m_data_t * array;
        } asChildren;
        struct
        {
            uint16_t objectId;
            uint16_t objectInstanceId;
        } asObjLink;
    } value;
};

typedef struct _lwm2m_object_t lwm2m_object_t;

typedef uint8_t (*lwm2m_read_callback_t) (lwm2m_context_t * contextP, uint16_t instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);
typedef uint8_t (*lwm2m_discover_callback_t) (lwm2m_context_t * contextP, uint16_t instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP);
typedef uint8_t (*lwm2m_write_callback_t) (lwm2m_context_t * contextP, uint16_t instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP, lwm2m_write_type_t writeType);
typedef uint8_t (*lwm2m_execute_callback_t) (lwm2m_context_t * contextP, uint16_t instanceId, uint16_t resourceId, uint8_t * buffer, int length, lwm2m_object_t * objectP);
typedef uint8_t (*lwm2m_create_callback_t) (lwm2m_context_t * contextP, uint16_t instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
typedef uint8_t (*lwm2m_raw_block1_create_callback_t) (lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_media_type_t format, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
typedef uint8_t (*lwm2m_raw_block1_write_callback_t) (lwm2m_context_t * contextP, lwm2m_uri_t * uriP, lwm2m_media_type_t format, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
typedef uint8_t (*lwm2m_raw_block1_execute_callback_t) (lwm2m_context_t * contextP, lwm2m_uri_t * uriP, uint8_t * buffer, int length, lwm2m_object_t * objectP, uint32_t block_num, uint8_t block_more);
#endif
typedef uint8_t (*lwm2m_delete_callback_t) (lwm2m_context_t * contextP, uint16_t instanceId, lwm2m_object_t * objectP);

struct _lwm2m_object_t
{
    struct _lwm2m_object_t * next;           // for internal use only.
    uint16_t       objID;
    uint8_t        versionMajor;
    uint8_t        versionMinor;
    lwm2m_list_t * instanceList;
    lwm2m_read_callback_t     readFunc;
    lwm2m_write_callback_t    writeFunc;
    lwm2m_execute_callback_t  executeFunc;
    lwm2m_create_callback_t   createFunc;
#ifdef LWM2M_RAW_BLOCK1_REQUESTS
    lwm2m_raw_block1_create_callback_t   rawBlock1CreateFunc;
    lwm2m_raw_block1_write_callback_t    rawBlock1WriteFunc;
    lwm2m_raw_block1_execute_callback_t  rawBlock1ExecuteFunc;
#endif
    lwm2m_delete_callback_t   deleteFunc;
    lwm2m_discover_callback_t discoverFunc;
    void * userData;
};

#endif // TMP_DEP_H
