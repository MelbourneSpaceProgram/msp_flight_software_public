# MSP CS-1 Software
All code that will run on-board the satellite. The project is written in C++ and C, using the Code Composer Studio (CCS) compiler, provided by Texas Instruments for the MSP432.

### Quick Links
- [JAMA](https://melbournespaceprogram.jamacloud.com)
- [Jenkins](http://ci.spaceprogram.melbourne/) 
- [JIRA](jira.spaceprogram.melbourne) 
- [Slack](http://melbournespace.slack.com/) 

### Getting Started
The default branch for this project is satellite, not master. Please ensure the satellite branch is used as the source branch for all work, and that all work is eventually merged back here.

Check out the [Software Onboarding Guide](https://docs.google.com/presentation/d/1aTFj6O11hw7FWGM_q_fx4gNXZuZPpbjtE1QgLcxke5M/edit#slide=id.g2c5c7b9d93_0_81).

### Prerequisites
- [Code Composer Studio](http://www.ti.com/tool/CCSTUDIO)
- [SimpleLink MSP432 SDK (v1.3+)](http://www.ti.com/tool/SIMPLELINK-MSP432-SDK)
- [MSP432 Launchpad](http://www.ti.com/tool/MSP-EXP432P401R)

When using CCS, be sure to change the Build Configuration to TIRTOS Build. The project will not build otherwise and you will get weird, cryptic errors. You can do this by right clicking on the MSP project -> Build Configurations -> Set Active -> TIRTOS Build.

### Automatic builds
We also have a Jenkins server running. This server builds a copy of the code and runs the automated tests outlined above whenever new code is pushed. The server also runs some code quality tools such as CppCheck. CppCheck provides insights into unused functions/variables, and other possible improvements.

### Feature Documentation
All feature documentation and code samples can be found on [Jama](https://melbournespaceprogram.jamacloud.com) at Design Description > Software

 - [Debug Interface](https://melbournespaceprogram.jamacloud.com/perspective.req#/items/10728?projectId=45) 
 - [Lithium Driver](https://melbournespaceprogram.jamacloud.com/perspective.req#/items/10782?projectId=45)
 - [Antenna Deployment](https://melbournespaceprogram.jamacloud.com/perspective.req#/items/11243?projectId=45)
 - [I2C Driver](https://melbournespaceprogram.jamacloud.com/perspective.req#/items/11126?projectId=45)

# Contributing code

## Logging

Log types supported are: 
- `Log_printX()`
- `Log_infoX()`
- `Log_warningX()`
- `Log_errorX()`
, where X is a number from 0 to 5. This number specifies the number of values that need to be entered into the string. For example:

```
Log_info0("No parameters");
Log_info1("One parameter %d", 123);
Log_error2("The value of system %d was %f", system, value);
```

Make sure to include the logging library.
`#include <xdc/runtime/log.h>`

The logger function automatically places a newline (`\n`) at the end of each message, so there is no need to add one in the log message.
Log messages are capped at 161 characters in length, including any values that are entered into the message.

## Review process
Please ensure all code is created under a separate branch to satellite, to make testing and review easier. Squash non-meaningful commits (if unsure, don't worry about it, will be caught in review). Ensure the code builds locally, and also on the Jenkins build server. All automated tests must pass. When ready, please submit a pull request from the new branch onto satellite. The code will then be reviewed and merged when ready.

See [CS1-SW-1](https://melbournespaceprogram.jamacloud.com/perspective.req#/items/10696?projectId=45) for a diagram on the review process

## General Coding Guidelines
| Rule                                                                                           | Justification                                                                                                                                      |
|------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------|
| Prefer explicit data types (uint32_t, uint8_t) over implicit data types (int, char).           | Lowers chance of overflows due to not understanding data types. More explicit about how large a number is expected to be.                          |
| Prefer const int over #define's.                                                               | #define's do not have any type safety, whereas const int does. This allows the compiler to pick up more errors.                                    |
| No blocks of commented out code.                                                               | If code is not used, it should be deleted. Not doing so leads to clutter and decreased readability. Removed code can always be retrieved from git. |
| Always use { } on if and while statements.                                                     | Not using them can allow for easier mistakes later, when someone wants to add an extra line to the block.                                          |
| No printfs or inclusion of <stdio.h>                                                           | 4                                                                                                                                                  |
|  No use of malloc or related functions.                                                        |                                                                                                                                                    |
| High memory users should be allocated statically at satellite boot up, not left until runtime. |                                                                                                                                                    |
| [All top ten C rules](https://github.com/stanislaw/awesome-safety-critical/blob/master/Backup/P10.pdf)                                                                            |                                                                                                                                                    |

Use Python 3.4 for any utility programs, with style conforming to PEP8

A complete rule-set is the [JSF AV C++ Coding Standards](http://www.stroustrup.com/JSF-AV-rules.pdf)
Some other interesting resources are available on the [awesome-safety-critical](https://github.com/stanislaw/awesome-safety-critical) GitHub repo

## Code Style

### To automatically fix style
 1. Download a pre-built binary of LLVM from the LLVM website.
 2. Install LLVM. Select the "Add LLVM to my path" option. Make note of where LLVM is installed to.
There may be an error about a failed MSVC integration. This is acceptable.
 3. Open Code Composer Studio and install the CppStyle plugin. Follow the instructions for 'Install from update site'.
 4. Enable CppStyle as the C/C++ code formatter by going to Window -> Preferences -> C/C++ -> Code Style -> Formatter page and switch "Code Formatter" from [built-in] to "CppStyle (clang-format)".
 5. Configure CppStyle Window -> Preferences -> CppStyle. Set the Clang-format path to the location of LLVM, plus /bin/clang-format.exe. Example: C:\Program Files\LLVM\bin\clang-format.exe
 6. Save all windows.
 7. Use 'Command + Shift + f' on MacOS or 'Ctrl + Shift + f' on other systems to have the code formatted automatically.

### To check code style
 1. Install Python (3.x)
 2. From the root of the project folder (flight_software/) run 
 `python cpplint.py --recursive src` 
 or 
  `python3 cpplint.py --recursive src`
 3. This will produce a listing of all style errors in the project.
 4. If you are only working on a specific folder, change "src" to "src/[my_folder]", so you only see relevant issues.
 5. Fix up all style issues and then run the checker again to make sure nothing was missed.

### Exceptions to style guide
 - We do not require a copyright header at the start of each file.
 - Indentations are 4 spaces, not 2

### Style Sample
``` 
/*
 * A sample source file for the code formatter preview
 * /
#include <math.h>

class Point {
  public:
    Point(double x, double y) : x(x), y(y) {}
	double distance(const Point& other) const;

	double x;
	double y;
};

double Point::distance(const Point& other) const {
  double dx = x - other.x;
  double dy = y - other.y;
  return sqrt(dx * dx + dy * dy)
}
```

## Unit Testing
### Set Up
1. Install [Ruby](https://www.ruby-lang.org/en/documentation/installation/). Make sure Ruby is added to the system PATH (will be asked as part of the installation).
2. Create a folder called 'tests' inside the folder with the code to test. The folder must have this name.
There can be multiple `tests` folders. Each functional folder can have its own `tests` folder within it.

### Write Tests
1. Inside the tests folder create a new file called [name_of_functionality]_tests.cpp
2. Add an import in the newly created cpp file for the following:
	1. #include <test_runners/[name_of_functionality]_tests.h>. This file won't exist yet, as it is automatically generated later.
	2. #include <test_runners/unity.h>
	3. Any other includes needed to run the code.
3. Define all unit tests, the **method must start with test, Test, spec, Spec, should, Should**, all available assertions can be found [here](https://github.com/ThrowTheSwitch/Unity)
```
void TestTestMessageSerialise(void) {
   TestMessage test_message(0x10);
   SerialisedMessage serial_message = test_message.Serialise();

   TEST_ASSERT_EQUAL_INT16(3, test_message.GetSerialisedSize());
   TEST_ASSERT_EQUAL_INT8(kMockTestSensor, serial_message.GetBuffer()[0]);
   TEST_ASSERT_EQUAL_INT8(kCurrentVersion, serial_message.GetBuffer()[1]);
   TEST_ASSERT_EQUAL_INT8(0x03, serial_message.GetBuffer()[2]);
}
```
4. Multiple unit tests can (and should) be defined in the one file. Make sure each test follows the format of a function with no/void args `NameOfTest(void)`

### Generate code to compile and run tests
1. Open up Python 3.4+ and move to the top project folder (most likely flight_software).
2. Run `python generate_tests.py` / `python3 generate_tests.py`

### Run Tests
1. Some code will be generated in the test_runners/ folder. It will be compiled and run on the LaunchPad.
2. Run the code as usual on the LaunchPad. The output (pass/fail) of each test will be printed in the CCS console.

### Ignoring Tests
It is possible to ignore tests using `TEST_IGNORE()` and `TEST_IGNORE_MESSAGE("some message")` . This is useful for enabling and disabling hardware dependent tests for FlatSat testing.
To ignore a Unity test, add the following to the beginning of the test method in [name_of_functionality]_tests.cpp:

```
if(![name_of_functionality]_test_enabled){
    TEST_IGNORE_MESSAGE("some informative message")
}
```
See `src/config/unit_tests.h` for the creation of the definition of this variable.
