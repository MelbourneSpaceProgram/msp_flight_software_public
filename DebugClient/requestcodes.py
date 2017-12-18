### Code definitions ###


VERSION = 0
TEMPERATURE = 0
RADIATION = 1
TEST = 3

"""
Parses bytes data (minus header information) to retrieve temperature

:param rawData: Raw bytes data minus version and code 
:returns: Formatted string for writing to console or log
"""


def parseTemperature(rawData):
    rawTemperature = rawData[0:4]  # TODO: Convert to 32 bit float
    rawSensorId = rawData[4]
    rawTimeStamp = rawData[5]
    return "Temp: {}, ID: {}, Time: {}".format(rawTemperature, rawSensorId, rawTimeStamp)


"""
Parses bytes data (minus header information) to retrieve radiation reading

:param rawData: Raw bytes data minus version and code 
:returns: Formatted string for writing to console or log
"""


def parseRadiation(rawData):
    # TODO: Implement
    return "This function is yet to be implemented"


"""
Parses bytes data (minus header information) to return test byte as an unsigned integer

:param rawData: Raw bytes data minus version and code 
:returns: Formatted string for writing to console or log
"""


def parseTest(rawData):
    return "Test: {}".format(rawData[0])


### Function Dictionary ###


dispatch = {
    TEMPERATURE: parseTemperature,
    RADIATION: parseRadiation,
    TEST: parseTest,
}
