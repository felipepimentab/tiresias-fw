# Firmware Architecture

## Overview

The Tiresias firmware is built on the Zephyr RTOS and follows a layered architecture with clear separation of concerns. The system is designed to be modular, maintainable, and extensible, with a focus on real-time performance and reliability for embedded applications.

The architecture follows a hierarchical structure with the following main components:

1. **Drivers Layer**: Hardware abstraction for direct device interaction
2. **Modules Layer**: Lightweight, synchronous components for specific functionalities
3. **Services Layer**: Complex, often asynchronous components with state machines
4. **Application Layer**: High-level business logic and system coordination

Communication between components is primarily achieved through the Zephyr Bus (ZBUS) messaging system, which provides a publish-subscribe pattern for decoupled communication.

## System Layers

### Drivers Layer

The drivers layer provides direct hardware abstraction and interfaces with physical devices. It encapsulates hardware-specific details and presents a clean API to higher layers.

Key components:
- **ADAU1787 Driver**: Controls the audio codec hardware
- **SigmaStudio Integration**: Provides DSP configuration and control

### Modules Layer

Modules are lightweight, focused components that provide specific functionality. They typically:
- Have minimal dependencies
- Provide synchronous operations
- Focus on a single responsibility
- Are resource-efficient

Key modules:
- **Peripheral Module**: Manages LEDs, buttons, and other peripheral devices
- **Connection Module**: Provides a lightweight BLE connectivity interface
- **Storage Module**: Handles persistent data storage
- **I2S Control Module**: Manages audio I2S interface

### Services Layer

Services are more complex components that often:
- Implement state machines
- Run in dedicated threads
- Use message-based communication
- Handle asynchronous operations
- Integrate multiple modules

Key services:
- **BLE Service**: Manages Bluetooth connectivity with a state machine
- **Audio Codec Service**: Controls audio processing and streaming

### Application Layer

The application layer contains the high-level business logic and coordinates the overall system behavior.

Key components:
- **Controller**: Central application logic that coordinates system behavior

## Communication Architecture

### ZBUS (Zephyr Bus)

The system uses ZBUS for inter-component communication, implementing a publish-subscribe pattern that decouples components and enhances modularity.

Key channels:
- **Button Event Channel**: Sends button press events from the Peripheral Module to the Controller
- **LED Task Channel**: Controls LED states (ON, OFF, BLINK)
- **BLE Command Channel**: Sends commands to the BLE service

### Thread Architecture

The firmware uses multiple threads with different priorities to ensure real-time performance:

1. **Main Thread**: System initialization and coordination
2. **BLE Thread**: Handles Bluetooth communication
3. **Peripheral Thread**: Manages hardware peripherals
4. **Audio Processing Thread**: Handles audio data processing

## Initialization Sequence

1. System boot and Zephyr RTOS initialization
2. Peripheral module initialization
3. Wait for service threads to initialize (600ms delay)
4. BLE service initialization
5. Application controller initialization

## Error Handling

The system implements a robust error handling strategy:
- Return code propagation for synchronous operations
- Logging at appropriate levels (ERR, WRN, INF, DBG)
- Visual feedback through LEDs for critical errors
- Timeouts to prevent deadlocks in thread communication

## Memory Management

- Static allocation is preferred where possible
- Thread stack sizes are carefully configured to balance resource usage and prevent stack overflows
- Heap usage is minimized to prevent fragmentation

## Build System

The project uses CMake with Zephyr's build system integration:
- Modular CMakeLists.txt files in each directory
- Clear dependency management
- Configurable build options through Kconfig

## Configuration

System configuration is managed through:
- Zephyr's Kconfig system for compile-time configuration
- Device Tree Overlays for hardware configuration
- Runtime configuration stored in non-volatile memory

## Future Extensibility

The architecture is designed to be extensible through:
- Clear API boundaries between components
- Message-based communication that allows new components to subscribe to existing channels
- Modular design that allows new services and modules to be added with minimal impact on existing code

## Version Management

The firmware follows semantic versioning (MAJOR.MINOR.PATCH) with:
- Automated version tracking
- Version information accessible at runtime
- Version-based feature detection