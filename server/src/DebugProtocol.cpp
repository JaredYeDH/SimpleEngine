#include "DebugProtocol.h"
namespace vs {
	struct MetaMessage
	{
		Json js;
		MetaMessage()
		{
			js = Json::object();
		}
	};
	struct Message : public MetaMessage {
		Message()
		{
			js["id"] = 0;
			js["format"] = "";
			js["variables"] = Json::object();
			js["sendTelemetry"] = true;
			js["showUser"] = true;
			js["url"] = "";
			js["urlLabel"] = "";
		}
	};
	struct Module : public MetaMessage {
		Module()
		{
			js["id"] = "";
			js["name"] = "";
			js["path"] = "";
			js["isOptimized"] = true;
			js["isUserCode"] = true;
			js["version"] = "";
			js["symbolStatus"] = "";
			js["symbolFilePath"] = "";
			js["dateTimeStamp"] = "";
			js["addressRange"] = "";
		}
	};
	struct ColumnDescriptor : public MetaMessage {
		ColumnDescriptor()
		{
			js["attributeName"] = "";
			js["label"] = "";
			js["format"] = "";
			js["type"] = "";
			js["width"] = 0;
		}
	};
	struct ModulesViewDescriptor : public MetaMessage {
		ModulesViewDescriptor()
		{
			js["columns"] = Json::array({ ColumnDescriptor().js });
		}
	};
	struct Thread : public MetaMessage {
		Thread()
		{
			js["id"] = 0;
			js["name"] = "";
		}
	};
	struct Checksum : public MetaMessage {
		Checksum()
		{
			js["algorithm"] = "";
			js["checksum"] = "";
		}
	};
	struct Source : public MetaMessage {
		Source()
		{
			js["name"] = "";
			js["path"] = "";
			js["sourceReference"] = 0;
			js["presentationHint"] = "";
			js["origin"] = "";
			js["sources"] = Json::array({ Source().js });
			js["adapterData"] = Json::object();
			js["checksums"] = Json::array({ Checksum().js });
		}
	};
	struct StackFrame : public MetaMessage {
		StackFrame()
		{
			js["id"] = 0;
			js["name"] = "";
			js["source"] = Source().js;
			js["line"] = 0;
			js["column"] = 0;
			js["endLine"] = 0;
			js["endColumn"] = 0;
			js["moduleId"] = "";
			js["presentationHint"] = "";
		}
	};
	struct Scope : public MetaMessage {
		Scope()
		{
			js["name"] = "";
			js["variablesReference"] = 0;
			js["namedVariables"] = 0;
			js["indexedVariables"] = 0;
			js["expensive"] = true;
			js["source"] = Source().js;
			js["line"] = 0;
			js["column"] = 0;
			js["endLine"] = 0;
			js["endColumn"] = 0;
		}
	};

	struct VariablePresentationHint : public MetaMessage {
		VariablePresentationHint()
		{
			js["kind"] = "";
			js["attributes"] = Json::array({ "" });
			js["visibility"] = "";
		}
	};
	struct Variable : public MetaMessage {
		Variable()
		{
			js["name"] = "";
			js["value"] = "";
			js["type"] = "";
			js["presentationHint"] = VariablePresentationHint().js;
			js["evaluateName"] = "";
			js["variablesReference"] = 0;
			js["namedVariables"] = 0;
			js["indexedVariables"] = 0;
		}
	};
	struct SourceBreakpoint : public MetaMessage {
		SourceBreakpoint()
		{
			js["line"] = 0;
			js["column"] = 0;
			js["condition"] = "";
			js["hitCondition"] = "";
			js["logMessage"] = "";
		}
	};
	struct FunctionBreakpoint : public MetaMessage {
		FunctionBreakpoint()
		{
			js["name"] = "";
			js["condition"] = "";
			js["hitCondition"] = "";
		}
	};
	struct Breakpoint : public MetaMessage {
		Breakpoint()
		{
			js["id"] = 0;
			js["verified"] = true;
			js["message"] = "";
			js["source"] = Source().js;
			js["line"] = 0;
			js["column"] = 0;
			js["endLine"] = 0;
			js["endColumn"] = 0;
		}
	};
	struct StepInTarget : public MetaMessage {
		StepInTarget()
		{
			js["id"] = 0;
			js["label"] = "";
		}
	};
	struct GotoTarget : public MetaMessage {
		GotoTarget()
		{
			js["id"] = 0;
			js["label"] = "";
			js["line"] = 0;
			js["column"] = 0;
			js["endLine"] = 0;
			js["endColumn"] = 0;
		}
	};
	struct CompletionItem : public MetaMessage {
		CompletionItem()
		{
			js["label"] = "";
			js["text"] = "";
			js["type"] = "";
			js["start"] = 0;
			js["length"] = 0;
		}
	};

	struct ValueFormat : public MetaMessage {
		ValueFormat()
		{
			js["hex"] = true;
		}
	};

	struct ExceptionPathSegment : public MetaMessage {
		ExceptionPathSegment()
		{
			js["negate"] = true;
			js["names"] = Json::array({ "" });
		}
	};
	struct ExceptionOptions : public MetaMessage {
		ExceptionOptions()
		{
			js["path"] = Json::array({ ExceptionPathSegment().js });
			js["breakMode"] = "";
		}
	};
	struct ExceptionDetails : public MetaMessage {
		ExceptionDetails()
		{
			js["message"] = "";
			js["typeName"] = "";
			js["fullTypeName"] = "";
			js["evaluateName"] = "";
			js["stackTrace"] = "";
			js["innerException"] = Json::array({ ExceptionDetails().js });
		}
	};
	struct InitializeRequestArguments : public MetaMessage {
		InitializeRequestArguments()
		{
			js["clientID"] = "";
			js["adapterID"] = "";
			js["locale"] = "1";
			js["linesStartAt1"] = true;
			js["columnsStartAt1"] = true;
			js["pathFormat"] = "";
			js["supportsVariableType"] = false;
			js["supportsVariablePaging"] = false;
			js["supportsRunInTerminalRequest "] = false;
		}
	};
	struct ConfigurationDoneArguments : public MetaMessage {
	};
	struct LaunchRequestArguments : public MetaMessage {
		LaunchRequestArguments()
		{
			js["noDebug"] = false;
		}
	};
	struct AttachRequestArguments : public MetaMessage {
	};
	struct RestartArguments : public MetaMessage {
	};
	struct DisconnectArguments : public MetaMessage {
		DisconnectArguments()
		{
			js["terminateDebuggee"] = true;
		}
	};
	struct SetBreakpointsArguments : public MetaMessage {
		SetBreakpointsArguments()
		{
			js["source"] = Source().js;
			Json sourceBreakpoints = Json::parse("[]");
			sourceBreakpoints[0] = SourceBreakpoint().js;
			js["breakpoints"] = sourceBreakpoints;
			js["lines"] = Json::parse("[]");
			js["sourceModified"] = false;
		}
	};
	struct SetFunctionBreakpointsArguments : public MetaMessage {
		SetFunctionBreakpointsArguments()
		{
			js["breakpoints"] = FunctionBreakpoint().js;
		}
	};
	struct SetExceptionBreakpointsArguments : public MetaMessage {
		SetExceptionBreakpointsArguments()
		{
			js["filters"] = Json::array();
			js["exceptionOptions"] = Json::array({ ExceptionOptions().js });
		}
	};
	struct ContinueArguments : public MetaMessage {
		ContinueArguments()
		{
			js["threadId"] = 0;
		}
	};
	struct NextArguments : public MetaMessage {
		NextArguments()
		{
			js["threadId"] = 0;
		}
	};
	struct StepInArguments : public MetaMessage {
		StepInArguments()
		{
			js["threadId"] = 0;
			js["targetId"] = 0;
		}
	};
	struct StepOutArguments : public MetaMessage {
		StepOutArguments()
		{
			js["threadId"] = 0;
		}
	};
	struct StepBackArguments : public MetaMessage {
		StepBackArguments()
		{
			js["threadId"] = 0;
		}
	};
	struct ReverseContinueArguments : public MetaMessage {
		ReverseContinueArguments()
		{
			js["threadId"] = 0;
		}
	};
	struct RestartFrameArguments : public MetaMessage {
		RestartFrameArguments()
		{
			js["frameId"] = 0;
		}
	};
	struct GotoArguments : public MetaMessage {
		GotoArguments()
		{
			js["threadId"] = 0;
			js["targetId"] = 0;
		}
	};
	struct PauseArguments : public MetaMessage {
		PauseArguments()
		{
			js["threadId"] = 0;
		}
	};
	struct StackFrameFormat : public ValueFormat {
		StackFrameFormat()
		{
			js["parameters"] = true;
			js["parameterTypes"] = true;
			js["parameterNames"] = true;
			js["parameterValues"] = true;
			js["line"] = true;
			js["module"] = true;
			js["includeAll"] = true;
		}
	};
	struct StackTraceArguments : public MetaMessage {
		StackTraceArguments()
		{
			js["threadId"] = 0;
			js["startFrame"] = 0;
			js["levels"] = 0;
			js["format"] = StackFrameFormat().js;
		}
	};
	struct ScopesArguments : public MetaMessage {
		ScopesArguments()
		{
			js["frameId"] = 0;
		}
	};
	struct VariablesArguments : public MetaMessage {
		VariablesArguments()
		{
			js["variablesReference"] = 0;
			js["filter"] = "";
			js["start"] = 0;
			js["count"] = 0;
			js["format"] = ValueFormat().js;
		}
	};
	struct SetVariableArguments : public MetaMessage {
		SetVariableArguments()
		{
			js["variablesReference"] = 0;
			js["name"] = "";
			js["value"] = "";
			js["format"] = ValueFormat().js;
		}
	};
	struct SourceArguments : public MetaMessage {
		SourceArguments()
		{
			js["source"] = Source().js;
			js["sourceReference"] = 0;
		}
	};
	struct ModulesArguments : public MetaMessage {
		ModulesArguments()
		{
			js["startModule"] = 0;
			js["moduleCount"] = 0;
		}
	};
	struct LoadedSourcesArguments : public MetaMessage {
	};
	struct EvaluateArguments : public MetaMessage {
		EvaluateArguments()
		{
			js["expression"] = "";
			js["frameId"] = 0;
			js["context"] = "";
			js["format"] = ValueFormat().js;
		}
	};
	struct StepInTargetsArguments : public MetaMessage {
		StepInTargetsArguments()
		{
			js["frameId"] = 0;
		}
	};
	struct GotoTargetsArguments : public MetaMessage {
		GotoTargetsArguments()
		{
			js["source"] = Source().js;
			js["line"] = 0;
			js["column"] = 0;
		}
	};
	struct CompletionsArguments : public MetaMessage {
		CompletionsArguments()
		{
			js["frameId"] = 0;
			js["text"] = "";
			js["column"] = 0;
			js["line"] = 0;
		}
	};
	struct ExceptionInfoArguments : public MetaMessage {
		ExceptionInfoArguments()
		{
			js["threadId"] = 0;
		}
	};
	struct ExceptionBreakpointsFilter : public MetaMessage {
		ExceptionBreakpointsFilter() {
			js["filter"] = "";
			js["label"] = "";
			js["default"] = true;
		}
	};
	struct Capabilities : public MetaMessage {
		Capabilities()
		{
			js["supportsConfigurationDoneRequest"] = true;
			js["supportsFunctionBreakpoints"] = true;
			js["supportsConditionalBreakpoints"] = true;
			js["supportsHitConditionalBreakpoints"] = true;
			js["supportsEvaluateForHovers"] = true;
			js["exceptionBreakpointFilters"] = Json::array({ ExceptionBreakpointsFilter().js });
			js["supportsStepBack"] = true;
			js["supportsSetVariable"] = true;
			js["supportsRestartFrame"] = true;
			js["supportsGotoTargetsRequest"] = true;
			js["supportsStepInTargetsRequest"] = true;
			js["supportsCompletionsRequest"] = true;
			js["supportsModulesRequest"] = true;
			js["additionalModuleColumns"] = Json::array({ ColumnDescriptor().js });
			js["supportedChecksumAlgorithms"] = Json::array({ "" });
			js["supportsRestartRequest"] = true;
			js["supportsExceptionOptions"] = true;
			js["supportsValueFormattingOptions"] = true;
			js["supportsExceptionInfoRequest"] = true;
			js["supportTerminateDebuggee"] = true;
			js["supportsDelayedStackTraceLoading"] = true;
			js["supportsLoadedSourcesRequest"] = true;
			js["supportsLogPoints"] = true;
		}
	};

	struct ProtocolMessage : public MetaMessage
	{
		ProtocolMessage()
		{
			js["seq"] = 0;
			js["type"] = "";
		}
	};
	struct Request :public ProtocolMessage
	{
		Request()
		{
			js["type"] = "request";
			js["command"] = "";
			js["arguments"] = Json::object();
		}
	};
	struct Event : public ProtocolMessage
	{
		Event()
		{
			js["type"] = "event";
			js["event"] = "";
			js["body"] = Json::object();
		}
	};
	struct Response : public ProtocolMessage
	{
		Response()
		{
			js["type"] = "response";
			js["request_seq"] = 0;
			js["success"] = true;
			js["command"] = "";
			js["message"] = "";
			js["body"] = Json::object();
		}
	};

	struct InitializedEvent : public Event
	{
		InitializedEvent()
		{
			js["event"] = "initialized";
		}
	};

	struct StoppedEvent : public Event
	{
		StoppedEvent()
		{
			js["event"] = "stopped";
			auto& body = js["body"];
			body["reason"] = "";
			body["description"] = "";
			body["threadId"] = 0;
			body["text"] = "";
			body["allThreadsStopped"] = false;
		}
	};
	struct ContinuedEvent : public Event {
		ContinuedEvent()
		{
			js["event"] = "continued";
			auto& body = js["body"];
			body["threadId"] = 0;
			body["allThreadsContinued"] = false;
		}
	};
	struct ExitedEvent : public Event {
		ExitedEvent()
		{
			js["event"] = "exited";
			auto& body = js["body"];
			body["exitCode"] = 0;
		}
	};
	struct TerminatedEvent : public Event {
		TerminatedEvent()
		{
			js["event"] = "terminated";
			auto& body = js["body"];
			body["restart"] = Json::object();
		}
	};
	struct ThreadEvent : public Event {
		ThreadEvent()
		{
			js["event"] = "thread";
			auto& body = js["body"];
			body["reason"] = "";
			body["threadId"] = 0;
		}
	};
	struct OutputEvent : public Event {
		OutputEvent()
		{
			js["event"] = "output";
			auto& body = js["body"];
			body["category"] = "console";
			body["output"] = "";
			body["variablesReference"] = 0;
			body["source"] = "";
			body["line"] = 0;
			body["column"] = 0;
			body["data"] = Json::object();
		}
	};
	struct BreakpointEvent : public Event {
		BreakpointEvent()
		{
			js["event"] = "breakpoint";
			auto& body = js["body"];
			body["reason"] = "";
			body["breakpoint"] = Json::object();
		}
	};
	struct ModuleEvent : public Event {
		ModuleEvent()
		{
			js["event"] = "module";
			auto& body = js["body"];
			body["reason"] = "";
			body["module"] = Json::object();
		}
	};
	struct LoadedSourceEvent : public Event {
		LoadedSourceEvent()
		{
			js["event"] = "loadedSource";
			auto& body = js["body"];
			body["reason"] = "";
			body["source"] = Json::object();
		}
	};

	struct ProcessEvent : public Event {
		ProcessEvent()
		{
			js["event"] = "process";
			auto& body = js["body"];
			body["name"] = "";
			body["systemProcessId"] = 0;
			body["isLocalProcess"] = true;
			body["startMethod"] = "";
		}
	};
	struct CapabilitiesEvent : public Event {
		CapabilitiesEvent()
		{
			js["event"] = "capabilities";
			auto& body = js["body"];
			body["capabilities"] = Json::object();
		}
	};
	struct RunInTerminalRequest : public Request {
		RunInTerminalRequest()
		{
			js["type"] = "request";
			js["command"] = "runInTerminal";
			js["arguments"] = Json::object();
		}
	};
	struct RunInTerminalRequestArguments : public MetaMessage
	{
		RunInTerminalRequestArguments()
		{
			js["kind"] = "";
			js["title"] = "";
			js["cwd"] = "";
			js["args"] = Json::parse("[]");
			js["env"] = Json::object();
		}
	};
	struct RunInTerminalResponse : public Response {
		RunInTerminalResponse()
		{
			auto& body = js["body"];
			body["processId"] = 0;
		}
	};
	struct ErrorResponse : public Response {
		ErrorResponse()
		{
			js["success"] = false;
			auto& body = js["body"];
			body["error"] = Json::object();
		}
	};
	struct InitializeRequest : public Request {
		InitializeRequest()
		{
			js["command"] = "initialize";
			js["arguments"] = InitializeRequestArguments().js;
		}
	};
	struct InitializeResponse : public Response {
		InitializeResponse()
		{
			js["command"] = "initialize";
			js["body"] = Capabilities().js;
		}
	};

	struct ConfigurationDoneRequest : public Request {
		ConfigurationDoneRequest()
		{
			js["command"] = "configurationDone";
			js["arguments"] = ConfigurationDoneArguments().js;
		}
	};
	struct ConfigurationDoneResponse : public Response {
		ConfigurationDoneResponse()
		{
			js["command"] = "configurationDone";
		}
	};
	struct LaunchRequest : public Request {
		LaunchRequest()
		{
			js["command"] = "launch";
			js["arguments"] = LaunchRequestArguments().js;
		}
	};
	struct LaunchResponse : public Response {
		LaunchResponse()
		{
			js["command"] = "launch";
		}
	};
	struct AttachRequest : public Request {
		AttachRequest()
		{
			js["command"] = "attach";
			js["arguments"] = AttachRequestArguments().js;
		}
	};
	struct AttachResponse : public Response {
		AttachResponse()
		{
			js["command"] = "attach";
		}
	};
	struct RestartRequest : public Request {
		RestartRequest()
		{
			js["command"] = "restart";
			js["arguments"] = RestartArguments().js;
		}
	};
	struct RestartResponse : public Response {
		RestartResponse()
		{
			js["command"] = "restart";
		}
	};
	struct DisconnectRequest : public Request {
		DisconnectRequest()
		{
			js["command"] = "disconnect";
			js["arguments"] = DisconnectArguments().js;
		}
	};
	struct DisconnectResponse : public Response {
		DisconnectResponse()
		{
			js["command"] = "disconnect";
		}
	};
	struct SetBreakpointsRequest : public Request {
		SetBreakpointsRequest()
		{
			js["command"] = "setBreakpoints";
			js["arguments"] = SetBreakpointsArguments().js;
		}
	};
	struct SetBreakpointsResponse : public Response {
		SetBreakpointsResponse()
		{
			js["command"] = "setBreakpoints";
			auto& body = js["body"];
			body["breakpoints"] = Breakpoint().js;
		}
	};
	struct SetFunctionBreakpointsRequest : public Request {
		SetFunctionBreakpointsRequest()
		{
			js["command"] = "setFunctionBreakpoints";
			js["arguments"] = SetFunctionBreakpointsArguments().js;
		}
	};
	struct SetFunctionBreakpointsResponse : public Response {
		SetFunctionBreakpointsResponse()
		{
			js["command"] = "setFunctionBreakpoints";
			auto& body = js["body"];
			body["breakpoints"] = Breakpoint().js;
		}
	};
	struct SetExceptionBreakpointsRequest : public Request {
		SetExceptionBreakpointsRequest()
		{
			js["command"] = "setExceptionBreakpoints";
			js["arguments"] = SetExceptionBreakpointsArguments().js;
		}
	};

	struct SetExceptionBreakpointsResponse : public Response {
		SetExceptionBreakpointsResponse()
		{
			js["command"] = "setExceptionBreakpoints";
		}
	};
	struct ContinueRequest : public Request {
		ContinueRequest()
		{
			js["command"] = "continue";
			js["arguments"] = ContinueArguments().js;
		}
	};
	struct ContinueResponse : public Response {
		ContinueResponse()
		{
			js["command"] = "continue";
			auto& body = js["body"];
			body["allThreadsContinued"] = false;
		}
	};
	struct NextRequest : public Request {
		NextRequest()
		{
			js["command"] = "next";
			js["arguments"] = NextArguments().js;
		}
	};
	struct NextResponse : public Response {
		NextResponse()
		{
			js["command"] = "next";
		}
	};
	struct StepInRequest : public Request {
		StepInRequest()
		{
			js["command"] = "stepIn";
			js["arguments"] = StepInArguments().js;
		}
	};
	struct StepInResponse : public Response {
		StepInResponse()
		{
			js["command"] = "stepIn";
		}
	};

	struct StepOutRequest : public Request {
		StepOutRequest()
		{
			js["command"] = "stepOut";
			js["arguments"] = StepOutArguments().js;
		}
	};
	struct StepOutResponse : public Response {
		StepOutResponse()
		{
			js["command"] = "stepOut";
		}
	};
	struct StepBackRequest : public Request {
		StepBackRequest()
		{
			js["command"] = "stepBack";
			js["arguments"] = StepBackArguments().js;
		}
	};
	struct StepBackResponse : public Response {
		StepBackResponse()
		{
			js["command"] = "stepBack";
		}
	};
	struct ReverseContinueRequest : public Request {
		ReverseContinueRequest()
		{
			js["command"] = "reverseContinue";
			js["arguments"] = ReverseContinueArguments().js;
		}
	};
	struct ReverseContinueResponse : public Response {
		ReverseContinueResponse()
		{
			js["command"] = "reverseContinue";
		}
	};
	struct RestartFrameRequest : public Request {
		RestartFrameRequest()
		{
			js["command"] = "restartFrame";
			js["arguments"] = RestartFrameArguments().js;
		}
	};
	struct RestartFrameResponse : public Response {
		RestartFrameResponse()
		{
			js["command"] = "restartFrame";
		}
	};
	struct GotoRequest : public Request {
		GotoRequest()
		{
			js["command"] = "goto";
			js["arguments"] = GotoArguments().js;
		}
	};
	struct GotoResponse : public Response {
		GotoResponse()
		{
			js["command"] = "goto";
		}
	};
	struct PauseRequest : public Request {
		PauseRequest()
		{
			js["command"] = "pause";
			js["arguments"] = PauseArguments().js;
		}
	};
	struct PauseResponse : public Response {
		PauseResponse()
		{
			js["command"] = "pause";
		}
	};
	struct StackTraceRequest : public Request {
		StackTraceRequest()
		{
			js["command"] = "stackTrace";
			js["arguments"] = StackTraceArguments().js;
		}
	};
	struct StackTraceResponse : public Response {
		StackTraceResponse()
		{
			js["command"] = "stackTrace";
			auto& body = js["body"];
			body["stackFrames"] = Json::array({ StackFrame().js });
			body["totalFrames"] = 0;
		}
	};
	struct ScopesRequest : public Request {
		ScopesRequest()
		{
			js["command"] = "scopes";
			js["arguments"] = ScopesArguments().js;
		}
	};
	struct ScopesResponse : public Response {
		ScopesResponse()
		{
			js["command"] = "scopes";
			auto& body = js["body"];
			body["scopes"] = Json::array({ Scope().js });
		}
	};
	struct VariablesRequest : public Request {
		VariablesRequest()
		{
			js["command"] = "variables";
			js["arguments"] = VariablesArguments().js;
		}
	};
	struct VariablesResponse : public Response {
		VariablesResponse()
		{
			js["command"] = "variables";
			auto& body = js["body"];
			body["variables"] = Json::array({ Variable().js });
		}
	};
	struct SetVariableRequest : public Request {
		SetVariableRequest()
		{
			js["command"] = "setVariable";
			js["arguments"] = SetVariableArguments().js;
		}
	};
	struct SetVariableResponse : public Response {
		SetVariableResponse()
		{
			js["command"] = "setVariable";
			auto& body = js["body"];
			body["value"] = "";
			body["type"] = "";
			body["variablesReference"] = 0;
			body["namedVariables"] = 0;
			body["indexedVariables"] = 0;
		}
	};
	struct SourceRequest : public Request {
		SourceRequest()
		{
			js["command"] = "source";
			js["arguments"] = SourceArguments().js;
		}
	};
	struct SourceResponse : public Response {
		SourceResponse()
		{
			js["command"] = "source";
			auto& body = js["body"];
			body["content"] = "";
			body["mimeType"] = "";
		}
	};
	struct ThreadsRequest : public Request {
		ThreadsRequest()
		{
			js["command"] = "threads";
		}
	};
	struct ThreadsResponse : public Response {
		ThreadsResponse()
		{
			js["command"] = "threads";
			auto& body = js["body"];
			body["threads"] = Json::array({ Thread().js });
		}
	};
	struct ModulesRequest : public Request {
		ModulesRequest()
		{
			js["command"] = "modules";
			js["arguments"] = ModulesArguments().js;
		}
	};
	struct ModulesResponse : public Response {
		ModulesResponse()
		{
			js["command"] = "modules";
			auto& body = js["body"];
			body["modules"] = Json::array({ Module().js });
			body["totalModules"] = 0;
		}
	};
	struct LoadedSourcesRequest : public Request {
		LoadedSourcesRequest()
		{
			js["command"] = "loadedSources";
			js["arguments"] = LoadedSourcesArguments().js;
		}
	};
	struct LoadedSourcesResponse : public Response {
		LoadedSourcesResponse()
		{
			js["command"] = "loadedSources";
			auto& body = js["body"];
			body["sources"] = Json::array({ Source().js });
		}
	};
	struct EvaluateRequest : public Request {
		EvaluateRequest()
		{
			js["command"] = "evaluate";
			js["arguments"] = EvaluateArguments().js;
		}
	};
	struct EvaluateResponse : public Response {
		EvaluateResponse()
		{
			js["command"] = "evaluate";
			auto& body = js["body"];
			body["result"] = "";
			body["type"] = "";
			body["presentationHint"] = VariablePresentationHint().js;
			body["variablesReference"] = 0;
			body["namedVariables"] = 0;
			body["indexedVariables"] = 0;
		}
	};
	struct StepInTargetsRequest : public Request {
		StepInTargetsRequest()
		{
			js["command"] = "stepInTargets";
			js["arguments"] = StepInTargetsArguments().js;
		}
	};
	struct StepInTargetsResponse : public Response {
		StepInTargetsResponse()
		{
			js["command"] = "stepInTargets";
			auto& body = js["body"];
			body["targets"] = Json::array({ StepInTarget().js });
		}
	};
	struct GotoTargetsRequest : public Request {
		GotoTargetsRequest()
		{
			js["command"] = "gotoTargets";
			js["arguments"] = GotoTargetsArguments().js;
		}
	};
	struct GotoTargetsResponse : public Response {
		GotoTargetsResponse()
		{
			js["command"] = "gotoTargets";
			auto& body = js["body"];
			body["targets"] = Json::array({ GotoTarget().js });
		}
	};
	struct CompletionsRequest : public Request {
		CompletionsRequest()
		{
			js["command"] = "completions";
			js["arguments"] = CompletionsArguments().js;
		}
	};
	struct CompletionsResponse : public Response {
		CompletionsResponse()
		{
			js["command"] = "completions";
			auto& body = js["body"];
			body["targets"] = Json::array({ CompletionItem().js });
		}
	};
	struct ExceptionInfoRequest : public Request {
		ExceptionInfoRequest()
		{
			js["command"] = "exceptionInfo";
			js["arguments"] = ExceptionInfoArguments().js;
		}
	};
	struct ExceptionInfoResponse : public Response {
		ExceptionInfoResponse()
		{
			js["command"] = "exceptionInfo";
			auto& body = js["body"];
			body["exceptionId"] = "";
			body["description"] = "";
			body["breakMode"] = "";
			body["details"] = ExceptionDetails().js;
		}
	};

}
