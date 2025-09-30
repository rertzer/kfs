#include "kfs_strrchr.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "criterion.h"

Test(kfs_strrchr, a_char_in_null_string, .signal = SIGSEGV) {
	char* str = NULL;
	char  c = 'A';
	kfs_strrchr(str, c);
}

Test(kfs_strrchr, null_char_in_null_string) {
	char* str = "";
	char  c = '\0';
	char* k = kfs_strrchr(str, c);
	char* l = strrchr(str, c);
	cr_expect(k == l);
}

Test(kfs_strrchr, a_char_in_empty_string) {
	char* str = "";
	char  c = 'a';
	char* k = kfs_strrchr(str, c);
	char* l = strrchr(str, c);
	cr_expect(k == l);
}

Test(kfs_strrchr, a_char_not_in_string) {
	char* str = "bcdef";
	char  c = 'a';
	char* k = kfs_strrchr(str, c);
	char* l = strrchr(str, c);
	cr_expect(k == l);
}

Test(kfs_strrchr, a_char_in_string) {
	char* strs[] = {"abc", "babc", "baabkf", "a", "affffffffa"};
	char  c = 'a';
	for (size_t i = 0; i < sizeof(strs) / sizeof(*strs); ++i) {
		char* k = kfs_strrchr(strs[i], c);
		char* l = strrchr(strs[i], c);
		cr_expect(k == l);
	}
}

Test(kfs_strrchr, null_char_in_string) {
	char* strs[] = {"abc", "babc", "baabkf", "a", "affffffffa"};
	char  c = '\0';
	for (size_t i = 0; i < sizeof(strs) / sizeof(*strs); ++i) {
		char* k = kfs_strrchr(strs[i], c);
		char* l = strrchr(strs[i], c);
		cr_expect(k == l);
	}
}
