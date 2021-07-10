/*
  Copyright 2011-2020 David Anderson. All Rights Reserved.

  This trivial test program is hereby placed in the public domain.
*/


#include "config.h"
#include <stdio.h>
#if defined(_WIN32) && defined(HAVE_STDAFX_H)
#include "stdafx.h"
#endif /* HAVE_STDAFX_H */
#ifdef HAVE_STRING_H
#include <string.h>  /* strcpy() strlen() */
#endif
#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif
#include "dwarf.h"
#include "libdwarf.h"
#include "dwconf.h"
#if 0
#include "dwarf_base_types.h"
#include "dwarf_opaque.h"
#include "dwarf_error.h"
#endif

/*  Fake glflags and functions we do not
    use in the test so dwconf.c compiles */
char * makename(const char *x );
const char * sanitized(const char *s);
void sanitized_string_destructor(void);
struct glflags_s {
    int gf_show_dwarfdump_conf;
    int gf_expr_ops_joined;
};
struct glflags_s glflags;
void sanitized_string_destructor(void)
{
}
const char * sanitized(const char *s)
{
    return s;
}
char * makename(const char *x )
{
    return (char *)x;
}


#define CANBUF 25
static struct canap_s {
    char *res_exp;
    char *first;
    char *second;
} canap[] = {
    {
    "ab/c", "ab", "c"}, {
    "ab/c", "ab/", "c"}, {
    "ab/c", "ab", "/c"}, {
    "ab/c", "ab////", "/////c"}, {
    "ab/", "ab", ""}, {
    "ab/", "ab////", ""}, {
    "ab/", "ab////", ""}, {
    "/a", "", "a"}, {
    0, "/abcdefgbijkl", "pqrstuvwxyzabcd"}, {
    0, 0, 0}
};

static int
test_canonical_append(void)
{
    /* Make buf big, this is test code, so be safe. */
    char lbuf[1000];
    unsigned i;
    int failcount = 0;

    printf("Entry test_canonical_append\n");
    for (i = 0;; ++i) {
        char *res = 0;

        if (canap[i].first == 0 && canap[i].second == 0)
            break;

        res = _dwarf_canonical_append(lbuf, CANBUF, canap[i].first,
            canap[i].second);
        if (res == 0) {
            if (canap[i].res_exp == 0) {
                /* GOOD */
                printf("PASS %u\n", i);
            } else {
                ++failcount;
                printf("FAIL: entry %u wrong, expected "
                    "%s, got NULL\n",
                    i, canap[i].res_exp);
            }
        } else {
            if (canap[i].res_exp == 0) {
                ++failcount;
                printf("FAIL: entry %u wrong, got %s "
                    "expected NULL\n",
                    i, res);
            } else {
                if (strcmp(res, canap[i].res_exp) == 0) {
                    printf("PASS %u\n", i);
                    /* GOOD */
                } else {
                    ++failcount;
                    printf("FAIL: entry %u wrong, "
                        "expected %s got %s\n",
                        i, canap[i].res_exp, res);
                }
            }
        }
    }
    if (failcount) {
        printf("FAIL count %u\n", failcount);
    }
    return failcount;
}

int main(void)
{
    int errs = 0;

    errs += test_canonical_append();
    if (errs) {
        printf("FAIL. canonical path test errors\n");
        return 1;
    }
    printf("PASS canonical path tests\n");
    return 0;
}
