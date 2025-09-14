#include "criterion.h"
// #include "include_test/test_kfs_strlen.h"
#include "libkfs.h"

Test(strlen, is_defined) {
	kfs_strlen("");
}

Test(kfs_strlen, simple_strings) {
	size_t len = kfs_strlen("");
	cr_expect(len == 0);

	len = kfs_strlen("A");
	cr_expect(len == 1);

	len = kfs_strlen("abcdefghijklmnopqrstuvwxyz");
	cr_expect(len == 26);
}
