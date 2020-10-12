
#ifndef OPENFLOW_DPKM_H
#define OPENFLOW_DPKM_H 1
/*
 * Openflow extensions for the Data Plane Key Management Protocol.
 * This class defines the enumerators and structs representing messages and flags
 * for carrying out specific functionality.
 * Designed to match the format of messages in the floodlight controller.
 */
#include <openflow/openflow.h>
#include <openvswitch/types.h>

/* Initialise experimenter id to identify messages that are part of DPKM. */
#define DPKM_VENDOR 0xa20a0323
#define KEY_LEN 256
#define IP_LEN 32

/* DPKM_STATUS message flags indicating the state of WG and response type. */
enum ofp_dpkm_status_flag {
    OFP_DPKM_STATUS_FLAG_CONFIGURED = 0,   /* WG is configured. */
    OFP_DPKM_STATUS_FLAG_PEER_ADDED = 1,   /* WG peer has been added. */
    OFP_DPKM_STATUS_FLAG_PEER_REMOVED = 2, /* WG peer has been removed. */
    OFP_DPKM_STATUS_FLAG_CONNECTED = 3,    /* WG has a peer connection. */
    OFP_DPKM_STATUS_FLAG_REVOKED = 4,      /* Private key is revoked. */
};

/* Defines structure of DPKM_STATUS message. */
struct ofp_dpkm_status {
    ovs_be32 status_flag;  /* Response type flag based on received message. */
    char key[KEY_LEN];     /* Public Key of the WG interface. */
    char ipv4_addr[IP_LEN];/* IPv4 address of the switch. */
    char ipv4_wg[IP_LEN];  /* IPv4 address of the WG interface. */
    char ipv4_peer[IP_LEN];/* IPv4 address of a peer switch (or blank). */
};
OFP_ASSERT(sizeof(struct ofp_dpkm_status) == 356);

/* Defines structure of DPKM_SET_KEY message. */
struct ofp_dpkm_set_key {
    ovs_be32 experimenter;/* DPKM Experimenter ID: 0xa20a0323. */
    ovs_be32 subtype;     /* Subtype of message (0). */
};
OFP_ASSERT(sizeof(struct ofp_dpkm_set_key) == 8);

/* Defines structure of DPKM_DELETE_KEY message. */
struct ofp_dpkm_delete_key {
    ovs_be32 experimenter;/* DPKM Experimenter ID: 0xa20a0323. */
    ovs_be32 subtype;     /* Subtype of message (1). */
};
OFP_ASSERT(sizeof(struct ofp_dpkm_delete_key) == 8);

/* Defines structure of DPKM_ADD_PEER message. */
struct ofp_dpkm_add_peer {
    ovs_be32 experimenter; /* DPKM Experimenter ID: 0xa20a0323. */
    ovs_be32 subtype;      /* Subtype of message (2). */
    char key[KEY_LEN];     /* Public Key of the peer switch to be added. */
    char ipv4_addr[IP_LEN];/* IPv4 address of the peer switch to be added. */
    char ipv4_wg[IP_LEN];  /* WG IPv4 address of the peer switch to be added. */
};
OFP_ASSERT(sizeof(struct ofp_dpkm_add_peer) == 328);

/* Defines structure of DPKM_DELETE_PEER message. */
struct ofp_dpkm_delete_peer {
    ovs_be32 experimenter; /* DPKM Experimenter ID: 0xa20a0323. */
    ovs_be32 subtype;      /* Subtype of message (3). */
    char key[KEY_LEN];     /* Public Key of the peer switch to be deleted. */
    char ipv4_addr[IP_LEN];/* IPv4 address of the peer switch to be deleted. */
    char ipv4_wg[IP_LEN];  /* WG IPv4 address of the peer switch to be deleted. */
};
OFP_ASSERT(sizeof(struct ofp_dpkm_delete_peer) == 328);

/* Defines structure of DPKM_TEST messages (Only used for testing purpose). */
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

#endif /* openflow/dpkm-ext.h */
