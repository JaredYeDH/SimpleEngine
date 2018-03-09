ProtocolMessage ={
    seq  = 0,  
    type = ""       --Values: 'request', 'response', 'event', etc.
}

Request = {
    seq = 0,
    type = "",
    command = "",
    arguments = "",
}

Event = {
    seq = 0,
    type = "",
    event = "",
    body = ""
}

Response = {
    seq = 0,
    type = "",
    request_seq = 0,
    success = true,
    command = "",
    message = "",
    body = ""
}

local json = require("cjson")
while true do 
	local vscode_request = io.lines()()
	--print(vscode_request)
	local rq =  json.decode(vscode_request)
    if rq.command == 'initialize' then
        local initEvent = {type = "event",seq = rq.request_seq + 1}
        initEvent =  json.encode(initEvent)
        io.write(initEvent)
        
        rq.type = "response"
        rq.request_seq =  rq.seq
        rq.seq = rq.seq + 1
        rq.success = true
        rq.body = {}
		rq.body.supportsConfigurationDoneRequest = true;
		rq.body.supportsEvaluateForHovers = true;
        rq.body.supportsStepBack = true;

        print(json.encode(rq))
    elseif rq.command == "launch" then
        rq.seq = rq.seq+1

        dofile()
    elseif rq.command == "setBreakpoints" then
        rq.seq = rq.seq+1
        local event = {
            seq = rq.seq+1,
            type = "event",
            event = "breakpoint",
            body = ""
        }
        
        print(json.encode(event))
    else
        rq.seq = rq.seq+1
        print(json.encode(rq))
    end
end 



io:read()
local t = {}
print("print")
require("mobdebug").listen() -- same as listen("*", 8172)