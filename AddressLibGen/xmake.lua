target("addresslibgen")
    -- set build by default
    set_default(false)

    -- set build group
    set_group("tool")

    -- add source files
    add_files("src/**.cpp")
