//
//  c4Internal.hh
//  Couchbase Lite Core
//
//  Created by Jens Alfke on 9/15/15.
//  Copyright (c) 2015-2016 Couchbase. All rights reserved.
//

#pragma once

#include "Base.hh"
#include "Error.hh"
#include "RefCounted.hh"
#include <functional>


// Defining C4DB_THREADSAFE as 1 will make C4Database thread-safe: the same handle can be called
// simultaneously from multiple threads. Transactions will be single-threaded: once a thread has
// called c4db_beginTransaction, other threads making that call will block until the transaction
// ends.
#if C4DB_THREADSAFE
#include <mutex>
#endif


struct C4DocEnumerator;

namespace litecore {
    class Database;
    class Document;
}

using namespace std;
using namespace litecore;


// SLICE STUFF:


// Predefine C4Slice as a typedef of slice so we can use the richer slice API:

typedef slice C4Slice;

typedef struct {
    const void *buf;
    size_t size;
} C4SliceResult;


#define kC4SliceNull slice::null


#define C4_IMPL // This tells c4Base.h to skip its declaration of C4Slice
#include "c4Base.h"


namespace c4Internal {

    // ERRORS & EXCEPTIONS:

    void recordError(C4ErrorDomain domain, int code, C4Error* outError) noexcept;
    void recordException(const exception &e, C4Error* outError) noexcept;
    static inline void clearError(C4Error* outError) noexcept {if (outError) outError->code = 0;}

    #define catchError(OUTERR) \
        catch (const exception &x) { \
            recordException(x, OUTERR); \
        }

    #define catchExceptions() \
        catch (const exception &x) { }

    #define checkParam(TEST, OUTERROR) \
        ((TEST) || (recordError(LiteCoreDomain, kC4ErrorInvalidParameter, OUTERROR), false))

    C4SliceResult stringResult(const char *str);

    // DOC ENUMERATORS:

    typedef function<bool(const Document&,
                          uint32_t documentFlags,  // C4DocumentFlags
                          slice docType)> EnumFilter;

    void setEnumFilter(C4DocEnumerator*, EnumFilter);


    // Internal C4EnumeratorFlags value. Includes purged docs (what ForestDB calls 'deleted'),
    // so this is equivalent to litecore::DocEnumerator::includeDeleted.
    // Should only need to be used internally, for the view indexer's enumerator.
    static const uint16_t kC4IncludePurged = 0x8000;

}

using namespace c4Internal;
