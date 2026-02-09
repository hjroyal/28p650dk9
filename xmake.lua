-- TI C2000 F28P65x Project Build Configuration
-- Generated based on CCS project configuration

-- 配置TI SDK路径（可修改）
local TI_CCS_ROOT = "C:/ti/ccs2031/ccs"
local TI_COMPILER_ROOT = TI_CCS_ROOT .. "/tools/compiler/ti-cgt-c2000_22.6.2.LTS"
local TI_OBJ2BIN_ROOT = TI_CCS_ROOT .. "/utils/tiobj2bin"

-- 项目配置
local PROJECT_NAME = "dev_28p650dk9_v1_10"
local TARGET_NAME = PROJECT_NAME

-- 定义工具链
toolchain("c2000")
    set_kind("cross")
    set_toolset("cc", TI_COMPILER_ROOT .. "/bin/cl2000")
    set_toolset("cxx", TI_COMPILER_ROOT .. "/bin/cl2000")
    set_toolset("as", TI_COMPILER_ROOT .. "/bin/cl2000")
    -- 不使用默认的ld，我们将使用自定义链接规则
    set_toolset("ld", "echo")
    set_toolset("ar", TI_COMPILER_ROOT .. "/bin/ar2000")
    
    -- 设置工具链描述
    set_description("TI C2000 Code Generation Tools")
    
    -- 设置编译选项检查策略（禁用自动检查）
    on_check(function (toolchain)
        return true
    end)

toolchain_end()

-- 设置使用自定义工具链
set_toolchains("c2000")

-- 定义目标
target(TARGET_NAME)
    set_kind("binary")
    set_filename(TARGET_NAME .. ".out")
    
    -- 目标平台设置
    set_arch("c2000")
    
    -- 设置策略：禁用自动忽略未知flags
    set_policy("check.auto_ignore_flags", false)
    
    -- 添加源文件
    -- cg文件夹（main函数所在位置）
    add_files("cg/Controller_cla.cla")
    add_files("cg/Controller.c")
    add_files("cg/Controller_main.c")
    add_files("cg/Controller_hal.c")
    
    -- CLA文件需要特殊处理，使用规则
    rule("cla")
         set_extensions(".cla")
         on_build_file(function (target, sourcefile, opt)
              -- CLA文件编译输出到固定位置，确保链接器能找到
              local objectfile = target:objectfile(sourcefile)
              local objdir = path.directory(objectfile)
              local compiler = TI_COMPILER_ROOT .. "/bin/cl2000"
              
              -- 确保对象文件目录存在
              os.mkdir(objdir)
              
              -- 构建编译命令
              local args = {
                  "-v28", "-ml", "-mt",
                  "--cla_support=cla2",
                  "--float_support=fpu32",
                  "--tmu_support=tmu1",
                  "-O0",
                  "--opt_for_speed=5",
                  "--fp_mode=relaxed",
                  "-g",
                  "--symdebug:dwarf_version=3",
                  "--c11",
                  "--float_operations_allowed=all",
                  "--diag_warning=225",
                  "--display_error_number",
                  "--abi=eabi",
                  "--preproc_with_compile",
                  "-D_PLEXIM_",
                  "-DCPU1",
                  "-DEXTERNAL_MODE=1",
                  "-D_FLASH",
                  "-I.",
                  "-Iapp",
                  "-Itiinc",
                  "-Iinc",
                  "-Iinc_impl",
                  "-Ipil",
                  "-Ishrd",
                  "-Itiinc/driverlib",
                  "-Icg",
                  "-I" .. TI_COMPILER_ROOT .. "/include",
                  "-Idrv",
                  "-Idrv/tiinc",
                  "-Idrv/tiinc/driverlib",
                  "-Ihal",
                  "-Imid/pil",
                  "-Imid/pil/inc",
                  "-Imid/pil/inc_impl",
                  "-Imid/pil/shrd",
                  "-Imid/pil/src",
                  "-Isim",
                  "-fr", objdir,
                  "-fs", objdir,
                  "-o", objectfile,
                  sourcefile
              }
              
              os.execv(compiler, args)
              
              -- 返回对象文件路径，让xmake知道生成了这个文件
              return objectfile
          end)
    rule_end()
    
    add_files("cg/Controller_cla.cla", {rule = "cla"})

    -- drv文件夹（根据.ccsproject排除列表，只包含需要的文件）
    add_files("drv/tisrc/f28p65x_adc.c")
    add_files("drv/tisrc/f28p65x_codestartbranch.asm")
    add_files("drv/tisrc/f28p65x_devinit.c")
    add_files("drv/tisrc/f28p65x_globalvariabledefs.c")
    add_files("drv/tisrc/f28p65x_usdelay.asm")    

    -- mid文件夹（根据.ccsproject排除列表）
    add_files("mid/pil/shrd/ain_type4.c")
    add_files("mid/pil/shrd/csv.c")
    add_files("mid/pil/shrd/dac_type1.c")
    add_files("mid/pil/shrd/dcan_type0.c")
    add_files("mid/pil/shrd/dispatcher.c")
    add_files("mid/pil/shrd/dispatcher_asm.asm")
    add_files("mid/pil/shrd/ecap_type3.c")
    add_files("mid/pil/shrd/epwm_type4.c")
    add_files("mid/pil/shrd/mcan_type0.c")
    add_files("mid/pil/shrd/plx_ipc.c")
    add_files("mid/pil/shrd/power.c")
    add_files("mid/pil/shrd/qep.c")
    add_files("mid/pil/shrd/sdfm.c")
    add_files("mid/pil/shrd/spi_type2.c")
    add_files("mid/pil/shrd/stack_monitor.c")
    
    -- pil/src文件夹（设备驱动实现）
    add_files("mid/pil/src/dio_28p65x.c")
    add_files("mid/pil/src/sci_28p65x.c")

    -- 添加头文件搜索路径
    add_includedirs(TI_COMPILER_ROOT .. "/include")
    add_includedirs("app")
    add_includedirs("cg")
    add_includedirs("drv")
    add_includedirs("drv/tiinc")
    add_includedirs("drv/tiinc/driverlib")
    add_includedirs("hal")
    add_includedirs("mid/pil")
    add_includedirs("mid/pil/inc")
    add_includedirs("mid/pil/inc_impl")
    add_includedirs("mid/pil/shrd")
    add_includedirs("mid/pil/src")
    add_includedirs("sim")

    
    -- 添加库文件搜索路径
    add_linkdirs(TI_COMPILER_ROOT .. "/lib")
    add_linkdirs(TI_COMPILER_ROOT .. "/include")
    
    -- 添加库文件（链接命令文件将通过自定义链接规则添加）
    add_files("drv/tisrc/dcl/plx_dcl_fpu32_eabi.lib")
    add_files("drv/tisrc/driverlib/plx_driverlib_fpu32_eabi.lib")
    add_files("mid/pil/pil_c28.a")
    add_files("drv/tisrc/driverlib/plx_driverlib.a")
    add_files("drv/tisrc/dcl/plx_dcl.a")
    add_links("libc.a")
    
    -- 编译选项（基于CCS项目配置）
    add_cflags("-v28", "-ml", "-mt", {force = true})
    add_cflags("--cla_support=cla2", {force = true})
    add_cflags("--float_support=fpu32", {force = true})
    add_cflags("--tmu_support=tmu1", {force = true})
    add_cflags("-O0", {force = true})
    add_cflags("--opt_for_speed=5", {force = true})
    add_cflags("--fp_mode=relaxed", {force = true})
    add_cflags("-g", {force = true})
    add_cflags("--symdebug:dwarf_version=3", {force = true})
    add_cflags("--c11", {force = true})
    add_cflags("--float_operations_allowed=all", {force = true})
    add_cflags("--diag_warning=225", {force = true})
    add_cflags("--display_error_number", {force = true})
    add_cflags("--abi=eabi", {force = true})
    
    -- 汇编选项
    add_asflags("-v28", "-ml", "-mt", {force = true})
    add_asflags("--cla_support=cla2", {force = true})
    add_asflags("--float_support=fpu32", {force = true})
    add_asflags("--tmu_support=tmu1", {force = true})
    add_asflags("--abi=eabi", {force = true})
    
    -- 预定义宏
    add_defines("_PLEXIM_")
    add_defines("CPU1")
    add_defines("EXTERNAL_MODE=1")
    add_defines("_FLASH")
    
    -- 设置输出目录
    set_targetdir("user/build")
    set_objectdir("user/build/.obj")
    set_dependir("user/build/.deps")
    
    -- 设置规则目录
    set_rundir("user/build")
    
    -- 自定义链接规则
    on_link(function (target)
        local linker = TI_COMPILER_ROOT .. "/bin/cl2000"
        local out_file = target:targetfile()
        local build_dir = path.directory(out_file)
        local objectfiles = target:objectfiles()
        
        -- 确保build目录存在
        if not os.exists(build_dir) then
            os.mkdir(build_dir)
        end
        
        -- 构建链接命令
        local args = {
            "-v28", "-ml", "-mt",
            "--cla_support=cla2",
            "--float_support=fpu32",
            "--tmu_support=tmu1",
            "--abi=eabi",
            "-z",
            "--output_file=" .. out_file
        }
        
        -- 添加所有对象文件
        for _, objfile in ipairs(objectfiles) do
            table.insert(args, objfile)
        end
        
        -- 添加库搜索路径
        table.insert(args, "-i" .. TI_COMPILER_ROOT .. "/lib")
        table.insert(args, "-i" .. TI_COMPILER_ROOT .. "/include")
        
        -- 添加链接的库
        table.insert(args, "-llibc.a")
        
        -- 添加链接器命令文件
        table.insert(args, "app/f28p65x_flash_lnk_cpu1.cmd")
        table.insert(args, "app/f28p65x_headers_nonBIOS_cpu1.cmd")
        
        -- 添加其他库文件
        table.insert(args, "drv/tisrc/dcl/plx_dcl_fpu32_eabi.lib")
        table.insert(args, "drv/tisrc/driverlib/plx_driverlib_fpu32_eabi.lib")
        table.insert(args, "mid/pil/pil_c28.a")
        table.insert(args, "drv/tisrc/driverlib/plx_driverlib.a")
        table.insert(args, "drv/tisrc/dcl/plx_dcl.a")
        
        -- 添加链接选项（所有输出文件指向build目录）
        table.insert(args, "-m" .. path.join(build_dir, TARGET_NAME .. ".map"))
        table.insert(args, "--stack_size=0x600")
        table.insert(args, "--warn_sections")
        table.insert(args, "--reread_libs")
        table.insert(args, "--xml_link_info=" .. path.join(build_dir, TARGET_NAME .. "_linkInfo.xml"))
        table.insert(args, "--rom_model")
        
        print("Linking: " .. linker .. " " .. table.concat(args, " "))
        os.execv(linker, args)
    end)

    
    -- 构建后生成.bin和.hex文件（基于CCS编译输出）
    after_build(function (target)
        local out_file = target:targetfile()
        local build_dir = path.directory(out_file)
        local base_name = path.basename(out_file)
        local bin_file = path.join(build_dir, base_name .. ".bin")
        local hex_file = path.join(build_dir, base_name .. ".hex")
        
        -- TI工具路径（严格按照CCS输出格式，使用绝对路径并带.exe后缀）
        local hex2000 = TI_COMPILER_ROOT .. "/bin/hex2000.exe"
        local ofd2000 = TI_COMPILER_ROOT .. "/bin/ofd2000.exe"
        local tiobj2bin = TI_OBJ2BIN_ROOT .. "/tiobj2bin.bat"
        local mkhex4bin = TI_OBJ2BIN_ROOT .. "/mkhex4bin.exe"
        
        -- 检查.out文件是否存在
        if not os.exists(out_file) then
            print("ERROR: Output file not found: " .. out_file)
            return
        end
        
        print("Generating .bin and .hex files...")
        print("Input: " .. out_file)
        print("Output directory: " .. build_dir)
        
        -- 1. 生成.hex文件（使用--intel --binary生成Intel HEX二进制格式）
        print("\n=== Generating HEX file ===")
        local hex_args = {
            "--memwidth=16",
            "--order=LS",
            "--romwidth=16",
            "--diag_wrap=off",
            "--ascii",
            "-o", hex_file,
            out_file
        }
        
        print("Command: " .. hex2000 .. " " .. table.concat(hex_args, " "))
        local ret = os.execv(hex2000, hex_args)
        if ret ~= 0 then
            print("WARNING: hex2000 failed with code " .. ret)
        else
            if os.exists(hex_file) then
                local hex_size = os.filesize(hex_file)
                print("Successfully generated: " .. hex_file .. " (" .. hex_size .. " bytes)")
            else
                print("ERROR: Hex file not created: " .. hex_file)
            end
        end
        
        -- 2. 生成.bin文件（集成tiobj2bin.bat逻辑，避免批处理阻塞）
        print("\n=== Generating BIN file ===")
        
        -- 生成临时文件名
        local xmltmp = os.tmpfile() .. ".xml"
        local hextmp = os.tmpfile() .. ".tmp"
        
        -- 步骤1: 使用ofd2000生成XML文件
        print("Step 1: Generating XML from .out file...")
        local ofd_args = {
            "-x",
            "--xml_indent=0",
            "--obj_display=none,sections,header,segments",
            out_file
        }
        
        -- 执行ofd2000并捕获输出到文件
        print("  " .. ofd2000 .. " " .. table.concat(ofd_args, " ") .. " > " .. xmltmp)
        local ofd_output = os.iorunv(ofd2000, ofd_args)
        if not ofd_output then
            print("ERROR: ofd2000 failed")
            return
        end
        local xmlfile = io.open(xmltmp, "w")
        if xmlfile then
            xmlfile:write(ofd_output)
            xmlfile:close()
        else
            print("ERROR: Failed to write XML file: " .. xmltmp)
            return
        end
        
        -- 步骤2: 使用mkhex4bin生成hex命令文件
        print("Step 2: Generating hex command file...")
        print("  " .. mkhex4bin .. " " .. xmltmp .. " > " .. hextmp)
        local mkhex_output = os.iorun(mkhex4bin .. " " .. xmltmp)
        if not mkhex_output then
            print("ERROR: mkhex4bin failed")
            os.rm(xmltmp)
            return
        end
        local hexfile = io.open(hextmp, "w")
        if hexfile then
            hexfile:write(mkhex_output)
            hexfile:close()
        else
            print("ERROR: Failed to write hex cmd file: " .. hextmp)
            os.rm(xmltmp)
            return
        end
        
        -- 步骤3: 使用hex2000生成二进制文件
        print("Step 3: Generating binary file...")
        local hex_bin_args = {
            "-q",
            "-b",
            "-image",
            "-o", bin_file,
            hextmp,
            out_file
        }
        
        print("  " .. hex2000 .. " " .. table.concat(hex_bin_args, " "))
        ret = os.execv(hex2000, hex_bin_args)
        if ret ~= 0 then
            print("ERROR: hex2000 binary generation failed with code " .. ret)
            os.rm(xmltmp)
            os.rm(hextmp)
            return
        end
        
        -- 清理临时文件
        print("Step 4: Cleaning up temporary files...")
        os.rm(xmltmp)
        os.rm(hextmp)
        
        if os.exists(bin_file) then
            local bin_size = os.filesize(bin_file)
            print("Successfully generated: " .. bin_file .. " (" .. bin_size .. " bytes)")
        else
            print("ERROR: Binary file was not created")
        end
        
        -- 验证生成的文件
        print("\n=== Build completed successfully! ===")
        print("Output files in build directory:")
        print("  OUT:  " .. out_file)
        
        if os.exists(hex_file) then
            local hex_size = os.filesize(hex_file)
            print("  HEX:  " .. hex_file .. " (" .. hex_size .. " bytes)")
        else
            print("  HEX:  " .. hex_file .. " (NOT GENERATED)")
        end
        
        if os.exists(bin_file) then
            local bin_size = os.filesize(bin_file)
            print("  BIN:  " .. bin_file .. " (" .. bin_size .. " bytes)")
        else
            print("  BIN:  " .. bin_file .. " (NOT GENERATED)")
        end
        
        -- 打印 MAP 文件信息
        local map_file = path.join(build_dir, base_name .. ".map")
        if os.exists(map_file) then
            local map_size = os.filesize(map_file)
            print("  MAP:  " .. map_file .. " (" .. map_size .. " bytes)")
            
            -- 解析 MAP 文件中的内存使用情况
            print("\n=== Memory Usage Summary ===")
            local map_content = io.readfile(map_file)
            if map_content then
                -- 解析 MEMORY CONFIGURATION 表格
                -- 格式: name origin length used unused attr fill
                local in_mem_config = false
                local header_found = false
                local flash_used = 0
                local flash_total = 0
                local ram_used = 0
                local ram_total = 0
                
                for line in map_content:gmatch("[^\r\n]+") do
                    -- 找到表头行 "name origin length used unused attr fill"
                    if line:match("^%s*name%s+origin") then
                        header_found = true
                        in_mem_config = true
                    -- 找到分隔符行，开始解析数据
                    elseif header_found and line:match("^%-%-%-%-%-%-%-%-") then
                        -- 分隔符，继续下一行
                    elseif in_mem_config and header_found then
                        -- 解析数据行: FLASH_BANK0 00080002 0001fffe 00004abb 0001b543 RWIX
                        local name, origin, length, used, unused = line:match("^%s*(%S+)%s+(%S+)%s+(%S+)%s+(%S+)%s+(%S+)")
                        if name and used then
                            local used_val = tonumber(used, 16) or 0
                            local length_val = tonumber(length, 16) or 0
                            
                            -- 计算 FLASH 使用情况 (FLASH_BANK0, FLASH_BANK1, FLASH_BANK2)
                            if name:match("FLASH_BANK") then
                                flash_used = flash_used + used_val
                                flash_total = flash_total + length_val
                            end
                            
                            -- 计算 RAM 使用情况 (RAMM0, RAMM1, RAMLS, RAMGS0-4)
                            if name:match("^RAM") then
                                ram_used = ram_used + used_val
                                ram_total = ram_total + length_val
                            end
                        end
                        
                        -- 空行或新的大标题（如 SECTIONS, GLOBAL 等）结束内存配置部分
                        -- 注意：不要匹配 "PAGE 0:" 这种格式
                        if line:match("^%s*$") or line:match("^[A-Z]+$") or line:match("^%u[%u_]+%u$") then
                            in_mem_config = false
                        end
                    elseif line:match("MEMORY CONFIGURATION") then
                        in_mem_config = true
                    end
                end
                
                -- 打印 FLASH 使用情况
                if flash_total > 0 then
                    local flash_percent = (flash_used / flash_total) * 100
                    print(string.format("  FLASH:  0x%05X / 0x%05X bytes (%5.1f%% used)", 
                        flash_used, flash_total, flash_percent))
                    print(string.format("          %d / %d KB", 
                        math.floor(flash_used / 1024), math.floor(flash_total / 1024)))
                end
                
                -- 打印 RAM 使用情况
                if ram_total > 0 then
                    local ram_percent = (ram_used / ram_total) * 100
                    print(string.format("  RAM:    0x%05X / 0x%05X bytes (%5.1f%% used)", 
                        ram_used, ram_total, ram_percent))
                    print(string.format("          %d / %d KB", 
                        math.floor(ram_used / 1024), math.floor(ram_total / 1024)))
                end
            end
        else
            print("  MAP:  " .. map_file .. " (NOT GENERATED)")
        end


    end)
    
target_end()
