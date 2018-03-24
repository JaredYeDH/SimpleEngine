local json = require("cjson")

io.input("debugger_protocol.txt")

function handle_input()
	while true do
		local line = io.read("*L")
		if string.match(line,"Content-Length:") then
			line = io.read("*L")
			
		end
	end
	return {}
end
while true do
	local c = io.read(1)
	if c == ":" then
		local num = io.read("*n")
		io.read("*L")
		io.read("*L")
		local s = io.read(num)
		print("s:"..s)
		break
	end
end
local f,err = io.open("debugger_protocol.txt","r")
if err then print(err) end

print("hello2")
local line = f:read("*l")
local line = f:read("*l")
print(line)

