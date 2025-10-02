# M5StickCPlus2 Button Counting Program: Function Description

This code is a dedicated button counting program developed for the **M5StickCPlus2 development board**, focusing on simple human-computer interaction and real-time information display. Its core functions are detailed below:

## 1. Initialization Configuration

When the program starts, it completes the hardware initialization and display parameter setting to ensure the development board works normally:

- **Hardware Initialization**: Initializes core components of the M5StickCPlus2, including the display screen, physical buttons (BtnA/BtnB), and built-in speaker.
- Display Parameter Setting:
  - Rotates the screen by 90 degrees to match the default usage orientation of the development board.
  - Sets the text color to **green** for clear visibility.
  - Configures text alignment (top-left alignment)、font (`Orbitron_Light_24`) and text size (no scaling, size = 1).

## 2. Core Display Content

The screen fixedly shows two types of information to provide clear visual feedback:

- **Student ID Display**: Located at the top of the screen, it shows the preset student ID (defined by the `studentID` variable, with a fixed value of `"111111111"`).
- **Button A Count Display**: Located in the middle of the screen, it shows the number of times Button A has been pressed. The initial count value (`countA`) is 0.

## 3. Button Interaction Logic

The program supports independent interaction with BtnA and BtnB, with different feedback mechanisms for each button:

- When BtnA is Pressed:
  1. The built-in speaker emits an 8000Hz prompt tone (lasting 20 milliseconds) to indicate a valid press.
  2. The Button A count variable (`countA`) is incremented by 1.
  3. The screen updates in real time to display the latest count value.
- When BtnB is Pressed:
  1. The built-in speaker emits a 6000Hz prompt tone (different from BtnA’s tone) to distinguish operations.
  2. The Button A count variable (`countA`) is reset to 0 (clearing historical counts).
  3. The screen synchronously refreshes to show the reset count (0).

## 4. Real-Time Refresh Mechanism

After each button operation (either BtnA press or BtnB press), the program actively calls the `updateDisplay()` function:

- First, it clears the current screen content to avoid overlapping display of old and new information.
- Then, it redraws the latest student ID and Button A count on the screen.
- This ensures the displayed content is always consistent with the actual state of the program.





# M5StickCPlus2 按键计数程序：功能说明

该代码是专为**M5StickCPlus2 开发板**设计的按键计数程序，专注于实现简单的人机交互与实时信息展示。其核心功能如下：

## 1. 初始化配置

程序启动时，会完成硬件初始化与显示参数设置，确保开发板正常工作：

- **硬件初始化**：初始化 M5StickCPlus2 的核心组件，包括显示屏、物理按键（BtnA/BtnB）和内置扬声器。
- 显示参数设置：
  - 将屏幕旋转 90 度，适配开发板默认使用方向；
  - 文字颜色设为**绿色**，保证显示清晰；
  - 配置文字对齐方式（左上角对齐）、字体（`Orbitron_Light_24`）和文字大小（无缩放，size=1）。

## 2. 核心显示内容

屏幕固定展示两类信息，提供直观的视觉反馈：

- **学号显示**：位于屏幕顶部，展示预设的学号（由`studentID`变量定义，固定值为`"111111111"`）；
- **A 键计数显示**：位于屏幕中部，展示 A 键被按下的次数，初始计数（`countA`）为 0。

## 3. 按键交互逻辑

程序支持 A、B 两键的独立交互，每个按键有不同的反馈机制：

- 按下 A 键时：
  1. 内置扬声器发出 8000Hz 提示音（持续 20 毫秒），提示操作有效；
  2. A 键计数变量（`countA`）加 1；
  3. 屏幕实时更新，显示最新计数。
- 按下 B 键时：
  1. 内置扬声器发出 6000Hz 提示音（与 A 键音调不同，用于区分操作）；
  2. A 键计数变量（`countA`）重置为 0（清除历史计数）；
  3. 屏幕同步刷新，显示重置后的计数（0）。

## 4. 实时刷新机制

每次按键操作（无论是 A 键按下还是 B 键按下）后，程序会主动调用`updateDisplay()`函数：

- 首先清除当前屏幕内容，避免新旧信息重叠显示；
- 然后重新绘制最新的学号和 A 键计数；
- 确保屏幕显示内容与程序实际状态始终一致。