# F28P650DK9 控制器项目

TI C2000 F28P65x 系列 DSP 控制器项目，支持 Plexim 嵌入式代码生成。

## 项目结构

```
.
├── app/                    # 应用层配置
│   ├── f28p65x_flash_lnk_cpu1.cmd    # Flash 链接命令文件
│   ├── f28p65x_ram_lnk_cpu1.cmd      # RAM 链接命令文件
│   └── f28p65x_headers_nonBIOS_cpu1.cmd
├── cg/                     # 代码生成 (Controller)
│   ├── Controller.c
│   ├── Controller.h
│   ├── Controller_main.c
│   ├── Controller_hal.c
│   └── Controller_cla.cla
├── drv/                    # 驱动层
│   ├── tiinc/             # TI 头文件
│   └── tisrc/             # TI 源文件和库
├── hal/                    # 硬件抽象层
├── mid/pil/               # PIL (Processor in the Loop) 中间件
│   ├── inc/               # 公共头文件
│   ├── inc_impl/          # 实现头文件
│   ├── shrd/              # 共享代码
│   ├── src/               # 设备特定实现
│   └── pil_c28.a          # PIL 库
├── sim/                    # 仿真文件
├── user/                   # CCS 项目配置
│   ├── .ccsproject
│   ├── .cproject
│   ├── .project
│   └── xds110.ccxml       # 调试配置
└── xmake.lua              # xmake 构建脚本
```

## 构建方式

### 方式一：使用 xmake（推荐）

```bash
# 清理并构建
xmake clean
xmake

```

构建输出：
- `user/build/dev_28p650dk9_v1_10.out` - 可执行文件
- `user/build/dev_28p650dk9_v1_10.hex` - Intel HEX 格式
- `user/build/dev_28p650dk9_v1_10.bin` - 二进制格式
- `user/build/dev_28p650dk9_v1_10.map` - 链接映射文件

### 方式二：使用 CCS (Code Composer Studio)

1. 打开 CCS
2. 导入项目：`File` → `Import` → `CCS Projects`
3. 选择 `user` 目录
4. 构建项目

## 芯片规格

- **型号**: TMS320F28P650DK9
- **内核**: C28x 32位 DSP
- **主频**: 200 MHz
- **FLASH**: 1.28M
- **RAM**: 248 KB 
- **CLA**: CLA2 (控制律加速器)
- **FPU**: FPU32 + TMU1

## 内存使用

构建完成后自动显示：

```
FLASH: 0x04ABB / 0x5FFFE bytes (4.9% used)
       18 / 383 KB
RAM:   0x01024 / 0x0E54F bytes (7.0% used)
       4 / 57 KB
```

## 预定义宏

- `_PLEXIM_` - Plexim 代码生成标识
- `CPU1` - CPU1 内核
- `EXTERNAL_MODE=1` - 外部模式支持
- `_FLASH` - Flash 构建配置

## 工具链

- **CCS**: 12.8.0 或更高版本
- **编译器**: TI CGT C2000 v22.6.2.LTS
- **xmake**: v3.0.0+

## 许可证

Copyright (c) 2023 by Plexim GmbH. All rights reserved.

## 注意事项

1. **CLA 文件**: `Controller_cla.cla` 使用特殊编译规则，通过 xmake 的 cla 规则处理。

2. **库文件**: 项目使用以下库：
   - `pil_c28.a` - PIL 运行时库
   - `plx_driverlib_fpu32_eabi.lib` - 驱动库 (EABI/FPU32)
   - `plx_dcl_fpu32_eabi.lib` - DCL 库 (EABI/FPU32)


