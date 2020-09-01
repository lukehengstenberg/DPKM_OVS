
#ifndef OPENFLOW_DPKM_H
#define OPENFLOW_DPKM_H 1

#include <openflow/openflow.h>
#include <openvswitch/types.h>

#define DPKM_VENDOR 0xa20a0323
#define KEY_LEN 256
#define IP_LEN 32

enum ofp_dpkm_key_flag {

    OFP_DPKM_KEY_FLAG_PRIVATE_KEY = 0, /* Indicates TLV contains private
                                               key. */
    OFP_DPKM_KEY_FLAG_PUBLIC_KEY = 1,  /* Indicates TLV contains public
                                               key. */
    OFP_DPKM_KEY_FLAG_DELETE_PEER = 2, /* Indicates attribute is used for
                                               deleting a peer. */
};

struct ofp_dpkm_key_tlv {
    ovs_be16 type;
    ovs_be16 length;
    ovs_be32 key[8];
    ovs_be32 ipv4_dst;
    ovs_be32 key_flag; /* ofp_dpkm_key_flag. */
    uint8_t pad[4]; /* 64-bit alignment. */
};
OFP_ASSERT(sizeof(struct ofp_dpkm_key_tlv) == 48);

enum ofp_dpkm_status_flag {
    OFP_DPKM_STATUS_FLAG_CONFIGURED = 0,   /* WG is configured. */
    OFP_DPKM_STATUS_FLAG_PEER_ADDED = 1,   /* WG peer has been added. */
    OFP_DPKM_STATUS_FLAG_PEER_REMOVED = 2, /* WG peer has been removed. */
    OFP_DPKM_STATUS_FLAG_CONNECTED = 3,    /* WG has a peer connection. */
    OFP_DPKM_STATUS_FLAG_REVOKED = 4,      /* Private key is revoked. */
};

struct ofp_dpkm_status_tlv {
    ovs_be16 type;
    ovs_be16 length;
    ovs_be32 flags; /* OFP_DPKM_*. */
    struct ofp_dpkm_key_tlv peers;
};
OFP_ASSERT(sizeof(struct ofp_dpkm_status_tlv) == 56);

struct ofp_dpkm_status {
    ovs_be32 status_flag;
    char key[KEY_LEN];
    char ipv4_addr[IP_LEN];
    char ipv4_wg[IP_LEN];
    char ipv4_peer[IP_LEN];
};
OFP_ASSERT(sizeof(struct ofp_dpkm_status) == 356);

struct ofp_dpkm_set_key {
    ovs_be32 experimenter;
    ovs_be32 subtype;
};
OFP_ASSERT(sizeof(struct ofp_dpkm_set_key) == 8);

struct ofp_dpkm_test_request {
    ovs_be32 experimenter;
    ovs_be32 subtype;
};
OFP_ASSERT(sizeof(struct ofp_dpkm_test_request) == 8);

struct ofp_dpkm_test_reply {
    ovs_be32 experimenter;
    ovs_be32 subtype;
};
OFP_ASSERT(sizeof(struct ofp_dpkm_test_reply) == 8);

struct ofp_dpkm_add_peer {
    ovs_be32 experimenter;
    ovs_be32 subtype;
    char key[KEY_LEN];
    char ipv4_addr[IP_LEN];
    char ipv4_wg[IP_LEN];
};
OFP_ASSERT(sizeof(struct ofp_dpkm_add_peer) == 328);

struct ofp_dpkm_delete_peer {
    ovs_be32 experimenter;
    ovs_be32 subtype;
    char key[KEY_LEN];
    char ipv4_addr[IP_LEN];
    char ipv4_wg[IP_LEN];
};
OFP_ASSERT(sizeof(struct ofp_dpkm_delete_peer) == 328);

#endif /* openflow/dpkm-ext.h */
