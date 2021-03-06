//
// c4Listener.h
//
// Copyright (c) 2017 Couchbase, Inc All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once
#include "c4Base.h"

#ifdef __cplusplus
extern "C" {
#endif

    /** \defgroup Listener  Network Listener: REST API and Sync Server
        @{ */


    /** Flags indicating which network API(s) to serve. */
    typedef C4_OPTIONS(unsigned, C4ListenerAPIs) {
        kC4RESTAPI = 0x01,              ///< CouchDB-like REST API
        kC4SyncAPI = 0x02               ///< Replication server
    };


    /** Different ways to provide TLS private keys. */
    typedef C4_ENUM(unsigned, C4PrivateKeyRepresentation) {
        kC4PrivateKeyFromCert,          ///< Key in secure storage, associated with certificate
        kC4PrivateKeyData,              ///< PEM or DER data (may be PKCS12-encrypted)
    };


    /** TLS configuration for C4Listener. */
    typedef struct C4TLSConfig {
        C4PrivateKeyRepresentation privateKeyRepresentation; ///< Interpretation of `privateKey`
        C4Slice privateKey;             ///< Private key data
        C4String privateKeyPassword;    ///< Password to decrypt private key data
        C4Slice certificate;            ///< X.509 certificate data
        bool requireClientCerts;        ///< True to require clients to authenticate with a cert
        C4Slice rootClientCerts;        ///< Root CA certs to trust when verifying client cert
    } C4TLSConfig;


    /** Configuration for a C4Listener. */
    typedef struct C4ListenerConfig {
        uint16_t port;                  ///< TCP port to listen on
        C4ListenerAPIs apis;            ///< Which API(s) to enable
        C4TLSConfig* tlsConfig;         ///< TLS configuration, or NULL for no TLS

        // For REST listeners only:
        C4String directory;             ///< Directory where newly-PUT databases will be created
        bool allowCreateDBs;            ///< If true, "PUT /db" is allowed
        bool allowDeleteDBs;            ///< If true, "DELETE /db" is allowed

        // For sync listeners only:
        bool allowPush;
        bool allowPull;
    } C4ListenerConfig;


    /** Returns flags for the available APIs in this build (REST, sync, or both.) */
    C4ListenerAPIs c4listener_availableAPIs(void) C4API;

    /** Starts a new listener. */
    C4Listener* c4listener_start(const C4ListenerConfig *config C4NONNULL, C4Error *error) C4API;

    /** Closes and disposes a listener. */
    void c4listener_free(C4Listener *listener) C4API;

    /** Makes a database available from the network, under the given name. */
    bool c4listener_shareDB(C4Listener *listener C4NONNULL,
                            C4String name,
                            C4Database *db C4NONNULL) C4API;

    /** Makes a previously-shared database unavailable. */
    bool c4listener_unshareDB(C4Listener *listener C4NONNULL,
                              C4String name) C4API;


    /** A convenience that, given a filesystem path to a database, returns the database name
        for use in an HTTP URI path. */
    C4StringResult c4db_URINameFromPath(C4String path) C4API;

#ifdef __cplusplus
}
#endif
