#include "kfs_strlen.h"
#include <signal.h>
#include "criterion.h"

Test(strlen, is_defined) {
	kfs_strlen("");
}

Test(strlen, null_string, .signal = SIGSEGV) {
	kfs_strlen(NULL);
}

Test(kfs_strlen, simple_strings) {
	size_t len = kfs_strlen("");
	cr_expect(len == 0);

	len = kfs_strlen("A");
	cr_expect(len == 1);

	len = kfs_strlen("abcdefghijklmnopqrstuvwxyz");
	cr_expect(len == 26);
}
