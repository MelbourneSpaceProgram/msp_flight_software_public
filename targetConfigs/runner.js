importPackage(Packages.com.ti.debug.engine.scripting)
importPackage(Packages.com.ti.ccstudio.scripting.environment)
importPackage(Packages.java.lang)


// Must end with slash
var projectRoot = "/tmp/code/";
var logFilePath = "/tmp/code/out.txt";

var script = ScriptingEnvironment.instance();
script.setScriptTimeout(100 * 1000) // 100 seconds

script.traceSetConsoleLevel(TraceLevel.INFO)

script.traceWrite("Starting MSP432")

var debugServer = script.getServer("DebugServer.1");
debugServer.setConfig( projectRoot + "targetConfigs/auto_flatsat.ccxml")

var debugSession = debugServer.openSession(".*CORTEX.*");
debugSession.target.connect()
debugSession.memory.loadProgram(projectRoot + "TIRTOS Build/MSP.out")

var loaderExit = debugSession.symbol.getAddress("C$$EXIT");
var loaderExitBp = debugSession.breakpoint.add(loaderExit);

debugSession.beginCIOLogging(logFilePath);

debugSession.target.restart()
debugSession.target.run()

nPC = debugSession.expression.evaluate("PC")

if (nPC == loaderExit) {
	script.traceSetConsoleLevel(TraceLevel.INFO)
	script.traceWrite("TEST FAILED!")
	script.traceEnd()
}

debugSession.target.halt();
debugSession.target.disconnect();
debugSession.endCIOLogging();

debugSession.terminate();
debugServer.stop();
