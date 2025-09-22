# Peripheral Module Documentation

## Overview

The Peripheral module provides a thread-safe interface for controlling LEDs and handling button presses on the nRF5340 Development Kit. It implements a robust architecture using Zephyr RTOS features such as work queues, atomic operations, and GPIO drivers.

## Features

- **Thread-safe interface** for LED control and button event handling
- **Dedicated work queue** for asynchronous event processing
- **Software debouncing** for buttons to prevent spurious events
- **Non-blocking LED control** functions with configurable parameters
- **ZBUS messaging system** for event-driven communication
- **Comprehensive error handling** and logging

## Architecture

The module is designed with the following components:

1. **GPIO Configuration**: Uses Device Tree aliases for hardware independence
2. **Work Queue**: Dedicated thread for asynchronous event processing
3. **Button Debounce**: Software debouncing to filter out spurious button presses
4. **LED Control**: Thread-safe API for controlling LEDs
5. **ZBUS Messaging**: Event-driven communication system

## LED Control API

### LED Identifiers

```c
enum led_t {
  LED_1, /* Board RED */
  LED_2, /* Board GREEN */
  LED_3, /* Board BLUE */
  LED_4, /* Light RED */
};
```

### LED States

```c
typedef enum {
  LED_OFF = 1, /* LED is turned off (not illuminated) */
  LED_ON = 0,  /* LED is turned on (illuminated) */
} led_state_t;
```

### Basic LED Control

```c
int peripheral_set_led(enum led_t led, led_state_t state);
```

This function provides direct control over an LED's state (on or off). It maps the logical LED identifier to the corresponding GPIO pin and sets its state accordingly.

**Parameters:**
- `led`: The LED to control (LED_1 through LED_4)
- `state`: Desired state: LED_ON to turn on, LED_OFF to turn off

**Return Values:**
- 0 on success
- -EINVAL if the LED identifier is invalid
- Other negative values from the GPIO driver if the operation fails

### Simple LED Blinking

```c
int peripheral_set_led_blink(enum led_t led);
```

This is a simplified version of the LED blinking functionality that uses default parameters for a single blink cycle (500ms on, 500ms off).

**Parameters:**
- `led`: The LED to blink (LED_1 through LED_4)

**Return Values:**
- 0 on success
- -EINVAL if the LED identifier is invalid
- -EBUSY if the work queue is full
- Other negative values from the GPIO driver if the operation fails

### Advanced LED Blinking

```c
int peripheral_set_led_blink_async(
    enum led_t led, uint8_t count, uint16_t on_time_ms, uint16_t off_time_ms, led_state_t end_state);
```

This function provides a non-blocking way to blink an LED with precise control over the number of blink cycles, duration of ON and OFF states, and final state after blinking completes.

**Parameters:**
- `led`: The LED to blink (LED_1 through LED_4)
- `count`: Number of complete on-off cycles (0 for indefinite blinking)
- `on_time_ms`: Duration in milliseconds for the ON state
- `off_time_ms`: Duration in milliseconds for the OFF state
- `end_state`: Final state of the LED after blinking completes (LED_ON or LED_OFF)

**Return Values:**
- 0 on success
- -EINVAL if the LED identifier is invalid
- -EBUSY if the work queue is full
- Other negative values from the GPIO driver if the operation fails

### ZBUS LED Task Publishing

```c
int peripheral_publish_led_task(enum peripheral_event event);
```

This function publishes a message to the LED task channel, which will be processed by the LED task listener callback.

**Parameters:**
- `event`: The peripheral event to publish (should be an LED-related event)

**Return Values:**
- 0 on success
- Negative errno on failure

## Button Handling

### Button Events

```c
enum button_event_t {
  BUTTON_1_PRESSED, /* First button press event */
  BUTTON_2_PRESSED, /* Second button press event */
  BUTTON_3_PRESSED, /* Third button press event */
  BUTTON_4_PRESSED, /* Fourth button press event */
};
```

### Button Event Handling

The module uses a callback-based approach for button event handling. When a button press is detected and debounced, the corresponding event is published to the button event channel on ZBUS.

Button presses are debounced with a 50ms delay to prevent spurious events caused by mechanical button bounce.

## ZBUS Messaging System

The module uses ZBUS for event-driven communication. There are two main channels:

1. **Button Event Channel**: Publishes button press events
2. **LED Task Channel**: Receives LED control commands

### Button Event Message

```c
struct btn_event_msg {
  enum button_event_t event;
};
```

### LED Task Message

```c
struct led_task_msg {
  enum peripheral_event event;
};
```

### Peripheral Events

```c
enum peripheral_event {
  LED_1_ON,     /* Turn on LED 1 */
  LED_1_OFF,    /* Turn off LED 1 */
  LED_1_BLINK,  /* Blink LED 1 */
  LED_2_ON,     /* Turn on LED 2 */
  LED_2_OFF,    /* Turn off LED 2 */
  LED_2_BLINK,  /* Blink LED 2 */
  LED_3_ON,     /* Turn on LED 3 */
  LED_3_OFF,    /* Turn off LED 3 */
  LED_3_BLINK,  /* Blink LED 3 */
  LED_4_ON,     /* Turn on LED 4 */
  LED_4_OFF,    /* Turn off LED 4 */
  LED_4_BLINK,  /* Blink LED 4 */
  BTN_1,        /* Button 1 pressed event */
  BTN_2,        /* Button 2 pressed event */
  BTN_3,        /* Button 3 pressed event */
  BTN_4,        /* Button 4 pressed event */
};
```

## Usage Examples

### Basic LED Control

```c
// Initialize the peripheral module
peripheral_init();

// Turn on LED 1
peripheral_set_led(LED_1, LED_ON);

// Turn off LED 2
peripheral_set_led(LED_2, LED_OFF);

// Blink LED 3 once with default timing (500ms on, 500ms off)
peripheral_set_led_blink(LED_3);

// Blink LED 4 three times with custom timing (200ms on, 300ms off)
// and leave it on when finished
peripheral_set_led_blink_async(LED_4, 3, 200, 300, LED_ON);
```

### Using ZBUS for LED Control

```c
// Turn on LED 1 using ZBUS
peripheral_publish_led_task(LED_1_ON);

// Blink LED 2 using ZBUS
peripheral_publish_led_task(LED_2_BLINK);
```

## Implementation Details

### Work Queue

The module uses a dedicated work queue to handle all events asynchronously. This design provides several benefits:
- Button callbacks (which run in interrupt context) return quickly, improving system responsiveness
- LED operations don't block the calling thread, allowing for concurrent operations
- Multiple events can be queued and processed in sequence
- Work items can be scheduled with delays for timing-sensitive operations (like LED blinking)

### Button Debounce

The button debounce mechanism solves several common issues with physical buttons:
- Prevents multiple events from being generated when a button bounces during press or release
- Ensures consistent behavior across different button types and conditions
- Reduces CPU load by filtering out spurious button events
- Improves user experience by providing predictable button response

### LED Blinking

The LED blink implementation provides:
- Non-blocking operation using Zephyr's delayable work items
- Support for configurable on/off durations for precise timing control
- Ability to specify the number of blink cycles (including infinite blinking)
- Configurable final LED state when blinking completes
- Independent control of multiple LEDs simultaneously

## Error Handling

All public API functions return error codes to indicate success or failure:
- 0 indicates success
- Negative values indicate errors, following Zephyr's errno conventions
- Common error codes include -EINVAL (invalid parameter) and -EBUSY (resource busy)

## Thread Safety

The module is designed to be thread-safe:
- Uses atomic operations for work item allocation
- Properly handles concurrent access to shared resources
- Can be safely called from any context, including interrupt handlers and other threads