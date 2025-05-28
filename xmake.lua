-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibob64")

-- set project
set_project("WhichKey-OB64")
set_version("1.2.0")

-- set defaults
set_arch("x64")
set_languages("c++23")
set_optimize("faster")
set_warnings("allextra", "error")
set_defaultmode("releasedbg")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- enable ini support in commonlibob64
set_config("commonlib_ini", true)

add_defines("NOMINMAX")

-- targets
target("WhichKey-OB64")
    -- add dependencies to target
    add_deps("commonlibob64")

    -- add commonlibsse plugin
    add_rules("commonlibob64.plugin", {
        name = "WhichKey-OB64",
        author = "Water Face",
        description = "OBSE64 plugin to display the key name instead of lock level when appropriate"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
