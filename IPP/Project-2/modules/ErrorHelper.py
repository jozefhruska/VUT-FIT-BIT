# Modules
import sys

class ErrorHelper():
	errorTypes = {
		10: 'Invalid use of program arguments.',
		11: 'Unable to access the input files.',
		12: 'Unable to access the output files.',

		31: 'Invalid input XML file formatting.',
		32: 'Unexpected structure or lexical error in input XML file.',

		52: 'IPPCode19 semantic error.',
		53: 'Runtime - Wrong operand type.',
		54: 'Runtime - Acces to an undefined variable.',
		55: 'Runtime - Use of an undefined frame.',
		56: 'Runtime - Missing value.',
		57: 'Runtime - Wrong operand value.',
		58: 'Runtime - Unallowed string opration.',

		99: 'Internal error.'
	}

	def __init__(self):
		pass

	def warning(self, message):
		sys.stderr.write('Warning: ' + message + '\n')

	def fatal(self, code):
		if code in self.errorTypes:
			sys.stderr.write('Error: ' + self.errorTypes[code] + '\n')
			sys.exit(code)
		else:
			sys.stderr.write('Interpret error: Unknown error code!\n')
			sys.exit(code)

	def fatalWithMessage(self, code, message):
		if code in self.errorTypes:
			sys.stderr.write('Error: ' + message + '\n')
			sys.exit(code)
		else:
			sys.stderr.write('Interpret error: Unknown error code!\n')
			sys.exit(code)