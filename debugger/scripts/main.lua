-- local function debugger_loop(server)

-- end
-- coro_debugger = coroutine.create(debugger_loop)
local trap_wait = false

function debug_hook(event, line)
    print('debug_hook',event,line)
    while trap_wait do
        lua_run_loop()
        lua_sleep(10)
    end
end

function SetHook()
    print('Run()')
    debug.sethook(debug_hook, "lcr")

end

function SetTrapWait(bWait)
    print(bWait)
    trap_wait= bWait
end

local update = 0
function OnGameUpdate()
    print('OnGameUpdate' .. tostring(update))
    update = update + 1
    local c = 3 
    local d = 1
    local e = 4

    do 
        local c = 3 
        local d = 1
        local e = 4
    end

    do 
        local c = 3 
        local d = 1
        local e = 4
    end


    do 
        local c = 3 
        local d = 1
        local e = 4
    end


    do 
        local c = 3 
        local d = 1
        local e = 4
    end


    do 
        local c = 3 
        local d = 1
        local e = 4
    end


    do 
        local c = 3 
        local d = 1
        local e = 4
    end


    do 
        local c = 3 
        local d = 1
        local e = 4
    end


    do 
        local c = 3 
        local d = 1
        local e = 4
    end

    do 
        local c = 3 
        local d = 1
        local e = 4
    end
    do 
        local c = 3 
        local d = 1
        local e = 4
    end

end

function DoSomething()

end


print("hello world")