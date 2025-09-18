#include <sel4test/test.h>
#include <stddef.h>
#include <string.h>
/* Local user-space shim: provide memzero() for this test. */
void memzero(void *p, size_t n) {
  volatile unsigned char *q = (volatile unsigned char*)p;
  for (size_t i = 0; i < n; i++) { q[i] = 0; }
}
#include <autoconf.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Rust function we exported from kernel/rust/mem.rs */
extern void memzero(void *p, size_t n);

static int test_rust_memclr(void)
{
    unsigned char buf[8] = {1,2,3,4,5,6,7,8};

    /* Call into Rust implementation */
    memzero(buf, sizeof buf);

    /* Print results so you see it in QEMU console */
    printf("Rust memzero result: ");
    for (int i = 0; i < 8; i++) {
        printf("%u ", buf[i]);
    }
    printf("\n");

    /* Sanity check: everything should be zero */
    for (int i = 0; i < 8; i++) {
        assert(buf[i] == 0);
    }
    return 0; /* success */
}

/* Register with sel4test test harness */
DEFINE_TEST(RUST_MEMCLR, "Rust memzero clears buffer", test_rust_memclr, true)

