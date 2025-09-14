# STM32 智能家居系统

<div align="center">

![STM32](https://img.shields.io/badge/STM32-F103C6-blue.svg)
![C](https://img.shields.io/badge/Language-C-green.svg)
![ESP8266](https://img.shields.io/badge/WiFi-ESP8266-orange.svg)
![OneNet](https://img.shields.io/badge/Cloud-OneNet-red.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

一个基于STM32F103C6的智能家居控制系统，支持温湿度监测、LED控制、按键交互、OLED显示和云端数据上传

</div>

## 📋 项目概述

本项目是一个完整的STM32智能家居解决方案，集成了多种传感器和执行器，通过ESP8266 WiFi模块实现与OneNet云平台的连接，支持远程监控和控制。系统采用模块化设计，便于扩展和维护。

### ✨ 主要特性

- 🌡️ **环境监测**: DHT11温湿度传感器实时监测环境参数
- 💡 **智能照明**: LED灯控制，支持本地和远程开关
- 🔘 **人机交互**: 按键输入，支持本地控制
- 📺 **信息显示**: OLED屏幕实时显示系统状态
- 🌐 **云端连接**: ESP8266 WiFi模块连接OneNet云平台
- 📡 **MQTT通信**: 基于MQTT协议的数据传输
- 🔄 **实时同步**: 云端数据与本地设备实时同步

## 🏗️ 系统架构
### 软件架构

```
┌─────────────────────────────────────────────────────────────┐
│                    应用层 (Application Layer)                │
├─────────────────────────────────────────────────────────────┤
│  main.c - 主程序逻辑                                        │
│  - 温湿度数据采集与显示                                      │
│  - LED状态控制                                             │
│  - 按键事件处理                                            │
│  - 云端数据同步                                            │
├─────────────────────────────────────────────────────────────┤
│                    网络层 (Network Layer)                   │
├─────────────────────────────────────────────────────────────┤
│  OneNet API - 云端数据上传与命令接收                        │
│  MQTT Kit - MQTT协议实现                                   │
│  ESP8266 Driver - WiFi模块驱动                             │
├─────────────────────────────────────────────────────────────┤
│                    硬件抽象层 (HAL)                         │
├─────────────────────────────────────────────────────────────┤
│  DHT11 Driver - 温湿度传感器驱动                            │
│  OLED Driver - 显示屏驱动                                  │
│  LED Driver - LED控制驱动                                  │
│  KEY Driver - 按键驱动                                     │
│  USART Driver - 串口通信驱动                               │
├─────────────────────────────────────────────────────────────┤
│                    硬件层 (Hardware Layer)                  │
├─────────────────────────────────────────────────────────────┤
│  STM32F103C6 微控制器                                      │
│  ESP8266 WiFi模块                                          │
│  DHT11 温湿度传感器                                         │
│  OLED 显示屏                                               │
│  LED 指示灯                                                │
│  按键开关                                                  │
└─────────────────────────────────────────────────────────────┘
```

## 🔧 硬件要求

### 核心组件

| 组件 | 型号 | 数量 | 功能描述 |
|------|------|------|----------|
| 微控制器 | STM32F103C6 | 1 | 主控芯片，Cortex-M3内核 |
| WiFi模块 | ESP8266 | 1 | 网络连接，MQTT通信 |
| 温湿度传感器 | DHT11 | 1 | 环境参数监测 |
| 显示屏 | OLED 128x64 | 1 | 信息显示 |
| LED指示灯 | 通用LED | 1 | 状态指示 |
| 按键 | 轻触开关 | 1 | 用户输入 |

### 连接说明

#### STM32F103C6 引脚分配

| 功能 | 引脚 | 说明 |
|------|------|------|
| DHT11数据线 | PA0 | 温湿度传感器数据接口 |
| OLED SCL | PB8 | I2C时钟线 |
| OLED SDA | PB9 | I2C数据线 |
| LED控制 | PC13 | LED指示灯控制 |
| 按键输入 | PA1 | 按键检测 |
| ESP8266 TX | PA2 | 串口2发送 |
| ESP8266 RX | PA3 | 串口2接收 |
| 调试串口 TX | PA9 | 串口1发送 |
| 调试串口 RX | PA10 | 串口1接收 |

#### ESP8266 连接

| ESP8266引脚 | STM32引脚 | 说明 |
|-------------|-----------|------|
| VCC | 3.3V | 电源正极 |
| GND | GND | 电源负极 |
| TX | PA3 | 数据发送 |
| RX | PA2 | 数据接收 |
| EN | 3.3V | 使能引脚 |

## 🚀 快速开始

### 环境准备

1. **开发环境**
   - Clion
   - STM32CubeMX (可选，用于配置)
   - ST-Link 调试器

2. **编译工具链**
   - ARM GCC 工具链
   - CMake 3.27+

### 编译步骤

1. **克隆项目**
   ```bash
   git clone https://github.com/your-username/smart-home-base-on-STM32.git
   cd smart-home-base-on-STM32
   ```

2. **选择目标模块**
   ```bash
   # 进入目标模块目录
   cd OLED  # 选择OLED模块（包含完整功能）
   ```

3. **编译项目**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **烧录程序**
   ```bash
   # 使用ST-Link烧录
   st-flash write biaozhunceshi.bin 0x8000000
   ```

### 配置说明

#### WiFi配置

在 `NET/device/src/esp8266.c` 中配置WiFi参数：

```c
// WiFi连接信息
#define WIFI_SSID "your_wifi_name"
#define WIFI_PASSWORD "your_wifi_password"
```

#### OneNet配置

在 `NET/onenet/src/onenet.c` 中配置OneNet参数：

```c
// OneNet设备信息
#define DEVICE_ID "your_device_id"
#define API_KEY "your_api_key"
#define PRODUCT_ID "your_product_id"
```

## 📖 使用说明

### 基本操作

1. **上电启动**
   - 系统自动初始化所有硬件模块
   - OLED屏幕显示欢迎界面
   - ESP8266自动连接WiFi

2. **本地控制**
   - 按下按键可切换LED状态
   - OLED屏幕实时显示温湿度数据
   - 串口输出调试信息

3. **远程监控**
   - 通过OneNet平台查看实时数据
   - 远程控制LED开关
   - 查看历史数据记录

### 功能模块

#### 温湿度监测
- **传感器**: DHT11
- **测量范围**: 温度 0-50°C，湿度 20-95%RH
- **更新频率**: 5秒/次
- **显示方式**: OLED屏幕 + 云端上传

#### LED控制
- **控制方式**: 本地按键 + 远程命令
- **状态指示**: 实时显示在OLED屏幕
- **云端同步**: 状态变化自动上传

#### 按键交互
- **功能**: LED开关切换
- **防抖处理**: 软件防抖，确保稳定
- **响应时间**: < 50ms

#### 网络通信
- **协议**: MQTT over TCP
- **服务器**: OneNet MQTT服务器
- **心跳间隔**: 60秒
- **重连机制**: 自动重连

## 🐛 故障排除

### 常见问题

#### 1. 编译错误
- **问题**: 找不到头文件
- **解决**: 检查CMakeLists.txt中的include_directories配置

#### 2. 烧录失败
- **问题**: ST-Link连接失败
- **解决**: 检查调试器连接和驱动安装

#### 3. WiFi连接失败
- **问题**: ESP8266无法连接WiFi
- **解决**: 检查WiFi配置和信号强度

#### 4. 传感器读取异常
- **问题**: DHT11读取失败
- **解决**: 检查接线和供电电压

#### 5. 云端连接失败
- **问题**: OneNet连接超时
- **解决**: 检查网络连接和设备配置

### 调试方法

1. **串口调试**
   ```c
   // 启用调试输出
   UsartPrintf(USART_DEBUG, "Debug message: %d\r\n", value);
   ```

2. **LED指示**
   ```c
   // 使用LED指示状态
   Led_Set(LED_ON);   // 表示正常
   Led_Set(LED_OFF);  // 表示异常
   ```

3. **OLED显示**
   ```c
   // 显示调试信息
   OLED_ShowString(0, 0, "Debug Info", 16);
   ```

</div>
