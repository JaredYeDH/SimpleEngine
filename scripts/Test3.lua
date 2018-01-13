local x = 50
local y=50
local move = 0
local factor  =1
function OnUpdate()
    print("move.."..move)
    if move > 30 then
        factor =  - 1
    end
    if  move < 0 then 
        factor = 1
    end 
    move = move + factor
    for i=1,11,1 do
        DrawCircle(x*i,y*i+ move,100)
    end
    -- for i=1,11,1 do
    --     DrawCircle(x*i,y/2*i+ move/2,100)
    -- end

    -- for i=1,11,1 do
    --     DrawCircle(x/2*i,y/2*i+ move/4,100)
    -- end
end

