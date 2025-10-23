/**
 * @file controller.c
 * @brief Main application controller implementation
 *
 * This module implements the main application controller that manages the overall
 * system state and coordinates between different services and modules. It uses
 * a state machine approach with ZBUS for inter-module communication.
 *
 * @author Tiresias Firmware Team
 * @version 1.0
 */

#include "controller.h"
#include "../services/audio_codec/audio_codec.h"
#include "../services/bluetooth/bluetooth.h"
#include "../system_modules/peripheral/peripheral.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

LOG_MODULE_REGISTER(controller_module, CONFIG_LOG_DEFAULT_LEVEL);

/* === Configuration Constants === */
#define CONTROLLER_THREAD_STACK_SIZE 1024
#define CONTROLLER_THREAD_PRIORITY 3
#define ZBUS_TIMEOUT_MS 100

/* === Error checker for controller === */
#define CONTROLLER_CHECK_ERROR(ret, msg)                                                                               \
  do {                                                                                                                 \
    if (ret != 0) {                                                                                                    \
      LOG_ERR(msg " :%d", ret);                                                                                        \
      set_controller_state(CONTROLLER_STATE_ERROR);                                                                    \
      BOARD_RED();                                                                                                     \
      return;                                                                                                          \
    }                                                                                                                  \
  } while (0)

/* === ZBUS Communication Setup === */
ZBUS_SUBSCRIBER_DEFINE(controller_sub, 8);
ZBUS_CHAN_DECLARE(btn_event_chan, bluetooth_state_chan, codec_state_chan);

ZBUS_CHAN_DEFINE(controller_event_chan, /* Name */
    controller_event_chan_msg, /* Message type */
    NULL, /* Validator */
    NULL, /* User data */
    ZBUS_OBSERVERS(controller_sub), /* No static observers */
    ZBUS_MSG_INIT(.event = CONTROLLER_EVENT_INIT) /* Initial value */
);

/* === State Management === */

/**
 * @brief Current controller state
 *
 * This variable holds the current state of the controller state machine.
 * Access should be controlled through the set_controller_state() function.
 */
static controller_state current_state = CONTROLLER_STATE_OFF;

/**
 * @brief Set the controller state with validation
 *
 * Updates the controller state and logs the transition. Prevents redundant
 * state changes and provides state transition logging for debugging.
 *
 * @param state The new controller state to transition to
 */
static void set_controller_state(controller_state state)
{
  if (current_state == state) {
    LOG_DBG("State already set to %d, ignoring", state);
    return;
  }

  LOG_INF("State transition: %d -> %d", current_state, state);
  current_state = state;
}

/**
 * @brief Get the current controller state
 *
 * @return controller_state Current state of the controller
 */
controller_state controller_get_state(void) { return current_state; }

/**
 * @brief Check if the controller is in an operational state
 *
 * @return true if controller is ready for operation, false otherwise
 */
bool controller_is_operational(void)
{
  return (current_state == CONTROLLER_STATE_IDLE || current_state == CONTROLLER_STATE_STD_OP
      || current_state == CONTROLLER_STATE_STREAMING);
}

/* === State Handler Functions === */

/**
 * @brief Handle OFF state
 *
 * Initializes the system by sending initialization commands to Bluetooth and
 * audio codec services, sets up ZBUS observers, and transitions to INITIALIZING state.
 *
 * @param chan ZBUS channel that triggered this state handler (unused in OFF state)
 */
static void handle_state_off(struct zbus_channel* chan)
{
  int ret;

  LOG_INF("Initializing system services");

  /* Initialize Bluetooth service */
  ret = bluetooth_send_command(BLUETOOTH_CMD_INIT);
  CONTROLLER_CHECK_ERROR(ret, "Failed to send Bluetooth init command");

  /* Initialize audio codec service */
  ret = codec_send_command(CODEC_CMD_INIT);
  CONTROLLER_CHECK_ERROR(ret, "Failed to send codec init command");

  /* Subscribe to service state channels */
  ret = zbus_chan_add_obs(&bluetooth_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
  CONTROLLER_CHECK_ERROR(ret, "Failed to subscribe to Bluetooth state channel");

  ret = zbus_chan_add_obs(&codec_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
  CONTROLLER_CHECK_ERROR(ret, "Failed to subscribe to codec state channel");

  /* Transition to initializing state */
  set_controller_state(CONTROLLER_STATE_INITIALIZING);
}

/**
 * @brief Handle INITIALIZING state
 *
 * Monitors the initialization progress of Bluetooth and audio codec services.
 * Transitions to IDLE state when both services are ready, or ERROR state if
 * any service fails to initialize.
 *
 * @param chan ZBUS channel that triggered this state handler
 */
static void handle_state_initializing(struct zbus_channel* chan)
{
  bluetooth_state bt_state;
  codec_state cd_state;
  int ret;

  /* Read Bluetooth service state */
  ret = zbus_chan_read(&bluetooth_state_chan, &bt_state, K_MSEC(ZBUS_TIMEOUT_MS));
  CONTROLLER_CHECK_ERROR(ret, "Failed to read Bluetooth state");

  /* Check if Bluetooth is still initializing */
  if (bt_state == BLUETOOTH_STATE_INITIALIZING) {
    LOG_DBG("Bluetooth service still initializing");
    return;
  }

  /* Handle Bluetooth initialization failure */
  if (bt_state == BLUETOOTH_STATE_INIT_ERROR) {
    LOG_ERR("Bluetooth initialization failed");
    set_controller_state(CONTROLLER_STATE_ERROR);
    BOARD_RED();
    (void)zbus_chan_rm_obs(&bluetooth_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    (void)zbus_chan_rm_obs(&codec_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    return;
  }

  /* Read audio codec service state */
  ret = zbus_chan_read(&codec_state_chan, &cd_state, K_MSEC(ZBUS_TIMEOUT_MS));
  CONTROLLER_CHECK_ERROR(ret, "Failed to read codec state");

  /* Check if codec is still initializing */
  if (cd_state == CODEC_STATE_INITIALIZING) {
    LOG_DBG("Audio codec service still initializing");
    return;
  }

  /* Handle codec initialization failure */
  if (cd_state == CODEC_STATE_ERROR) {
    LOG_ERR("Audio codec initialization failed");
    set_controller_state(CONTROLLER_STATE_ERROR);
    BOARD_RED();
    (void)zbus_chan_rm_obs(&bluetooth_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    (void)zbus_chan_rm_obs(&codec_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    return;
  }

  LOG_DBG("Service states - Bluetooth: %d, Codec: %d", bt_state, cd_state);

  /* Check if both services are ready */
  if (bt_state == BLUETOOTH_STATE_NOT_CONNECTED && cd_state == CODEC_STATE_IDLE) {
    LOG_INF("System initialization complete");
    set_controller_state(CONTROLLER_STATE_IDLE);
    ret = peripheral_publish_led_task(LED_2_BLINK);

    /* Unsubscribe from state channels during normal operation */
    (void)zbus_chan_rm_obs(&bluetooth_state_chan, &controller_sub, K_MSEC(ZBUS_TIMEOUT_MS));
    return;
  }
}

/**
 * @brief Handle IDLE state
 *
 * Processes user input events (button presses) and initiates appropriate actions.
 * Currently handles button 3 press to start Bluetooth advertising.
 *
 * @param chan ZBUS channel that triggered this state handler
 */
static void handle_state_idle(struct zbus_channel* chan)
{
  int ret;

  /* Handle button events */
  if (chan == &btn_event_chan) {
    button_event_t btn_event;

    ret = zbus_chan_read(chan, &btn_event, K_MSEC(ZBUS_TIMEOUT_MS));
    CONTROLLER_CHECK_ERROR(ret, "Failed to read button event");

    LOG_INF("Button event received: %d", btn_event);

    /* Handle button 3 press - start advertising */
    if (btn_event == BUTTON_3_PRESSED) {
      ret = bluetooth_send_command(BLUETOOTH_CMD_ADVERTISE);
      CONTROLLER_CHECK_ERROR(ret, "Failed to send Bluetooth advertise command");
      return;
    }
  }
}

/**
 * @brief Handle LOW_POWER state
 *
 * Manages system behavior in low power mode. Currently a placeholder for
 * future power management implementation.
 *
 * @param chan ZBUS channel that triggered this state handler
 */
static void handle_state_low_power(struct zbus_channel* chan)
{
  /* TODO: Implement low power mode handling */
  LOG_DBG("Low power state handler called");
}

/**
 * @brief Handle STD_OP state
 *
 * Manages standard operation mode. Currently a placeholder for future
 * standard operation implementation.
 *
 * @param chan ZBUS channel that triggered this state handler
 */
static void handle_state_std_op(struct zbus_channel* chan)
{
  /* TODO: Implement standard operation handling */
  LOG_DBG("Standard operation state handler called");
}

/**
 * @brief Handle ERROR state
 *
 * Manages system behavior in error conditions. Currently a placeholder for
 * future error recovery implementation.
 *
 * @param chan ZBUS channel that triggered this state handler
 */
static void handle_state_error(struct zbus_channel* chan)
{
  /* TODO: Implement error state handling and recovery */
  LOG_DBG("Error state handler called");
}

/**
 * @brief Handle STREAMING state
 *
 * Manages audio streaming operations. Currently a placeholder for future
 * audio streaming implementation.
 *
 * @param chan ZBUS channel that triggered this state handler
 */
static void handle_state_streaming(struct zbus_channel* chan)
{
  /* TODO: Implement audio streaming handling */
  LOG_DBG("Streaming state handler called");
}

/* === State Machine Implementation === */

/**
 * @brief Main controller state machine
 *
 * Dispatches events to appropriate state handlers based on the current controller state.
 * This function is called whenever a ZBUS event is received by the controller.
 *
 * @param chan ZBUS channel that triggered the state machine
 */
static void controller_state_machine(struct zbus_channel* chan)
{
  if (chan == NULL) {
    LOG_ERR("Invalid channel pointer");
    return;
  }

  switch (current_state) {
  case CONTROLLER_STATE_OFF:
    handle_state_off(chan);
    break;
  case CONTROLLER_STATE_INITIALIZING:
    handle_state_initializing(chan);
    break;
  case CONTROLLER_STATE_IDLE:
    handle_state_idle(chan);
    break;
  case CONTROLLER_STATE_LOW_POWER:
    handle_state_low_power(chan);
    break;
  case CONTROLLER_STATE_STD_OP:
    handle_state_std_op(chan);
    break;
  case CONTROLLER_STATE_ERROR:
    handle_state_error(chan);
    break;
  case CONTROLLER_STATE_STREAMING:
    handle_state_streaming(chan);
    break;
  default:
    LOG_ERR("Unknown controller state: %d", current_state);
    set_controller_state(CONTROLLER_STATE_ERROR);
    BOARD_RED();
    break;
  }
}

/* === Thread Implementation === */

/**
 * @brief Main controller thread function
 *
 * This is the main thread that handles all controller operations. It waits for
 * ZBUS events and processes them through the state machine. The thread runs
 * continuously and handles system coordination.
 */
static void controller_thread(void)
{
  struct zbus_channel* chan;
  int ret;

  LOG_INF("Controller thread started");

  /* Main event processing loop */
  while (1) {
    ret = zbus_sub_wait(&controller_sub, &chan, K_FOREVER);
    if (ret != 0) {
      LOG_ERR("Failed to wait for controller event: %d", ret);
      continue;
    }

    /* Process the event through the state machine */
    controller_state_machine(chan);
  }
}

int controller_init(void)
{
  int ret = zbus_chan_notify(&controller_event_chan, K_NO_WAIT);
  CONTROLLER_CHECK_ERROR(ret, "Failed to notify controller event channel");
  return ret;
}

/* === Thread Definition === */

/**
 * @brief Static thread definition for the controller
 *
 * Defines the controller thread with specified stack size and priority.
 * The thread starts automatically when the system boots.
 */
K_THREAD_DEFINE(controller_thread_id, CONTROLLER_THREAD_STACK_SIZE, controller_thread, NULL, NULL, NULL,
    CONTROLLER_THREAD_PRIORITY, 0, 0);