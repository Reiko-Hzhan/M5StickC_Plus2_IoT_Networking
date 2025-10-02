# Function Description of M5StickCPlus2 Tilt-Controlled Ball Collision Game Code

This code is a lightweight tilt-controlled ball collision game specifically developed for the **M5StickCPlus2 development board**. It relies on the board’s built-in IMU accelerometer for player interaction, while integrating randomly moving enemy balls and physical collision logic to form a complete "control-feedback-interaction" game loop.

## I. Overall Positioning

Based on the hardware features of the M5StickCPlus2 (240×135 resolution LCD screen, built-in 3-axis accelerometer), the core gameplay is as follows:

- Players control the red "Player Ball" by **tilting the development board**;
- The blue "Enemy Ball" moves randomly within the screen and bounces automatically when hitting boundaries;
- A physical bounce effect is triggered when the two balls collide, and the Player Ball rebounds in the opposite direction of the collision;
- The player’s student ID is displayed in real time on the screen as an identity marker.

## II. Core Elements and Attribute Configuration

The code predefines the attributes and control parameters of key game elements to ensure configurable and adjustable logic, as detailed in the table below:

| Element Type           | Key Attribute Details                                        |
| ---------------------- | ------------------------------------------------------------ |
| **Player Ball (Red)    | Initial Position: Center of the screen (X=120, Y=67)- Size: Radius = 8px- Color: Red (`RED`)- Initial Speed: 0 (stationary) |
| **Enemy Ball (Blue)**  | Initial Position: Random within the screen (avoids boundaries, no overlap with its own radius)- Size: Radius = 6px- Color: Blue (`BLUE`)- Speed: Random between 1–3 (50% chance of reverse direction) |
| **Control Parameters** | Sensitivity (1.2): Adjusts the impact of acceleration on ball speed; smaller values result in smoother control- Bounce Force (5.0): Force with which balls rebound upon collision- Speed Decay Coefficient (0.9): Simulates "friction" to gradually reduce ball speed (ball stops automatically when not tilted) |

## III. Core Functional Logic

### 1. Initialization Configuration (`setup` Function)

When the program starts, it completes hardware initialization and game state configuration (executed only once):

- **Hardware Initialization**: Call `M5.begin()` to initialize all components of the M5StickCPlus2 (screen, IMU sensor, I/O ports, etc.) and ensure hardware responds properly;
- Screen Configuration:
  - Set screen rotation (to match the board’s default holding posture);
  - Fill the background with black (`BLACK`) to enhance contrast between balls and the background;
  - Set text color to white (`WHITE`) for subsequent student ID display;
- **Enemy Ball Initialization**: Call the `initEnemy()` function to generate the initial position and speed of the Enemy Ball (ensuring it does not go beyond the screen).

### 2. Player Ball Control: Tilt Interaction (Core of `loop` Function)

Tilt control is implemented using the **IMU accelerometer** built into the M5StickCPlus2, following this core process:

1. **Acquire Accelerometer Data**: Read X/Y/Z-axis acceleration values via `M5.Imu.getAccelData(&ax, &ay, &az)`;

2. Speed Mapping: 

   Convert sensor data into ball speed:

   - Y-axis acceleration (`ay`) → X-direction speed of the Player Ball;
   - X-axis acceleration (`ax`) → Y-direction speed of the Player Ball;
   - Multiply by a coefficient of `0.2` and sensitivity of `1.2` to avoid overly sensitive control;

3. Physical Effect Simulation:

   - **Speed Decay**: Multiply the ball speed by `0.9` in each loop to simulate "ground friction"; the ball gradually stops when not tilted;

   - Boundary Restriction: 

     When the Player Ball hits the screen edge:

     - Reverse the speed and halve it (to achieve "soft bounce" and avoid excessive rebound force);
     - Force the ball’s position back inside the screen (using the `constrain()` function to prevent it from getting stuck on boundaries).

### 3. Enemy Ball Logic: Random Movement and Boundary Bounce

The movement of the Enemy Ball is controlled by two functions: `initEnemy()` (initialization) and `updateEnemy()` (real-time update):

- Initialization (`initEnemy()`):
  - Random Position: Use `random(enemyRadius, M5.Lcd.width() - enemyRadius)` to ensure the ball stays within the screen and does not touch boundaries;
  - Random Speed: Speed ranges from 1 to 3 (inclusive), with a 50% chance of reverse direction (achieved by negating the speed when `random(2) == 0`);
- Real-Time Update (`updateEnemy()`):
  - Position Update: Move the Enemy Ball at its current speed in each loop (`enemyX += enemySpeedX`, `enemyY += enemySpeedY`);
  - Boundary Bounce: Reverse the speed when hitting the screen edge, and force the ball’s position back inside the screen (to prevent it from "getting stuck outside the screen").

### 4. Collision Detection and Bounce Physics (`checkCollision()` + `handleCollision()`)

Collision logic that follows basic mechanical principles is implemented in two steps: "collision judgment" and "bounce handling":

- Collision Judgment (`checkCollision()`):
  1. Calculate the X/Y-axis distance difference between the centers of the two balls (`dx = ballX - enemyX`, `dy = ballY - enemyY`);
  2. Use the Pythagorean theorem to calculate the distance between the centers: `distance = sqrt(dx*dx + dy*dy)`;
  3. If the distance is less than the sum of the radii of the two balls (`ballRadius + enemyRadius`), a "collision" is determined, and `true` is returned;
- Bounce Handling (`handleCollision()`):
  1. Calculate Collision Direction: Normalize the direction vector by dividing `dx`/`dy` by the distance to ensure accurate direction;
  2. Apply Bounce Force: Assign a speed to the Player Ball in the opposite direction of the collision (using `ballSpeedX = dirX * bounceForce`) with a bounce force of `5.0` to achieve a "rebound" effect.

### 5. Screen Rendering and Information Display

The screen is refreshed at the end of each loop to ensure real-time update of the game 画面 (frame rate ≈ 100fps):

- **Clear Screen**: Call `M5.Lcd.fillScreen(BLACK)` to clear the previous frame and avoid residual images;
- Draw Balls:
  - Player Ball: Drawn using `M5.Lcd.fillCircle(ballX, ballY, ballRadius, ballColor)`;
  - Enemy Ball: Drawn using `M5.Lcd.fillCircle(enemyX, enemyY, enemyRadius, enemyColor)`;
- Display Student ID:
  - Set font size to 1 (`setTextSize(1)`);
  - Display Position: Top-left corner of the screen (X=10, Y=8);
- **Frame Rate Control**: `delay(10)` sets the loop interval to 10ms, preventing excessive hardware resource usage due to high frame rates.





# M5StickCPlus2 体感小球碰撞游戏代码功能说明

该代码是专为 **M5StickCPlus2 开发板** 编写的轻量化体感控制小球碰撞游戏，核心依托开发板内置的 IMU 加速度传感器实现玩家交互，同时集成随机运动的敌方小球与物理碰撞逻辑，形成完整的 “控制 - 反馈 - 交互” 游戏闭环。

## 一、整体定位

基于 M5StickCPlus2 硬件特性（240×135 分辨率 LCD 屏、内置 3 轴加速度计），实现以下核心玩法：

- 玩家通过 **倾斜开发板** 控制红色 “玩家球” 移动；
- 蓝色 “敌人球” 在屏幕内随机运动，碰到边界自动反弹；
- 两球碰撞时触发物理反弹效果，玩家球沿碰撞反方向弹开；
- 屏幕实时显示玩家学号，作为身份标识。

## 二、核心元素与属性配置

代码预先定义游戏关键元素的属性与控制参数，确保逻辑可配置、易调整，具体如下表：

| 元素类型         | 关键属性详情                                                 |
| ---------------- | :----------------------------------------------------------- |
| **玩家球（红）** | 初始位置：屏幕中心（X=120，Y=67）- 尺寸：半径 8px- 颜色：红色（`RED`）- 初始速度：0（静止状态） |
| **敌人球（蓝）** | 初始位置：屏幕内随机（避开边界，不超出自身半径）- 尺寸：半径 6px- 颜色：蓝色（`BLUE`）- 速度：1~3 随机（方向 50% 概率反向） |
| **控制参数**     | 灵敏度（1.2）：调节加速度对球速的影响，数值越小控制越平缓- 反弹力（5.0）：碰撞时小球的弹开力度- 速度衰减系数（0.9）：模拟 “摩擦”，使球速逐渐降低（不倾斜时自动静止） |

## 三、核心功能逻辑

### 1. 初始化配置（`setup` 函数）

程序启动时完成硬件初始化与游戏初始状态配置，仅执行一次：

- **硬件初始化**：调用 `M5.begin()` 初始化 M5StickCPlus2 全组件（屏幕、IMU 传感器、IO 口等），确保硬件正常响应；
- 屏幕配置：
  - 设置屏幕旋转方向（适配开发板默认握持姿势）；
  - 填充黑色背景（`BLACK`），提升小球与背景的对比度；
  - 文字颜色设为白色（`WHITE`），用于后续显示学号；
- **敌人球初始化**：调用 `initEnemy()` 函数，生成敌人球的初始位置与速度（避免初始位置超出屏幕）。

### 2. 玩家球控制：体感交互（`loop` 函数核心）

依托 M5StickCPlus2 内置的 **IMU 加速度传感器**，实现 “倾斜控制”，核心流程如下：

1. **获取加速度数据**：通过 `M5.Imu.getAccelData(&ax, &ay, &az)` 读取 X/Y/Z 轴加速度值；

2. 速度映射：

   将传感器数据转换为球速：

   - Y 轴加速度（`ay`）→ 玩家球 X 方向速度；
   - X 轴加速度（`ax`）→ 玩家球 Y 方向速度；
   - 乘以系数 `0.2` 和灵敏度 `1.2`，避免控制过于灵敏；

3. 物理效果模拟：

   - **速度衰减**：每次循环将球速乘以 `0.9`，模拟 “地面摩擦”，不倾斜时球会逐渐静止；

   - 边界限制：

     玩家球碰到屏幕边缘时：

     - 速度反向并减半（实现 “软反弹”，避免反弹过强）；
     - 强制将球位置拉回屏幕内（用 `constrain()` 函数，避免卡在边界）。

### 3. 敌人球逻辑：随机运动与边界反弹

敌人球的运动由 `initEnemy()`（初始化）和 `updateEnemy()`（实时更新）两个函数协同控制：

- 初始化（`initEnemy()`）：
  - 随机位置：通过 `random(enemyRadius, M5.Lcd.width() - enemyRadius)` 确保球在屏幕内，不触碰边界；
  - 随机速度：速度大小为 1~3（含边界值），方向通过 “`random(2) == 0` 时取负” 实现 50% 反向概率；
- 实时更新（`updateEnemy()`）：
  - 位置更新：每次循环按当前速度移动敌人球（`enemyX += enemySpeedX`，`enemyY += enemySpeedY`）；
  - 边界反弹：碰到屏幕边缘时速度反向，并强制将球位置拉回屏幕内（避免 “卡出屏幕”）。

### 4. 碰撞检测与反弹物理（`checkCollision()` + `handleCollision()`）

实现符合基础力学规律的碰撞逻辑，分 “判断碰撞” 和 “处理反弹” 两步：

- 碰撞判断（`checkCollision()`）：
  1. 计算两球中心的 X/Y 轴距离差（`dx = ballX - enemyX`，`dy = ballY - enemyY`）；
  2. 用勾股定理计算两球中心距离：`distance = sqrt(dx*dx + dy*dy)`；
  3. 若距离 < 两球半径之和（`ballRadius + enemyRadius`），判定为 “碰撞”，返回 `true`；
- 反弹处理（`handleCollision()`）：
  1. 计算碰撞方向：将 `dx`/`dy` 除以距离（标准化方向向量），确保方向准确；
  2. 应用反弹力：按 `bounceForce=5.0` 的力度，给玩家球赋予沿碰撞反方向的速度（`ballSpeedX = dirX * bounceForce`），实现 “弹开” 效果。

### 5. 屏幕渲染与信息显示

每次循环末尾刷新屏幕，确保画面实时更新（帧率约 100fps）：

- **清屏**：调用 `M5.Lcd.fillScreen(BLACK)` 清除上一帧画面，避免残影叠加；
- 绘制小球：
  - 玩家球：用 `M5.Lcd.fillCircle(ballX, ballY, ballRadius, ballColor)` 绘制；
  - 敌人球：用 `M5.Lcd.fillCircle(enemyX, enemyY, enemyRadius, enemyColor)` 绘制；
- 显示学号：
  - 字体大小设为 1（`setTextSize(1)`）；
  - 显示位置：屏幕左上角（X=10，Y=8）；
- **帧率控制**：`delay(10)` 使循环间隔 10ms，避免帧率过高导致硬件资源占用过多。