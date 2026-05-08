#ifndef PRINT_H
#define PRINT_H

#include <app_version.h>
#include <zephyr/sys/printk.h>

#define PRINT_TIRESIAS_TEXT()                                                                                          \
  do {                                                                                                                 \
    printk("Tireisas ");                                                                                               \
  } while (0)

#define PRINT_TIRESIAS_BANNER()                                                                                        \
  do {                                                                                                                 \
    printk(" _____ _               _            \n");                                                                  \
    printk("|_   _(_)_ __ ___  ___(_) __ _ ___  \n");                                                                  \
    printk("  | | | | '__/ _ \\/ __| |/ _` / __| \n");                                                                 \
    printk("  | | | | | |  __/\\__ \\ | (_| \\__ \\ \n");                                                              \
    printk("  |_| |_|_|  \\___||___/_|\\__,_|___/ ");                                                                  \
  } while (0)

#define PRINT_FIRMWARE_VERSION()                                                                                       \
  do {                                                                                                                 \
    printk("v%s\n\n", APP_VERSION_EXTENDED_STRING);                                                                    \
  } while (0)

#endif // PRINT_H
