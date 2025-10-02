# Function Description of M5StickCPlus2 BLE Peripheral Application

This code is a **Bluetooth Low Energy (BLE) peripheral application** specifically developed for the M5StickCPlus2 development board. Its core function is to configure the board as a discoverable BLE peripheral device, enabling it to establish connections with BLE central devices (such as smartphones and tablets) and provide real-time connection status feedback via the board’s screen.

## I. Overall Positioning

The M5StickCPlus2 development board is transformed into a **BLE Peripheral device**. Through custom BLE services and characteristics, it enables the following core capabilities:

- Allows BLE central devices (e.g., smartphone apps) to discover and connect to the board;
- Supports bidirectional data interaction (read/write/active push) between central and peripheral devices;
- Intuitively displays device status (e.g., advertising, connected, disconnected) on the screen, enhancing user visibility.

## II. Core Functional Modules

### 1. Basic BLE Device Configuration

Defines the BLE device’s identity and core parameters to ensure the central device can identify and connect to it:

- **Device Name**: The device is named `Reiko-Hzhan`, which serves as an identity tag during BLE advertising, allowing central devices to quickly locate the target device;
- **UUID Definition**: UUIDs (Universally Unique Identifiers) act as "identification cards" for BLE services and characteristics. Two key UUIDs are defined in the code:

| UUID Type                  | Specific Value                         | Description                                                  |
| -------------------------- | -------------------------------------- | ------------------------------------------------------------ |
| Custom Service UUID        | `4fafc201-1fb5-459e-8fcc-c5c9c331914b` | Identifies the core BLE service provided by the board. Central devices must use this UUID to find the corresponding service for interaction. |
| Custom Characteristic UUID | `beb5483e-36e1-4688-b7f5-ea07361b26a8` | A data interaction node under the service, supporting read, write, notify, and indicate operations between central and peripheral devices. |

### 2. BLE Service & Communication Capabilities

Builds the core logical units for BLE data interaction and defines communication rules between central and peripheral devices:

- **Service Creation**: A custom BLE service is created via `pServer->createService(SERVICE_UUID)`. A service acts as a "logical container" for BLE functions, grouping related interaction characteristics;

- Characteristic Configuration: 

  A characteristic is created under the service, supporting 4 core communication operations (covering bidirectional interaction needs):

  - `PROPERTY_READ`: Allows the central device to read data stored in this characteristic (e.g., sensor data from the development board);
  - `PROPERTY_WRITE`: Allows the central device to write data to this characteristic (e.g., commands to control the board’s LEDs or motors);
  - `PROPERTY_NOTIFY`: Enables the peripheral device (M5StickCPlus2) to actively push data to the central device without a request from the central device (e.g., real-time sensor data push);
  - `PROPERTY_INDICATE`: Similar to `NOTIFY`, but requires confirmation from the central device. Suitable for pushing critical data (e.g., alarm information);

- **Descriptor Addition**: The `BLE2902` descriptor (a BLE standard descriptor) is added to provide protocol support for the `NOTIFY`/`INDICATE` functions of the characteristic. This is a mandatory configuration required by BLE standards.

### 3. Connection Status Management

A callback class `MyServerCallbacks` is used to monitor BLE connection status in real time and trigger corresponding actions, ensuring connection status is visible and recoverable:

- Connection Event (`onConnect`):
  1. When a central device (e.g., a smartphone) successfully connects, the connection status flag `deviceConnected` is set to `true`;
  2. `Device connected` is printed on the board’s screen to inform the user of a successful connection;
- Disconnection Event (`onDisconnect`):
  1. When the central device disconnects, `deviceConnected` is set to `false`;
  2. `Device disconnected` is printed on the screen to alert the user of the connection interruption;
  3. BLE advertising is automatically restarted via `pServer->getAdvertising()->start()`, ensuring the device can be rediscovered by other central devices without manual program restart.

### 4. Screen Feedback & Status Display

The M5StickCPlus2’s LCD screen displays device status in phases to enhance user experience:

- Initialization Phase:
  - Screen Configuration: Black background (improves text contrast), white text (for clarity), and font size 2 (adapted to the screen size);
  - Display Content: `BLE Broadcasting...` to inform the user that the device is waiting for a central device connection;
- Device Information Display:
  - After advertising starts, `Device initialized:` and the device name `Reiko-Hzhan` are printed on the screen. This clarifies the device’s identity, helping the user confirm it is the target device;
- Dynamic Status Updates:
  - When connecting or disconnecting, the corresponding status message (`Device connected`/`Device disconnected`) is printed on the screen in real time. This allows the user to intuitively track the BLE connection status without additional debugging tools.

### 5. Device Activity Maintenance

A delay logic in the `loop` function maintains device activity, ensuring BLE functions remain available:

- The code uses `delay(30000)` to set a 30-second delay per loop, preventing the program from entering a sleep state due to high-frequency looping;
- This ensures continuous BLE advertising and uninterrupted connection status monitoring, allowing central devices to discover and connect at any time.

## III. Operational Process Summary

1. Initialization Phase:
   - Initializes the M5StickCPlus2 hardware (screen, I/O ports, sensors, etc.);
   - Configures core BLE parameters such as device name (`Reiko-Hzhan`), service UUID, and characteristic UUID;
2. Service Startup Phase:
   - Creates a BLE server instance and binds the connection callback class (`MyServerCallbacks`);
   - Creates a BLE service and characteristic using custom UUIDs, adds the `BLE2902` descriptor, and starts the service;
3. Advertising Startup Phase:
   - Configures BLE advertising (adds service UUID, enables scan response);
   - Starts advertising to broadcast device information to nearby devices and waits for central device discovery;
4. Connection Interaction Phase:
   - Central Device Connection: Triggers the `onConnect` callback, updates the connection status, and displays it on the screen;
   - Central Device Disconnection: Triggers the `onDisconnect` callback, updates the status, displays a notification, and restarts advertising;
5. Continuous Operation Phase:
   - Loops to monitor changes in connection status and maintains device activity via delays;
   - Waits for reconnection from the central device or for data interaction.





# M5StickCPlus2 BLE 外设应用程序功能说明

该代码是专为 **M5StickCPlus2 开发板** 编写的蓝牙低功耗（Bluetooth Low Energy, BLE）外设应用程序，核心功能是将开发板配置为可被发现的 BLE 从设备，支持与手机、平板等 BLE 主设备建立连接，并通过开发板屏幕实时反馈连接状态，为后续数据交互奠定基础。

## 一、整体定位

将 M5StickCPlus2 开发板改造为 **BLE 从设备（Peripheral）**，通过自定义 BLE 服务与特征，实现以下核心能力：

- 允许 BLE 主设备（如手机 APP）发现、连接设备；
- 支持主从设备间的双向数据交互（读 / 写 / 主动推送）；
- 通过屏幕直观展示设备状态（广播中、已连接、已断开等），提升用户可感知性。

## 二、核心功能模块

### 1. BLE 设备基础配置

完成 BLE 设备的身份标识与核心参数定义，确保主设备可识别并建立连接：

- **设备名称**：定义设备名为 `Reiko-Hzhan`，作为 BLE 广播时的身份标签，方便主设备快速定位目标设备；
- **UUID 定义**：UUID（通用唯一识别码）是 BLE 服务与特征的 “身份证”，代码中定义两组关键 UUID：

| UUID 类型       | 具体值                                 | 作用说明                                                     |
| --------------- | -------------------------------------- | ------------------------------------------------------------ |
| 自定义服务 UUID | `4fafc201-1fb5-459e-8fcc-c5c9c331914b` | 标识开发板提供的核心 BLE 服务，主设备需通过此 UUID 找到对应服务进行交互 |
| 自定义特征 UUID | `beb5483e-36e1-4688-b7f5-ea07361b26a8` | 服务下的数据交互节点，支持主从设备间的读、写、通知、指示等操作 |

### 2. BLE 服务与通信能力

构建 BLE 数据交互的核心逻辑单元，定义主从设备的通信规则：

- **服务创建**：通过 `pServer->createService(SERVICE_UUID)` 创建自定义 BLE 服务，服务是 BLE 功能的 “逻辑容器”，用于归类相关的交互特征；

- 特征配置：

  在服务下创建特征，支持 4 种核心通信操作（覆盖主从设备双向交互需求）：

  - `PROPERTY_READ`：允许主设备读取该特征中存储的数据（如开发板传感器数据）；
  - `PROPERTY_WRITE`：允许主设备向该特征写入数据（如控制开发板 LED、电机的指令）；
  - `PROPERTY_NOTIFY`：支持从设备（M5StickCPlus2）主动向主设备推送数据，无需主设备请求（如实时推送传感器数据）；
  - `PROPERTY_INDICATE`：与 `NOTIFY` 类似，但需要主设备回复确认，适用于关键数据推送（如告警信息）；

- **描述符添加**：通过 `BLE2902` 描述符（BLE 标准描述符），为特征的 `NOTIFY`/`INDICATE` 功能提供协议支持，是 BLE 标准要求的必要配置。

### 3. 连接状态管理

通过 `MyServerCallbacks` 回调类实时监控 BLE 连接状态，并触发对应响应动作，确保连接状态可感知、可恢复：

- 连接事件（`onConnect`）：
  1. 当主设备（如手机）成功连接时，将连接状态标记 `deviceConnected` 设为 `true`；
  2. 在开发板屏幕上打印 `Device connected`（设备已连接），告知用户连接成功；
- 断开事件（`onDisconnect`）：
  1. 当主设备断开连接时，将 `deviceConnected` 设为 `false`；
  2. 屏幕打印 `Device disconnected`（设备已断开），提示用户连接中断；
  3. 自动调用 `pServer->getAdvertising()->start()` 重启 BLE 广播，确保设备可被其他主设备再次发现，无需手动重启程序。

### 4. 屏幕反馈与状态展示

通过 M5StickCPlus2 的 LCD 屏，分阶段展示设备状态，提升用户操作体验：

- 初始化阶段：
  - 屏幕配置：黑色背景（提升文字对比度）、白色文字（清晰可见）、字号 2（适配屏幕尺寸）；
  - 显示内容：`BLE Broadcasting...`（BLE 广播中），告知用户设备正在等待主设备连接；
- 设备信息展示：
  - 广播启动后，屏幕打印 `Device initialized:`（设备已初始化）及设备名称 `Reiko-Hzhan`，明确当前设备身份，方便用户确认是否为目标设备；
- 动态状态更新：
  - 连接 / 断开时，屏幕实时打印对应状态信息（`Device connected`/`Device disconnected`），让用户直观了解 BLE 连接情况，无需额外工具调试。

### 5. 设备活跃维护

在 `loop` 函数中通过延迟逻辑维持设备活跃性，确保 BLE 功能持续可用：

- 代码中通过 `delay(30000)` 设定每 30 秒延迟一次，避免程序因高频循环进入休眠状态；
- 保障 BLE 广播持续发送、连接状态监控不中断，确保主设备随时可发现并连接。

## 三、运行流程总结

1. 初始化阶段：
   - 启动 M5StickCPlus2 硬件（屏幕、IO 口、传感器等）；
   - 配置 BLE 设备名称（`Reiko-Hzhan`）、服务 UUID、特征 UUID 等核心参数；
2. 启动服务阶段：
   - 创建 BLE 服务器实例，绑定连接回调类（`MyServerCallbacks`）；
   - 基于自定义 UUID 创建 BLE 服务与特征，添加 `BLE2902` 描述符，启动服务；
3. 开始广播阶段：
   - 配置 BLE 广播（添加服务 UUID、启用扫描响应）；
   - 启动广播，向周围发送设备信息，等待主设备发现；
4. 连接交互阶段：
   - 主设备连接：触发 `onConnect` 回调，更新连接状态并在屏幕显示；
   - 主设备断开：触发 `onDisconnect` 回调，更新状态、显示提示并重启广播；
5. 持续运行阶段：
   - 循环监控连接状态变化，通过延迟维持设备活跃；
   - 等待主设备再次连接或进行数据交互。