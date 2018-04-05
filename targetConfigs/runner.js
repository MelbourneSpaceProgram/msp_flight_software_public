importPackage(Packages.com.ti.debug.engine.scripting)
importPackage(Packages.com.ti.ccstudio.scripting.environment)
importPackage(Packages.java.lang)

// Must end with slash
var projectRoot = "/tmp/code/";
var logFilePath = "/tmp/code/out.txt";

var script = ScriptingEnvironment.instance();
script.setScriptTimeout(100 * 1000) // 100 seconds

script.traceSetConsoleLevel(TraceLevel.INFO)

// Valid arguments are either unittest or ditl
var test_type = 'unittest';
if(arguments.length != 0){
	test_type = arguments[0];
}

script.traceWrite("Running loader for type: " + test_type);
script.traceWrite("Starting MSP432")

var debugServer = script.getServer("DebugServer.1");

if(test_type === 'unittest'){
	debugServer.setConfig( projectRoot + "targetConfigs/auto_flatsat.ccxml")
} else if (test_type === 'ditl'){
	debugServer.setConfig( projectRoot + "targetConfigs/adcs_sim.ccxml")
}

var debugSession = debugServer.openSession(".*CORTEX.*");
debugSession.target.connect()
debugSession.memory.loadProgram(projectRoot + "TIRTOS Build/MSP.out")

var loaderExit = debugSession.symbol.getAddress("C$$EXIT");
var loaderExitBp = debugSession.breakpoint.add(loaderExit);

debugSession.beginCIOLogging(logFilePath);

debugSession.target.restart()
if(test_type === 'unittest'){
	script.traceWrite("Running synch")
	debugSession.target.run()
} else if (test_type === 'ditl'){
	script.traceWrite("Running asynch")
	debugSession.target.runAsynch();
	script.traceWrite("Disconnecting from MSP432");
	debugSession.target.disconnect();
	debugSession.endCIOLogging();

	debugSession.terminate();
	debugServer.stop();
}

nPC = debugSession.expression.evaluate("PC")

if (nPC == loaderExit) {
	script.traceSetConsoleLevel(TraceLevel.INFO)
	script.traceWrite("TEST FAILED!")
	script.traceEnd()
}

debugSession.target.disconnect();
debugSession.endCIOLogging();

debugSession.terminate();
debugServer.stop();
