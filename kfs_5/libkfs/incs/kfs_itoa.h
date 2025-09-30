#ifndef KFS_ITOA_H
#define KFS_ITOA_H

#ifdef JROS
#define kfs_itoa itoa
#endif

char* kfs_itoa(int value, char* buffer, int radix);

#endif
