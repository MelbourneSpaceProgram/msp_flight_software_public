import serial
def print_console(logo_path):
	with open(logo_path) as logo_file:
		for line in logo_file.readlines():
			print(line, end='')


print_console('logo.txt')
ser = serial.Serial('COM11', 115200)

print(ser.name)
ser.write(b'hello')

while(True):
    print(ser.readline())

