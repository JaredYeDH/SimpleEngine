
function GotoDebug()
    print("call goto debug")
    package.loaded[ 'mobdebug' ] = nil
    require("mobdebug").start()
end

function OnGameInit()
    
    -- print("main.lua OnGameInit")
end

function OnGameUpdate(dt)
    -- print("main.lua OnGameUpdate" .. "\t dt:" .. tostring(dt))
    
end

function OnGameDraw()
    -- print("main.lua OnGameDraw")
end