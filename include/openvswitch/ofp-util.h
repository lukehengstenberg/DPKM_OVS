/*
 * Copyright (c) 2008-2017 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OPENVSWITCH_OFP_UTIL_H
#define OPENVSWITCH_OFP_UTIL_H 1

#include <stdbool.h>
#include <stdint.h>
#include "openvswitch/ofp-protocol.h"
#include "openflow/dpkm-ext.h"

struct ofp_header;

#ifdef __cplusplus
extern "C" {
#endif

bool ofputil_decode_hello(const struct ofp_header *,
                          uint32_t *allowed_versions);
struct ofpbuf *ofputil_encode_hello(uint32_t version_bitmap);
void ofputil_hello_format(struct ds *, const struct ofp_header *);

struct ofpbuf *ofputil_encode_echo_request(enum ofp_version);
struct ofpbuf *ofputil_encode_echo_reply(const struct ofp_header *);

#define KEY_LEN 256
#define IP_LEN 32
/* Abstract ofp_dpkm_test_request and reply. */
struct ofputil_dpkm_test_request {
    uint32_t experimenter;
    uint32_t subtype;
};

/* Abstract of ofp_dpkm_set_key. */
struct ofputil_dpkm_set_key {
    uint32_t experimenter;
    uint32_t subtype;
};

/* Abstract of ofp_dpkm_delete_key. */
struct ofputil_dpkm_delete_key {
    uint32_t experimenter;
    uint32_t subtype;
};

/* Abstract of ofp_dpkm_add_peer. */
struct ofputil_dpkm_add_peer {
    uint32_t experimenter;
    uint32_t subtype;
    char key[KEY_LEN];
    char ipv4_addr[IP_LEN];
    char ipv4_wg[IP_LEN];
};

/* Abstract of ofp_dpkm_delete_peer. */
struct ofputil_dpkm_delete_peer {
    uint32_t experimenter;
    uint32_t subtype;
    char key[KEY_LEN];
    char ipv4_addr[IP_LEN];
    char ipv4_wg[IP_LEN];
};

/* Abstract of ofp_dpkm_status. */
struct ofputil_dpkm_status {
    uint32_t experimenter;
    uint32_t subtype;
    enum ofp_dpkm_status_flag status_flag;
    char key[KEY_LEN];
    char ipv4_addr[IP_LEN];
    char ipv4_wg[IP_LEN];
    char ipv4_peer[IP_LEN];
};

int test_if_working(void);
enum ofperr ofputil_decode_dpkm_test_message(const struct ofp_header *,
                                        struct ofputil_dpkm_test_request *);
void ofputil_format_dpkm_test_message(struct ds *,
                                 const struct ofputil_dpkm_test_request *);
struct ofpbuf *ofputil_encode_dpkm_test_reply(const struct ofp_header *,
                                         const struct ofputil_dpkm_test_request *);

enum ofperr ofputil_decode_dpkm_set_key(const struct ofp_header *,
                                    struct ofputil_dpkm_set_key *);
enum ofperr ofputil_decode_dpkm_delete_key(const struct ofp_header *,
                                    struct ofputil_dpkm_delete_key *);
enum ofperr ofputil_decode_dpkm_add_peer(const struct ofp_header *,
                                    struct ofputil_dpkm_add_peer *);
enum ofperr ofputil_decode_dpkm_delete_peer(const struct ofp_header *,
                                       struct ofputil_dpkm_delete_peer *);

struct ofpbuf *ofputil_encode_barrier_request(enum ofp_version);

#ifdef __cplusplus
}
#endif

#endif /* ofp-util.h */
