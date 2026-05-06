#ifndef ORIENTATION_H_
#define ORIENTATION_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize orientation BLE module
 *
 * Currently optional since the module is auto-started via thread,
 * but kept for future extensibility (e.g., enabling/disabling notifications).
 */
int orientation_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ORIENTATION_H_ */