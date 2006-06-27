/* vgpreloaded wrapper functions used to modify dyncomp's
   behavior. These run on the guest CPU, so they need to use client
   requests if they want to modify Dyncomp's behavior beyond what
   regular code can do. */

#include "valgrind.h"

/* Return a word-sized value with the same value as the argument, but
   a different tag, via loopholes in DynComp's checking. Perhaps this
   is excessively clever, but adding a client request would be a
   pain. -SMcC */
static long tag_launder_long(long x) {
    long y = 0;
    unsigned i;
    for (i = 0; i < 8*sizeof(long); i++) {
	if (x & (1 << i))
	    y |= 1 << i;
    }
    return y;
}

/* glibc's __libc_start_main does the moral equivalent of "environ =
   argv[argc + 1]", but it's unintuitive for argc and argv to always
   be comparable, so launder argc's tag. Note that argc and argv will
   often still end up comparable if the program actually looks at its
   arguments, since it's common to index argv by a value derived from
   argc. */
int I_WRAP_SONAME_FNNAME_ZU(NONE, main)(int argc, char **argv, char **env);
int I_WRAP_SONAME_FNNAME_ZU(NONE, main)(int argc, char **argv, char **env) {
    OrigFn fn;
    int result;
    VALGRIND_GET_ORIG_FN(fn);
    argc = tag_launder_long(argc);
    CALL_FN_W_WWW(result, fn, argc, argv, env);
    return result;
}

/* For ostream operators that do integer->ASCII conversion, make a
   fresh tag for the argument so that interactions caused by having a
   single digit lookup table don't cause every value printed to be
   considered as interacting. */

/* std::basic_ostream<char, std::char_traits<char> >::operator<<(int) */
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEi)
                             (void *this_ptr, int arg);
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEi)
                             (void *this_ptr, int arg) {
    OrigFn fn;
    void *result;
    VALGRIND_GET_ORIG_FN(fn);
    arg = tag_launder_long(arg);
    CALL_FN_W_WW(result, fn, this_ptr, arg);
    return result;
}

/* std::basic_ostream<...>::operator<<(unsigned int) */
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEj)
                             (void *this_ptr, unsigned int arg);
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEj)
                             (void *this_ptr, unsigned int arg) {
    OrigFn fn;
    void *result;
    VALGRIND_GET_ORIG_FN(fn);
    arg = tag_launder_long(arg);
    CALL_FN_W_WW(result, fn, this_ptr, arg);
    return result;
}

/* std::basic_ostream<char, std::char_traits<char> >::operator<<(long) */
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEl)
                             (void *this_ptr, long arg);
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEl)
                             (void *this_ptr, long arg) {
    OrigFn fn;
    void *result;
    VALGRIND_GET_ORIG_FN(fn);
    arg = tag_launder_long(arg);
    CALL_FN_W_WW(result, fn, this_ptr, arg);
    return result;
}

/* std::basic_ostream<...>::operator<<(unsigned long) */
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEm)
                             (void *this_ptr, unsigned long arg);
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEm)
                             (void *this_ptr, unsigned long arg) {
    OrigFn fn;
    void *result;
    VALGRIND_GET_ORIG_FN(fn);
    arg = tag_launder_long(arg);
    CALL_FN_W_WW(result, fn, this_ptr, arg);
    return result;
}

/* std::basic_ostream<char, std::char_traits<char> >::operator<<(short) */
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEs)
                             (void *this_ptr, short arg);
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEs)
                             (void *this_ptr, short arg) {
    OrigFn fn;
    void *result;
    VALGRIND_GET_ORIG_FN(fn);
    arg = tag_launder_long(arg);
    CALL_FN_W_WW(result, fn, this_ptr, arg);
    return result;
}

/* std::basic_ostream<...>::operator<<(unsigned short) */
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEt)
                             (void *this_ptr, unsigned short arg);
void *I_WRAP_SONAME_FNNAME_ZU(libstdcZpZpZa, _ZNSolsEt)
                             (void *this_ptr, unsigned short arg) {
    OrigFn fn;
    void *result;
    VALGRIND_GET_ORIG_FN(fn);
    arg = tag_launder_long(arg);
    CALL_FN_W_WW(result, fn, this_ptr, arg);
    return result;
}

/* XXX Should support float, double, long double, and long long too,
   but I'm not confident how to pass them through CALL_FN safely, nor
   be 64-bit clean. */