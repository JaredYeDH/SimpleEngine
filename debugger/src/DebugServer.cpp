#include "DebugServer.h"
#include "DebuggerCore.h"

#define MAIN_THREAD_ID 1
std::deque<Message> g_ReadQueue;
std::deque<Message> g_WriteQueue;

std::mutex g_ReadQueueMutex;
std::mutex g_WriteQueueMutex;


int g_MsgIdGen = 1;

namespace vs
{
	struct MetaMessage
	{
		Json js;
		MetaMessage()
			:js(Json::object())
		{

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
			js["sources"] = Json::object(); // Json::array({ Source().js });
			js["adapterData"] = Json::object();
			js["checksums"] = Json::array({ Checksum().js });
		}
	};
	struct StackFrame : public MetaMessage {
		StackFrame()
		{
			js["id"] = 0;
			js["name"] = "";
			js["source"] = Json::parse(Source().js.dump());
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
			js["source"] = Json::parse(Source().js.dump());
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
			js["presentationHint"] = Json::parse(VariablePresentationHint().js.dump());
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
			js["source"] = Json::parse(Source().js.dump());
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
			js["source"] = Json::parse(Source().js.dump());
			Json sourceBreakpoints = Json::parse("[]");
			sourceBreakpoints[0] = Json::parse(SourceBreakpoint().js.dump());
			js["breakpoints"] = sourceBreakpoints;
			js["lines"] = Json::parse("[]");
			js["sourceModified"] = false;
		}
	};
	struct SetFunctionBreakpointsArguments : public MetaMessage {
		SetFunctionBreakpointsArguments()
		{
			js["breakpoints"] = Json::parse(FunctionBreakpoint().js.dump());
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
			js["threadId"] = MAIN_THREAD_ID;
		}
	};
	struct NextArguments : public MetaMessage {
		NextArguments()
		{
			js["threadId"] = MAIN_THREAD_ID;
		}
	};
	struct StepInArguments : public MetaMessage {
		StepInArguments()
		{
			js["threadId"] = MAIN_THREAD_ID;
			js["targetId"] = MAIN_THREAD_ID;
		}
	};
	struct StepOutArguments : public MetaMessage {
		StepOutArguments()
		{
			js["threadId"] = MAIN_THREAD_ID;
		}
	};
	struct StepBackArguments : public MetaMessage {
		StepBackArguments()
		{
			js["threadId"] = MAIN_THREAD_ID;
		}
	};
	struct ReverseContinueArguments : public MetaMessage {
		ReverseContinueArguments()
		{
			js["threadId"] = MAIN_THREAD_ID;
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
			js["threadId"] = MAIN_THREAD_ID;
			js["targetId"] = 0;
		}
	};
	struct PauseArguments : public MetaMessage {
		PauseArguments()
		{
			js["threadId"] = MAIN_THREAD_ID;
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
			js["threadId"] = MAIN_THREAD_ID;
			js["startFrame"] = 0;
			js["levels"] = 0;
			js["format"] = Json::parse(StackFrameFormat().js.dump());
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
			js["format"] = Json::parse(ValueFormat().js.dump());
		}
	};
	struct SetVariableArguments : public MetaMessage {
		SetVariableArguments()
		{
			js["variablesReference"] = 0;
			js["name"] = "";
			js["value"] = "";
			js["format"] = Json::parse(ValueFormat().js.dump());
		}
	};
	struct SourceArguments : public MetaMessage {
		SourceArguments()
		{
			js["source"] = Json::parse(Source().js.dump());
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
			js["format"] = Json::parse(ValueFormat().js.dump());
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
			js["source"] = Json::parse(Source().js.dump());
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
			js["threadId"] = MAIN_THREAD_ID;
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
		void setRq(Json rq)
		{
			js["request_seq"] = rq["seq"];
			js["command"] = rq["command"];
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
			//	body["description"] = "";
			body["threadId"] = MAIN_THREAD_ID;
			//	body["text"] = "";
			//	body["allThreadsStopped"] = true;
		}
	};
	struct ContinuedEvent : public Event {
		ContinuedEvent()
		{
			js["event"] = "continued";
			auto& body = js["body"];
			body["threadId"] = MAIN_THREAD_ID;
			body["allThreadsContinued"] = true;
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
			body["threadId"] = MAIN_THREAD_ID;
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
			js["arguments"] = Json::parse(InitializeRequestArguments().js.dump());
		}
	};
	struct InitializeResponse : public Response {
		InitializeResponse()
		{
			js["command"] = "initialize";
			js["body"] = Json::parse(Capabilities().js.dump());
		}
	};

	struct ConfigurationDoneRequest : public Request {
		ConfigurationDoneRequest()
		{
			js["command"] = "configurationDone";
			js["arguments"] = Json::parse(ConfigurationDoneArguments().js.dump());
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
			js["arguments"] = Json::parse(LaunchRequestArguments().js.dump());
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
			js["arguments"] = Json::parse(AttachRequestArguments().js.dump());
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
			js["arguments"] = Json::parse(RestartArguments().js.dump());
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
			js["arguments"] = Json::parse(DisconnectArguments().js.dump());
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
			js["arguments"] = Json::parse(SetBreakpointsArguments().js.dump());
		}
	};
	struct SetBreakpointsResponse : public Response {
		SetBreakpointsResponse()
		{
			js["command"] = "setBreakpoints";
			auto& body = js["body"];
			body["breakpoints"] = Json::parse(Breakpoint().js.dump());
		}
	};
	struct SetFunctionBreakpointsRequest : public Request {
		SetFunctionBreakpointsRequest()
		{
			js["command"] = "setFunctionBreakpoints";
			js["arguments"] = Json::parse(SetFunctionBreakpointsArguments().js.dump());
		}
	};
	struct SetFunctionBreakpointsResponse : public Response {
		SetFunctionBreakpointsResponse()
		{
			js["command"] = "setFunctionBreakpoints";
			auto& body = js["body"];
			body["breakpoints"] = Json::parse(Breakpoint().js.dump());
		}
	};
	struct SetExceptionBreakpointsRequest : public Request {
		SetExceptionBreakpointsRequest()
		{
			js["command"] = "setExceptionBreakpoints";
			js["arguments"] = Json::parse(SetExceptionBreakpointsArguments().js.dump());
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
			js["arguments"] = Json::parse(ContinueArguments().js.dump());
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
			js["arguments"] = Json::parse(NextArguments().js.dump());
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
			js["arguments"] = Json::parse(StepInArguments().js.dump());
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
			js["arguments"] = Json::parse(StepOutArguments().js.dump());
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
			js["arguments"] = Json::parse(StepBackArguments().js.dump());
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
			js["arguments"] = Json::parse(ReverseContinueArguments().js.dump());
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
			js["arguments"] = Json::parse(RestartFrameArguments().js.dump());
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
			js["arguments"] = Json::parse(GotoArguments().js.dump());
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
			js["arguments"] = Json::parse(PauseArguments().js.dump());
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
			js["arguments"] = Json::parse(StackTraceArguments().js.dump());
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
			js["arguments"] = Json::parse(ScopesArguments().js.dump());
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
			js["arguments"] = Json::parse(VariablesArguments().js.dump());
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
			js["arguments"] = Json::parse(SetVariableArguments().js.dump());
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
			js["arguments"] = Json::parse(SourceArguments().js.dump());
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
			body["threads"] = Json::object();
			// body["threads"] =
		}
	};
	struct ModulesRequest : public Request {
		ModulesRequest()
		{
			js["command"] = "modules";
			js["arguments"] = Json::parse(ModulesArguments().js.dump());
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
			js["arguments"] = Json::parse(LoadedSourcesArguments().js.dump());
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
			js["arguments"] = Json::parse(EvaluateArguments().js.dump());
		}
	};
	struct EvaluateResponse : public Response {
		EvaluateResponse()
		{
			js["command"] = "evaluate";
			auto& body = js["body"];
			body["result"] = "";
			body["type"] = "";
			body["presentationHint"] = Json::parse(VariablePresentationHint().js.dump());
			body["variablesReference"] = 0;
			body["namedVariables"] = 0;
			body["indexedVariables"] = 0;
		}
	};
	struct StepInTargetsRequest : public Request {
		StepInTargetsRequest()
		{
			js["command"] = "stepInTargets";
			js["arguments"] = Json::parse(StepInTargetsArguments().js.dump());
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
			js["arguments"] = Json::parse(GotoTargetsArguments().js.dump());
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
			js["arguments"] = Json::parse(CompletionsArguments().js.dump());
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
			js["arguments"] = Json::parse(ExceptionInfoArguments().js.dump());
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
			body["details"] = Json::parse(ExceptionDetails().js.dump());
		}
	};


}



Message::Message(std::string msg, int type)
{
	this->content = msg;
	this->id = g_MsgIdGen;
	this->type = type;
}

void Message::log()
{
	/*std::cout << "msg id:" << id << std::endl
	<< " type:" << type << std::endl
	<< " content:" << content << std::endl;*/
	std::cout << content << std::endl;
}

String Message::wrapMsg(String type, Json message)
{
	message["type"] = type;
	message["seq"] = g_MsgIdGen++;
	String msg = message.dump();
	/*std::cout << "readyToSend:" << std::endl
	<< msg << std::endl;*/

	int len = msg.length();
	String wrapped("");
	wrapped = wrapped + "Content-Length: " + std::to_string(len)
		+ "\r\n\r\n" + msg;
	return wrapped;
}


void MessageDispatcher::Dispatch(lua_State* L, Message _msg, MessageHandler* handler)
{
	//std::cout << "Dispatch : " << std::endl;
	//_msg.log();
	String msg = _msg.content;
	Json request = Json::parse(msg);
	auto type = request["type"];
	if (type == "request")
	{
		String command = request["command"];
		//std::cout << command << std::endl;
		/*************************************************************************
		rq init:
		"{"command":"initialize","arguments":{"clientID":"vscode","clientName":"Visual Studio Code","adapterID":"mock","pathFormat":"path","linesStartAt1":true,"columnsStartAt1":true,"supportsVariableType":true,"supportsVariablePaging":true,"supportsRunInTerminalRequest":true,"locale":"zh-cn"},"type":"request","seq":1}"

		init response
		"{"seq":1,"type":"response","request_seq":1,"command":"initialize","success":true,"body":{"supportsConfigurationDoneRequest":true,"supportsEvaluateForHovers":true,"supportsStepBack":true}}"

		init event
		"{"seq":2,"type":"event","event":"initialized"}"
		/************************************************************************/
		if (command == "initialize") {

			vs::InitializeResponse resp;
			resp.setRq(request);
			resp.js["body"] = {
				{ "supportsConfigurationDoneRequest",true },
				{ "supportsEvaluateForHovers",true },
				{ "supportsStepBack",true }
			};
			handler->SendResponse(resp.js);
			handler->SendEvent(vs::InitializedEvent().js);
		}
		else if (command == "launch") {
			Json args = request["arguments"];
			String program = args["program"];
			//TODO here should start the program
			/*std::thread startRuntime([]() {});*/



			String rqstr = request.dump();
			/*	vs::LaunchResponse rsp;
			rsp.setRq(Json::parse(rqstr));
			handler->SendResponse(rsp.js);*/
			/*handler->StopOnBreakPointEvent({});*/


			/*	vs::StoppedEvent stepEvent;
			stepEvent.js["event"] = "stopped";
			stepEvent.js["body"]["reason"] = "step";
			stepEvent.js["body"]["threadId"] = MAIN_THREAD_ID;
			handler->SendEvent(stepEvent.js);
			DebuggerCore::GetInstance()->SetBreaked(true);



			stepEvent;
			stepEvent.js["event"] = "stopped";
			stepEvent.js["body"]["reason"] = "resume";
			stepEvent.js["body"]["threadId"] = MAIN_THREAD_ID;
			handler->SendEvent(stepEvent.js);
			DebuggerCore::GetInstance()->SetBreaked(false);*/

			onConfidoneFunc = [handler, rqstr]()
			{
				vs::LaunchResponse rsp;
				rsp.setRq(Json::parse(rqstr));
				handler->SendResponse(rsp.js);


				vs::StoppedEvent stopEvent;
				stopEvent.js["event"] = "stopped";
				stopEvent.js["body"]["reason"] = "entry";
				stopEvent.js["body"]["threadId"] = MAIN_THREAD_ID;
				handler->SendEvent(stopEvent.js);






				//	handler->StopOnBreakPointEvent({});


				//	handler->SendEvent(pauseEvent.js);

				//DebuggerCore::GetInstance()->SetBreaked(false);
			};
		}
		else if (command == "attach") {
			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);
		}
		else if (command == "disconnect") {
			DebuggerCore::GetInstance()->SetBreaked(false);
		}
		else if (command == "restart") {
			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);
		}
		else if (command == "setBreakpoints") {
			/************************************************************************/
			/*
			1._runtime._clearBreakPoints
			2.verify bp
			3.sendResponse(bp)
			*/
			/************************************************************************/
			//server->doWrite(wrapMsg(newResponse,"reso"));
			Json args = request["arguments"];
			String path = args["source"]["path"];
			DebuggerCore::GetInstance()->SetSources(path, args["source"]);
			DebuggerCore::GetInstance()->SetAllBreakPoints(path, args["breakpoints"]);
			DebuggerCore::GetInstance()->VerifyAllBreakPoints();

			vs::SetBreakpointsResponse rsp;
			rsp.setRq(request);

			const Json& bps = DebuggerCore::GetInstance()->GetAllBreakPoints(path);
			rsp.js["body"] = { "breakpoints", bps };

			handler->SendResponse(rsp.js);
		}
		else if (command == "setFunctionBreakpoints") {
			/*	vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);*/
		}
		else if (command == "setExceptionBreakpoints") {
			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);
		}
		else if (command == "configurationDone") {

			vs::ConfigurationDoneResponse rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);

			if (onConfidoneFunc)
				onConfidoneFunc();

			handler->m_ConfigDone = true;
		}
		else if (command == "continue") {

			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);

			DebuggerCore::GetInstance()->SetBreaked(false);
		}
		else if (command == "next") {
			/*vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);*/
		}
		else if (command == "stepIn") {
			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);
		}
		else if (command == "stepOut") {
			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);
		}
		else if (command == "stepBack") {

		}
		else if (command == "reverseContinue") {

		}
		else if (command == "restartFrame") {

		}
		else if (command == "goto") {

		}
		else if (command == "pause") {
			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);

			vs::StoppedEvent stopEvent;
			stopEvent.js["body"]["reason"] = "pause";
			DebuggerCore::GetInstance()->SetBreaked(true);

			handler->SendEvent(stopEvent.js);

		}
		else if (command == "stackTrace") {

			vs::Response rsp;

			std::string str = R"#({"body":{"stackFrames":[{"column":0,"id":0,"line":%d,"name":"--(0)","source":{"adapterData":"mock-adapter-data","name":"main.lua","path":"D:\\Github\\SimpleEngine\\debugger\\scripts\\main.lua","sourceReference":0}},{"column":0,"id":1,"line":%d,"name":"local(1)","source":{"adapterData":"mock-adapter-data","name":"main.lua","path":"D:\\Github\\SimpleEngine\\debugger\\scripts\\main.lua","sourceReference":0}},{"column":0,"id":2,"line":%d,"name":"function(2)","source":{"adapterData":"mock-adapter-data","name":"main.lua","path":"D:\\Github\\SimpleEngine\\debugger\\scripts\\main.lua","sourceReference":0}},{"column":0,"id":3,"line":%d,"name":"debugger_loop(server)(3)","source":{"adapterData":"mock-adapter-data","name":"main.lua","path":"D:\\Github\\SimpleEngine\\debugger\\scripts\\main.lua","sourceReference":0}}],"totalFrames":4},"command":"stackTrace","request_seq":9,"seq":21,"success":true,"type":"response"})#";

			char cstr[1024];
			sprintf(cstr, str.c_str(), handler->CurrentLine, handler->CurrentLine, handler->CurrentLine, handler->CurrentLine);

			rsp.js = Json::parse( std::string(cstr));
			
			
			rsp.setRq(request);



			handler->SendResponse(rsp.js);
		}
		else if (command == "scopes") {
			vs::Response rsp;
			String str = R"#({"seq":23,"type":"response","request_seq":11,"command":"scopes","success":true,"body":{"scopes":[{"name":"Local","variablesReference":1000,"expensive":false},{"name":"Global","variablesReference":1001,"expensive":true}]}})#";

			rsp.js = Json::parse(str);
			rsp.setRq(request);

			handler->SendResponse(rsp.js);
		}
		else if (command == "variables") {
			vs::Response rsp;
			rsp.js = Json::parse(R"#({"seq":24,"type":"response","request_seq":12,"command":"variables","success":true,"body":{"variables":[{"name":"local_0_i","type":"integer","value":"123","variablesReference":0},{"name":"local_0_f","type":"float","value":"3.14","variablesReference":0},{"name":"local_0_s","type":"string","value":"hello world","variablesReference":0},{"name":"local_0_o","type":"object","value":"Object","variablesReference":1002}]}})#");
			rsp.setRq(request);

			handler->SendResponse(rsp.js);
		}
		else if (command == "setVariable") {

		}
		else if (command == "source") {

		}
		else if (command == "threads") {
			vs::ThreadsResponse rsp;
			rsp.setRq(request);
			Json threads_js = Json::array({
				{
					{ "id",MAIN_THREAD_ID },
					{ "name","thread 1" }
				}
			});
			rsp.js["body"]["threads"] = {
				threads_js
			};
			handler->SendResponse(rsp.js);
		}
		else if (command == "evaluate") {

		}
		else if (command == "stepInTargets") {

		}
		else if (command == "gotoTargets") {

		}
		else if (command == "completions") {

		}
		else if (command == "exceptionInfo") {

		}
		else if (command == "loadedSources") {
			vs::Response rsp;
			rsp.setRq(request);
			handler->SendResponse(rsp.js);
		}
		else {

		}

	}
	else if (type == "response")
	{

	}
}


MessageHandler::MessageHandler(DebugServer* server)
	:m_Server(server), m_ConfigDone(false), CurrentLine(1)
{
	m_Dispatcher = new MessageDispatcher();
}

MessageHandler::~MessageHandler()
{
	delete m_Dispatcher;
}

void MessageHandler::Loop(lua_State*L)
{
	if (!g_ReadQueue.empty() || !g_WriteQueue.empty())
	{
		if (!g_ReadQueue.empty())
		{
			g_ReadQueueMutex.lock();
			auto msg = g_ReadQueue.front();
			g_ReadQueue.pop_front();
			g_ReadQueueMutex.unlock();
			std::cout << "[g_ReadQueue Log!!!!!!!!!!!!!]" << std::endl;
			msg.log();
			std::cout << std::endl;
			m_Dispatcher->Dispatch(L, msg, this);
		}
		if (!g_WriteQueue.empty())
		{
			g_WriteQueueMutex.lock();
			auto msg = g_WriteQueue.front();
			g_WriteQueue.pop_front();
			g_WriteQueueMutex.unlock();
			std::cout << "[g_WriteQueue Log!!!!!!!!!!!!!]" << std::endl;
			msg.log();
			std::cout << std::endl;
			if (m_Server)
			{
				m_Server->Write(msg);
			}

		}
	}
}

void MessageHandler::Send(Message msg)
{
	g_WriteQueueMutex.lock();
	g_WriteQueue.push_back(msg);
	g_WriteQueueMutex.unlock();
}

void MessageHandler::StopOnBreakPointEvent(Json bp)
{
	vs::StoppedEvent ev;
	ev.js["event"] = "stopped";
	ev.js["body"] = {
		{ "reason","breakpoint" },
		{ "threadId",MAIN_THREAD_ID }
	};
	SendEvent(ev.js);
}

bool MessageHandler::SendEvent(Json& msgJson)
{
	Message msg("");
	msg.content = msg.wrapMsg("event", msgJson);
	Send(msg);
	return true;
}

bool MessageHandler::SendResponse(Json& msgJson)
{
	Message msg("");
	msg.content = msg.wrapMsg("response", msgJson);
	Send(msg);
	return true;
}

bool MessageHandler::SendRequest(Json& msgJson)
{
	Message msg("");
	msg.content = msg.wrapMsg("request", msgJson);
	Send(msg);
	return true;
}



DebugSession::DebugSession(int id, asio::ip::tcp::socket& socket)
{
	m_ID = id;
	m_Socket = new asio::ip::tcp::socket(std::move(socket));
	mReadBuffCurrentIndex = 0;
	mReadBuff.resize(4096);
	//m_Handler = new MessageHandler();
}

DebugSession::~DebugSession()
{
	//	delete m_Handler;
}

void DebugSession::DoRead()
{
	if (m_Socket->is_open())
	{
		asio::async_read(*m_Socket, asio::buffer(m_OneByte, 1),
			[this](std::error_code ec, std::size_t len)
		{
			if (!ec)
			{
				mReadBuff[mReadBuffCurrentIndex++] = m_OneByte[0];
				std::string s(mReadBuff.begin(), mReadBuff.begin() + mReadBuffCurrentIndex);
				std::regex regex_match_txt("Content-Length: ([0-9]+)\r\n\r\n");
				std::smatch base_match;
				if (std::regex_match(s, base_match, regex_match_txt))
				{
					if (base_match.size() == 2)
					{
						std::ssub_match lenstr = base_match[1];
						int len = std::stoi(lenstr.str());
						asio::async_read(*m_Socket, asio::buffer(m_MsgBuff, len),
							[this](std::error_code ec, std::size_t len)
						{
							if (!ec)
							{
								mReadBuffCurrentIndex = 0;
								for (int i = 0; i < len; i++)
								{
									mReadBuff[mReadBuffCurrentIndex++] = m_MsgBuff[i];
								}
								auto msgstr = std::string(mReadBuff.begin(), mReadBuff.begin() + mReadBuffCurrentIndex);
								mReadBuffCurrentIndex = 0;
								EnReadQueue(msgstr);
							}
							else
							{
								std::cout << ec.message() << std::endl;
							}
						});
					}
				}
				DoRead();
			}
			else
			{
				std::cout << ec.message() << std::endl;
				m_Socket->close();
				std::cout << "Socket is close" << std::endl;
			}

		});
	}
}

void DebugSession::DoReadRunable()
{
	DoRead();
}

void DebugSession::EnReadQueue(const std::string& msgstr)
{

	g_ReadQueueMutex.lock();
	g_ReadQueue.push_back(Message(msgstr));
	g_ReadQueueMutex.unlock();
}

void DebugSession::DoWriteRunable()
{

}

void DebugSession::Write(Message msg)
{
	//std::cout << " DebugSession::Write(Message msg)" << std::endl;
	//msg.log();
	if (m_Socket)
	{
		if (m_Socket->is_open()) {
			asio::async_write(*m_Socket,
				asio::buffer(msg.content.c_str(),
					msg.content.length()),
				[this](std::error_code ec, std::size_t /*length*/)
			{

			});
		}
	}

}

void DebugSession::Start()
{
	std::cout << "Session start!" << std::endl;
	//std::thread readThread(std::bind(&DebugSession::DoReadRunable, this));

	DoReadRunable();

	DoWriteRunable();

	//std::thread writeThread(std::bind(&DebugSession::DoWriteRunable, this));
}


DebugServer::DebugServer(int port)
{
	m_Port = port;
	m_Session = nullptr;
	m_SessionIdGen = 0;
	asio::ip::tcp::endpoint localhost(asio::ip::tcp::v4(), port);
	m_Acceptor = new asio::ip::tcp::acceptor(m_IOContext, localhost);
	//	m_Handler = new MessageHandler();
}

DebugServer::~DebugServer()
{
	delete m_Acceptor;
}

void DebugServer::RunSession()
{
	//	std::thread(std::bind(&DebugServer::RunLooper, this));
	//RunSession(socket);

	// auto& socket_ptr = m_Sockets[m_SessionIdGen];
	// DebugSession* session = new DebugSession(m_SessionIdGen, *socket_ptr);
	// m_Sockets.erase(m_SessionIdGen);

	// std::cout << " new connection:" << m_SessionIdGen << std::endl;

	// m_SessionIdGen++;

	// session->Start();
	//m_Sessions.insert(std::make_pair(m_SessionIdGen, session));		

	//	auto& sessionIter = m_Sessions.find(m_SessionIdGen);
	//	sessionIter->second->Start();


}

void DebugServer::Listen()
{
	std::cout << " server Listen! port:" << m_Port << std::endl;
	m_Acceptor->async_accept(
		[this](std::error_code ec, asio::ip::tcp::socket socket)
	{
		if (!ec)
		{
			m_Session = new DebugSession(m_SessionIdGen, socket);
			//	session_mutext.lock();
			// m_Sessions.insert(std::make_pair(m_SessionIdGen, session));
			//	session_mutext.unlock();
			m_Session->Start();
			//std::thread(std::bind(&DebugServer::RunSession, this));
		}
		else
		{
			std::cout << "m_Acceptor->async_accept error : " << ec.message() << std::endl;
		}

		//session->Start();
		//		Listen();		
	});

}

void DebugServer::Run()
{
	m_IOContext.run();
}

void DebugServer::Write(Message msg)
{
	//std::cout << "server send post" << std::endl;
	asio::post(m_IOContext,
		[this, msg]()
	{
		//std::cout << "server send" << std::endl;
		if (m_Session)
		{
			m_Session->Write(msg);
		}

	});
}


