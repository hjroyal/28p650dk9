-- xmake.lua for OPiL buck example
-- Builds opiltarget executable and opil_plecs DLL
-- Output directory: buck/output
--
-- Usage:
--   # Configure and build all targets (default)
--   xmake f -p windows    # Configure for Windows (MSVC)
--   xmake f -p mingw      # Configure for MinGW
--   xmake f -p linux      # Configure for Linux
--   xmake                 # Build all targets
--
--   # Build specific target
--   xmake build opiltarget    # Build only controller executable
--   xmake build opil_plecs    # Build only PLECS DLL
--
--   # Clean build artifacts
--   xmake c               # Clean (remove output and .xmake directories)
--
--   # Show help
--   xmake --help
--
-- Output files:
--   output/opiltarget.exe        (Windows) or output/opiltarget (Linux)
--   output/libopil_plecs.dll     (Windows) or output/libopil_plecs.so (Linux)

set_project("opil-buck-example")
set_version("1.0.0")
set_languages("c11")
set_targetdir("build")

-- Platform detection and settings
if is_plat("windows") or is_plat("mingw") then
    add_defines("_WIN32")
    add_syslinks("ws2_32")
    add_defines("TARGET_COMM_SOCK_SERVER_PORT=8090")
    add_defines("HOST_COMM_SOCK_SERVER_IP=\"127.0.0.1\"")
    add_defines("HOST_COMM_SOCK_SERVER_PORT=8090")
elseif is_plat("linux") then
    add_defines("TARGET_COMM_SOCK_SERVER_PORT=8090")
    add_defines("HOST_COMM_SOCK_SERVER_IP=\"127.0.0.1\"")
    add_defines("HOST_COMM_SOCK_SERVER_PORT=8090")
end

-- Common include directories for all targets
add_includedirs("../..")
add_includedirs("../opil")
add_includedirs("../opil/comm")
add_includedirs("../opil/simif")
add_includedirs("../opil/ctlrif")
add_includedirs("../opil/logging")
add_includedirs("src")
add_includedirs(".")

-- Target 1: opiltarget executable (controller)
target("opiltarget")
    set_kind("binary")
    
    -- Add source files
    add_files("../opil/ctlrif/ctlrif.c")
    add_files("../opil/opiltarget.c")
    add_files("src/buckcontrol.c")
    add_files("src/main.c")
    
    -- Platform-specific communication source
    if is_plat("windows") or is_plat("mingw") then
        add_files("../opil/comm/win/targetCommSock.c")
    elseif is_plat("linux") then
        add_files("../opil/comm/linux/targetCommSock.c")
    end
    
    -- Debug information
    on_load(function (target)
        print("Building opiltarget for " .. os.host() .. "/" .. target:plat())
    end)

-- Target 2: opil_plecs DLL (PLECS interface)
target("opil_plecs")
    set_kind("shared")
    
    -- Add source files for PLECS DLL
    add_files("../opil/sw/plecs/opil_plecs.c")
    add_files("../opil/simif/simif.c")
    add_files("../opil/opilhost.c")
    
    -- Platform-specific communication source for host
    if is_plat("windows") or is_plat("mingw") then
        add_files("../opil/comm/win/hostCommSock.c")
    elseif is_plat("linux") then
        add_files("../opil/comm/linux/hostCommSock.c")
    end
    
    -- Set output name (libopil_plecs on Unix-like systems)
    if is_plat("windows") or is_plat("mingw") then
        set_filename("libopil_plecs.dll")
    else
        set_filename("libopil_plecs.so")
    end
    
    -- Debug information
    on_load(function (target)
        print("Building opil_plecs for " .. os.host() .. "/" .. target:plat())
    end)

-- Default build target (both)
target("all")
    set_kind("phony")
    add_deps("opiltarget", "opil_plecs")
    on_build(function (target)
        print("Build completed successfully!")
        print("Output files are in: " .. path.absolute("build"))
    end)