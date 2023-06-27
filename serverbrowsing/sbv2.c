#include "serverbrowsing.h"

#define FIXED_HEADER_LEN 6

int proto_sbv2 = -1;
gint proto_sbv2_ett = -1;

static gint* sbv2_etts[] = {
    &proto_sbv2_ett
};

int sbv2_incoming_length = -1;
int sbv2_request_type = -1;
int sbv2_listreq_protocol_version = -1;
int sbv2_listreq_encoding_version = -1;
int sbv2_listreq_game_version = -1;
int sbv2_listreq_for_gamename = -1;
int sbv2_listreq_from_gamename = -1;
int sbv2_listreq_challenge = -1;
int sbv2_listreq_filter = -1;
int sbv2_listreq_key_list = -1;
int sbv2_listreq_options = -1;
int sbv2_listreq_source_ip = -1;
int sbv2_listreq_max_results = -1;

int sbv2_crypt_header_len = -1;
int sbv2_crypt_header_random_data = -1;
int sbv2_crypt_header_keylen = -1;
int sbv2_crypt_header_key_data = -1;

int sbv2_listresp_public_ip = -1;
int sbv2_listresp_query_port = -1;
int sbv2_listresp_num_fields = -1;
int sbv2_listresp_num_popular_values = -1;

int sbv2_listresp_server_flags = -1;
int sbv2_listresp_server_ip = -1;
int sbv2_listresp_server_port = -1;
int sbv2_listresp_server_private_ip = -1;
int sbv2_listresp_server_private_port = -1;
int sbv2_listresp_server_icmp_ip = -1;
int sbv2_listresp_server_updateflags_unsolicited_udp_flag = -1;
int sbv2_listresp_server_updateflags_private_ip_flag = -1;
int sbv2_listresp_server_updateflags_connect_negotiate_flag = -1;
int sbv2_listresp_server_updateflags_icmp_ip_flag = -1;
int sbv2_listresp_server_updateflags_nonstandard_port_flag = -1;
int sbv2_listresp_server_updateflags_nonstandard_private_port_flag = -1;
int sbv2_listresp_server_updateflags_has_keys_flag = -1;
int sbv2_listresp_server_updateflags_has_rules_flag = -1;

static int* const server_updateflags_bits[] = {
	&sbv2_listresp_server_updateflags_unsolicited_udp_flag,
	&sbv2_listresp_server_updateflags_private_ip_flag,
	&sbv2_listresp_server_updateflags_connect_negotiate_flag,
	&sbv2_listresp_server_updateflags_icmp_ip_flag,
	&sbv2_listresp_server_updateflags_nonstandard_port_flag,
	&sbv2_listresp_server_updateflags_nonstandard_private_port_flag,
	&sbv2_listresp_server_updateflags_has_keys_flag,
	&sbv2_listresp_server_updateflags_has_rules_flag,
	NULL
};


unsigned char *enctypex_decoder(unsigned char *key, unsigned char *validate, unsigned char *data, int *datalen, enctypex_data_t *enctypex_data);

static hf_register_info sbv2_fields_hf[] = {
    //crypt command properties
    { &sbv2_incoming_length,
        { "incoming_length", "sbv2.incoming_length",
        FT_INT16, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_request_type,
        { "request_type", "sbv2.request_type",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_protocol_version,
        { "protocol_version", "sbv2.listreq.protocol_version",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_encoding_version,
        { "encoding_version", "sbv2.listreq.encoding_version",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_game_version,
        { "game_version", "sbv2.listreq.game_version",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_for_gamename,
        { "for_gamename", "sbv2.listreq.for_gamename",
        FT_STRING, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_from_gamename,
        { "from_gamename", "sbv2.listreq.from_gamename",
        FT_STRING, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_challenge,
        { "challenge", "sbv2.listreq.challenge",
        FT_BYTES, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_filter,
        { "filter", "sbv2.listreq.filter",
        FT_STRING, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_key_list,
        { "key_list", "sbv2.listreq.key_list",
        FT_STRING, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_options,
        { "options", "sbv2.listreq.options",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_source_ip,
        { "alternate_source_ip", "sbv2.listreq.alternate_source_ip",
        FT_IPv4, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listreq_max_results,
        { "max_results", "sbv2.listreq.max_results",
        FT_UINT32, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },

    //list response
    { &sbv2_listresp_public_ip,
        { "public_ip", "sbv2.listresp.public_ip",
        FT_IPv4, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_query_port,
        { "query_port", "sbv2.listresp.query_port",
        FT_UINT16, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_num_fields,
        { "num_fields", "sbv2.listresp.num_fields",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_num_popular_values,
        { "num_popular_values", "sbv2.listresp.num_popular_values",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    //server item
    { &sbv2_listresp_server_flags,
        { "server_flags", "sbv2.listresp.server_flags",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_ip,
        { "server_ip", "sbv2.listresp.server_ip",
        FT_IPv4, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_port,
        { "server_port", "sbv2.listresp.server_port",
        FT_UINT16, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_private_ip,
        { "private_ip", "sbv2.listresp.private_ip",
        FT_IPv4, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_private_port,
        { "private_port", "sbv2.listresp.private_port",
        FT_UINT16, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_icmp_ip,
        { "icmp_ip", "sbv2.listresp.icmp_ip",
        FT_IPv4, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
    // server list item flags
    { &sbv2_listresp_server_updateflags_unsolicited_udp_flag,
        { "unsolicited_udp_flag", "sbv2.listresp.server_flags.unsolicited_udp_flag",
        FT_BOOLEAN, 9,
        NULL, UNSOLICITED_UDP_FLAG,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_updateflags_private_ip_flag,
        { "private_ip_flag", "sbv2.listresp.server_flags.private_ip_flag",
        FT_BOOLEAN, 9,
        NULL, PRIVATE_IP_FLAG,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_updateflags_connect_negotiate_flag,
        { "connect_negotiate_flag", "sbv2.listresp.server_flags.connect_negotiate_flag",
        FT_BOOLEAN, 9,
        NULL, CONNECT_NEGOTIATE_FLAG,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_updateflags_icmp_ip_flag,
        { "icmp_ip_flag", "sbv2.listresp.server_flags.icmp_ip_flag",
        FT_BOOLEAN, 9,
        NULL, ICMP_IP_FLAG,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_updateflags_nonstandard_port_flag,
        { "nonstandard_port_flag", "sbv2.listresp.server_flags.nonstandard_port_flag",
        FT_BOOLEAN, 9,
        NULL, NONSTANDARD_PORT_FLAG,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_updateflags_nonstandard_private_port_flag,
        { "nonstandard_private_port_flag", "sbv2.listresp.server_flags.nonstandard_private_port_flag",
        FT_BOOLEAN, 9,
        NULL, NONSTANDARD_PRIVATE_PORT_FLAG,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_updateflags_has_keys_flag,
        { "has_keys_flag", "sbv2.listresp.server_flags.has_keys_flag",
        FT_BOOLEAN, 9,
        NULL, HAS_KEYS_FLAG,
        NULL, HFILL }
    },
    { &sbv2_listresp_server_updateflags_has_rules_flag,
        { "has_rules_flag", "sbv2.listresp.server_flags.has_rules_flag",
        FT_BOOLEAN, 9,
        NULL, HAS_FULL_RULES_FLAG,
        NULL, HFILL }
    },
    //
    { &sbv2_crypt_header_len,
        { "crypt.len", "sbv2.listresp.crypt.len",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_crypt_header_random_data,
        { "crypt.random", "sbv2.crypt.random",
        FT_BYTES, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL}
    },
    { &sbv2_crypt_header_keylen,
        { "crypt.keylen", "sbv2.listresp.crypt.keylen",
        FT_UINT8, BASE_DEC,
        NULL, 0x0,
        NULL, HFILL }
    },
    { &sbv2_crypt_header_key_data,
        { "crypt.key", "sbv2.crypt.key",
        FT_BYTES, BASE_NONE,
        NULL, 0x0,
        NULL, HFILL }
    },
};

typedef struct _sbv2_conv_t {
    enctypex_data_t enctypex_data;
    char challenge[LIST_CHALLENGE_LEN];
    int last_server_pdu;
    int response_crypt_header_pdu;
    const char** query_from_game; //pointer to gslist_keys
} sbv2_conv_t;

typedef struct _sbv2_pdu_crypto_state {
	enctypex_data_t state;
} sbv2_pdu_crypto_state;

static sbv2_conv_t* get_sbv2_conversation_data(packet_info* pinfo)
{
    conversation_t* conversation;
    sbv2_conv_t* conv_data;

    conversation = find_or_create_conversation(pinfo);

    /* Retrieve information from conversation
     * or add it if it isn't there yet
     */
    conv_data = (sbv2_conv_t*)conversation_get_proto_data(conversation, proto_sbv2);
    if (!conv_data) {
        /* Setup the conversation structure itself */
        conv_data = (sbv2_conv_t*)wmem_alloc0(wmem_file_scope(), sizeof(sbv2_conv_t));

        conversation_add_proto_data(conversation, proto_sbv2,
            conv_data);
    }

    return conv_data;
}

static sbv2_pdu_crypto_state* get_sbv2_pdu_crypto_state(packet_info* pinfo) {
    conversation_t* conversation;
    sbv2_pdu_crypto_state* conv_data;

    conversation = find_or_create_conversation_by_id(pinfo, CONVERSATION_TCP, pinfo->num);

    /* Retrieve information from conversation
     * or add it if it isn't there yet
     */
    conv_data = (sbv2_pdu_crypto_state*)conversation_get_proto_data(conversation, proto_sbv2);
    if (!conv_data) {
        /* Setup the conversation structure itself */
        conv_data = (sbv2_pdu_crypto_state*)wmem_alloc0(wmem_file_scope(), sizeof(sbv2_pdu_crypto_state));

        //copy latest crypto state
        sbv2_conv_t* sbv2_conv = get_sbv2_conversation_data(pinfo);

        memcpy(&conv_data->state, &sbv2_conv->enctypex_data, sizeof(conv_data->state));

        conversation_add_proto_data(conversation, proto_sbv2,
            conv_data);
    }

    return conv_data;
}


int dissect_sbv2_list_request(tvbuff_t* tvb, packet_info* pinfo, proto_tree* tree _U_, void* data _U_, int offset) {
    sbv2_conv_t *conv = get_sbv2_conversation_data(pinfo);

    col_set_str(pinfo->cinfo, COL_PROTOCOL, "SBV2 List Req");
    proto_tree_add_item(tree, sbv2_listreq_protocol_version, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);
    proto_tree_add_item(tree, sbv2_listreq_encoding_version, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);
    proto_tree_add_item(tree, sbv2_listreq_game_version, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);

    offset += add_string_nts_item(tvb, tree, sbv2_listreq_for_gamename, offset);
    
    int from_len = add_string_nts_item(tvb, tree, sbv2_listreq_from_gamename, offset);
    char *from_gamename = (char *)tvb_get_ptr(tvb, offset, from_len);
    if(conv->query_from_game == NULL) {
        conv->query_from_game = gslist_keys_find_by_gamename(from_gamename, from_len);
    }

    offset += from_len;

    proto_tree_add_item(tree, sbv2_listreq_challenge, tvb, offset,  LIST_CHALLENGE_LEN, ENC_BIG_ENDIAN); 
    for(int i=0;i<LIST_CHALLENGE_LEN;i++) {
        conv->challenge[i] = tvb_get_guint8(tvb, offset++);
    }
    offset += add_string_nts_item(tvb, tree, sbv2_listreq_filter, offset);
    offset += add_string_nts_item(tvb, tree, sbv2_listreq_key_list, offset);

    guint32 options = 0;
    proto_tree_add_item_ret_uint(tree, sbv2_listreq_options, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN, &options); offset += sizeof(uint32_t);

    if(options & ALTERNATE_SOURCE_IP) {
        proto_tree_add_item(tree, sbv2_listreq_source_ip, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    }
    if (options & LIMIT_RESULT_COUNT) {
        proto_tree_add_item(tree, sbv2_listreq_max_results, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    }

    return tvb_captured_length(tvb);
}

int dissect_sbv2_client_stream(tvbuff_t* tvb, packet_info* pinfo, proto_tree* tree _U_, void* data _U_) {
    
    int offset = 0;
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "SBV2");
    proto_tree_add_item(tree, sbv2_incoming_length, tvb, offset, sizeof(uint16_t), ENC_BIG_ENDIAN); offset += sizeof(uint16_t);
    proto_tree_add_item(tree, sbv2_request_type, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);
    return dissect_sbv2_list_request(tvb, pinfo, tree, data, offset);
}

static guint
    get_sbv2_incoming_message_len(packet_info* pinfo _U_, tvbuff_t* tvb, int offset, void* data _U_)
{
    uint16_t message_length = tvb_get_ntohs(tvb, offset);
    return (guint)message_length;
}

static guint
    get_sbv2_response_crypt_random_len(packet_info* pinfo _U_, tvbuff_t* tvb, int original_offset, void* data _U_)
{
    printf("get_sbv2_response_crypt_random_len - %d - %d\n", pinfo->num, pinfo->fd->num);
    
    int offset = original_offset;
    gint available = tvb_reported_length_remaining(tvb, offset);


    //calculate crypt header stuff
    guint8 random_len = tvb_get_guint8(tvb, offset); offset++;
    random_len ^= 0xEC;

    available--;
    if(available < random_len) {
        pinfo->desegment_offset = original_offset;
        pinfo->desegment_len = random_len;
        return 0;
    }
    available -= random_len;
    offset += random_len;

    guint8 key_len = tvb_get_guint8(tvb, offset); offset++;
    key_len ^= 0xEA; 

    if(available < key_len) {
        pinfo->desegment_offset = original_offset;
        pinfo->desegment_len = key_len;
        return 0;
    }
    available -= key_len;
    offset += key_len;


    //calculate fixed header
    if(available < FIXED_HEADER_LEN) { 
        pinfo->desegment_offset = original_offset;
        pinfo->desegment_len = FIXED_HEADER_LEN;
        return 0;
    }

    //decrypt data...
    enctypex_data_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    sbv2_conv_t *conv = get_sbv2_conversation_data(pinfo);


    int enctypex_data_len = offset - original_offset;
    void *key_data = tvb_memdup(wmem_packet_scope(), tvb, original_offset, enctypex_data_len);
    
    
    enctypex_init(&ctx.encxkey, conv->query_from_game[2], conv->challenge, key_data, &enctypex_data_len, &ctx);

    printf("enctypex_data_len: %d\n", enctypex_data_len);

    //skip fixed header
    for(int i=0;i<FIXED_HEADER_LEN;i++) {
        uint8_t c = tvb_get_guint8(tvb, offset++);
        enctypex_func6(&ctx.encxkey, &c, sizeof(c));
    }\



    //calculate key list
    uint8_t key_list_size = tvb_get_guint8(tvb, offset++);
    enctypex_func6(&ctx.encxkey, &key_list_size, sizeof(key_list_size));
    printf("key_list_size: %d\n", key_list_size);

    //calculate unique list

    //calculate main server list

    //after here is only adhoc messages!

    return (guint)2 + random_len + key_len;
}

int dissect_sbv2_response_list_item(tvbuff_t* tvb, packet_info* pinfo, proto_tree* tree _U_, void* data _U_, int offset) {
    proto_item* ti = proto_tree_add_item(tree, proto_sbv2, tvb, 0, -1, ENC_NA);
    proto_tree* subtree = proto_item_add_subtree(ti, proto_sbv2_ett);
    proto_item_set_text(subtree, "Server Item");

    tree = subtree;
    
    int len = 0;
    guint8 flags = tvb_get_guint8(tvb, offset);
    gint32 ip = 0;
	proto_tree_add_bitmask_value(tree, tvb, len + offset, sbv2_listresp_server_flags, proto_sbv2_ett, server_updateflags_bits, flags); len += sizeof(uint8_t);


    ip = tvb_get_ntohl(tvb, offset + len);
    proto_tree_add_item(tree, sbv2_listresp_server_ip, tvb, len + offset, sizeof(uint32_t), ENC_BIG_ENDIAN); len += sizeof(uint32_t);

    if(ip == -1) {
        return -len;
    }
    if (flags & NONSTANDARD_PORT_FLAG) {
        proto_tree_add_item(tree, sbv2_listresp_server_port, tvb, len + offset, sizeof(uint16_t), ENC_BIG_ENDIAN); len += sizeof(uint16_t);
    }
    if (flags & PRIVATE_IP_FLAG) {
        proto_tree_add_item(tree, sbv2_listresp_server_private_ip, tvb, len + offset, sizeof(uint32_t), ENC_BIG_ENDIAN); len += sizeof(uint32_t);
    }
    if (flags & NONSTANDARD_PRIVATE_PORT_FLAG) {
        proto_tree_add_item(tree, sbv2_listresp_server_private_port, tvb, len + offset, sizeof(uint16_t), ENC_BIG_ENDIAN); len += sizeof(uint16_t);    
    }
    if(flags & ICMP_IP_FLAG) {
        proto_tree_add_item(tree, sbv2_listresp_server_icmp_ip, tvb, len + offset, sizeof(uint32_t), ENC_BIG_ENDIAN); len += sizeof(uint32_t);
    }
    return len;
}
int dissect_sbv2_response_list_header(tvbuff_t* tvb, packet_info* pinfo, proto_tree* tree _U_, void* data _U_) {
    int offset = 0;
    //col_set_str(pinfo->cinfo, COL_PROTOCOL, "SBV2 list response");

    sbv2_conv_t *conv = get_sbv2_conversation_data(pinfo);

    proto_tree_add_item(tree, sbv2_listresp_public_ip, tvb, offset, sizeof(uint32_t), ENC_BIG_ENDIAN); offset += sizeof(uint32_t);
    proto_tree_add_item(tree, sbv2_listresp_query_port, tvb, offset, sizeof(uint16_t), ENC_BIG_ENDIAN); offset += sizeof(uint16_t);

    //proto_tree_add_item(tree, sbv2_listresp_num_fields, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);
    //proto_tree_add_item(tree, sbv2_listresp_num_popular_values, tvb, offset, sizeof(uint8_t), ENC_BIG_ENDIAN); offset += sizeof(uint8_t);


    // while(true) {
    //     int len = dissect_sbv2_response_list_item(tvb, pinfo, tree, data, offset);
    //     if(len < 0) {
    //         offset += -len;
    //         break;
    //     }
    //     offset += len;
    // }
    
    return tvb_captured_length(tvb);
}


int dissect_sbv2_response_crypt_header(tvbuff_t* tvb, packet_info* pinfo, proto_tree* tree _U_, void* data _U_) {
    int offset = 0;

    guint8 len = tvb_get_guint8(tvb, offset);
    len ^= 0xEC;
    proto_tree_add_uint(tree, sbv2_crypt_header_len, tvb, offset, sizeof(uint8_t), len); offset++;
    proto_tree_add_item(tree, sbv2_crypt_header_random_data, tvb, offset,  len, ENC_BIG_ENDIAN); offset += len;
    

    len = tvb_get_guint8(tvb, offset);
    len ^= 0xEA;


    proto_tree_add_uint(tree, sbv2_crypt_header_keylen, tvb, offset, sizeof(uint8_t), len); offset++;
    proto_tree_add_item(tree, sbv2_crypt_header_key_data, tvb, offset, len, ENC_BIG_ENDIAN); offset += len;


    return tvb_captured_length(tvb);
}

int dissect_sbv2(tvbuff_t* tvb, packet_info* pinfo, proto_tree* tree _U_, void* data _U_) { 

    if(pinfo->srcport != DEFAULT_SBV2_PORT) {
        tcp_dissect_pdus(tvb, pinfo, tree, TRUE, 2, get_sbv2_incoming_message_len, dissect_sbv2_client_stream, data);
    } else {
        sbv2_conv_t *conv = get_sbv2_conversation_data(pinfo);
        if(conv->response_crypt_header_pdu == 0 || pinfo->num == conv->response_crypt_header_pdu) {
            conv->response_crypt_header_pdu = pinfo->num;
            tcp_dissect_pdus(tvb, pinfo, tree, TRUE, 1, get_sbv2_response_crypt_random_len, dissect_sbv2_response_crypt_header, data);
            //return dissect_sbv2_response_crypt_header(tvb, pinfo, tree, data);
        } else {
            printf("skip num: %d %d\n", pinfo->num, pinfo->fd->num);
        }
        //return dissect_sbv2_response(tvb, pinfo, tree, data);

    }
   
    return tvb_captured_length(tvb);
}



void proto_register_sbv2(void)
{
    proto_sbv2 = proto_register_protocol(
        "GS SBv2",        /* name        */
        "sbv2",          /* short name  */
        "gs_sbv2"        /* filter_name */
    );
    proto_register_field_array(proto_sbv2, sbv2_fields_hf, array_length(sbv2_fields_hf));
    proto_register_subtree_array(sbv2_etts, array_length(sbv2_etts));
}


void proto_reg_handoff_sbv2(void)
{
    static dissector_handle_t sbv2_handle;

    sbv2_handle = create_dissector_handle(dissect_sbv2, proto_sbv2);
    dissector_add_uint("tcp.port", DEFAULT_SBV2_PORT, sbv2_handle);

}