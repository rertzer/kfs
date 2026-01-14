#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "criterion.h"

#include "kfs_strrchr.h"

static void kfs_strrchr_assert(char* str, char c);
static void kfs_strrchr_lst_assert(char** str, char c, size_t len);

static void kfs_strrchr_assert(char* str, char c) {
	char* k = kfs_strrchr(str, c);
	char* l = strrchr(str, c);
	cr_expect(k == l);
}

static void kfs_strrchr_lst_assert(char** str, char c, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		kfs_strrchr_assert(str[i], c);
	}
}

Test(kfs_strrchr, a_char_in_null_string, .signal = SIGSEGV) {
	char* str = NULL;
	kfs_strrchr(str, 'A');
}

Test(kfs_strrchr, null_char_in_null_string) {
	char* str = "";
	kfs_strrchr_assert(str, '\0');
}

Test(kfs_strrchr, a_char_in_empty_string) {
	char* str = "";
	kfs_strrchr_assert(str, 'a');
}

Test(kfs_strrchr, a_char_not_in_string) {
	char* str = "bcdef";
	kfs_strrchr_assert(str, 'a');
}

Test(kfs_strrchr, a_char_in_string) {
	char* strs[] = {"abc", "babc", "baabkf", "a", "affffffffa"};
	kfs_strrchr_lst_assert(strs, 'a', 5);
}

Test(kfs_strrchr, null_char_in_string) {
	char* strs[] = {"abc", "babc", "baabkf", "a", "affffffffa"};
	kfs_strrchr_lst_assert(strs, '\0', 5);
}
