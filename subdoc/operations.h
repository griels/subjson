#ifndef SUBDOC_OPERATIONS_H
#define SUBDOC_OPERATIONS_H
#include "subdoc-api.h"
#include "path.h"
#include "match.h"
#include "subdoc-util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /* Private; malloc'd because this block is pretty big (several k) */
    subdoc_PATH *path;
    /* cached JSON parser */
    jsonsl_t jsn;

    subdoc_MATCH match;
    /* opcode */
    uint8_t optype;

    /* Location of original document */
    subdoc_LOC doc_cur;
    /* Location of the user's "Value" (if applicable) */
    subdoc_LOC user_in;
    /* Location of the fragments consisting of the _new_ value */
    subdoc_LOC doc_new[8];
    /* Number of fragments active */
    size_t doc_new_len;

    /* Backing buffer for any of our own (in-library) required storage */
    subdoc_STRING bkbuf_extra;

    /* Backing buffer for various tokens we might need to insert */
    char numbufs[32];
} subdoc_OPERATION;

subdoc_OPERATION *
subdoc_op_alloc(void);

void
subdoc_op_clear(subdoc_OPERATION *);

void
subdoc_op_free(subdoc_OPERATION*);

#define SUBDOC_OP_SETVALUE(op, val, nval) do { \
    (op)->user_in.at = val; \
    (op)->user_in.length = nval; \
} while (0);

#define SUBDOC_OP_SETDOC(op, doc, ndoc) do { \
    (op)->doc_cur.at = doc; \
    (op)->doc_cur.length = ndoc; \
} while (0);

#define SUBDOC_OP_SETCODE(op, code) do { \
    (op)->optype = code; \
} while (0);

uint16_t
subdoc_op_exec(subdoc_OPERATION *op, const char *pth, size_t npth);

const char *
subdoc_strerror(uint16_t rc);

#ifdef __cplusplus
}
#endif
#endif
