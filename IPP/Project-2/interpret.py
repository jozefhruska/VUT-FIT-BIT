# Modules
import sys

from modules.ErrorHelper import ErrorHelper
from modules.ArgumentHelper import ArgumentHelper
from modules.XMLHelper import XMLHelper
from modules.FrameHelper import FrameHelper, VariableType
from modules.InstructionStack import InstructionStack

def Interpret():
	# 'Global' helper objects
	errorHelper = ErrorHelper()
	argumentHelper = ArgumentHelper()
	xmlHelper = XMLHelper()
	frameHelper = FrameHelper()
	instructionStack = InstructionStack()

	# Input
	inputFile = None
	if argumentHelper.args.input:
		try: inputFile = open(argumentHelper.args.input)
		except Exception as e:
			errorHelper.fatalWithMessage(11, str(e))

	# Open source files and parse the XML
	xmlHelper.parseXML(argumentHelper.args.source, instructionStack)

	# Prepare the instruction stack
	currentInstruction = instructionStack.getNextInstruction()
	while currentInstruction != False:

		# Frame operation & function call instructions
		if currentInstruction.opcode == 'MOVE':
			frameHelper.setVariable(
				currentInstruction.args['arg1'],
				argument = currentInstruction.args['arg2']
			)
		elif currentInstruction.opcode == 'CREATEFRAME':
			frameHelper.createFrame()
		elif currentInstruction.opcode == 'PUSHFRAME':
			frameHelper.pushFrame()
		elif currentInstruction.opcode == 'POPFRAME':
			frameHelper.popFrame()
		elif currentInstruction.opcode == 'DEFVAR':
			frameHelper.defineVariable(currentInstruction.args['arg1'])
		elif currentInstruction.opcode == 'CALL':
			instructionStack.call()
			instructionStack.jumpToLabel(currentInstruction.args['arg1'])
		elif currentInstruction.opcode == 'RETURN':
			instructionStack.ret()

		# Data stack instructions
		elif currentInstruction.opcode == 'PUSHS':
			frameHelper.dataStackPush(currentInstruction.args['arg1'])
		elif currentInstruction.opcode == 'POPS':
			type, value = frameHelper.dataStackPop();

			frameHelper.setVariable(
				currentInstruction.args['arg1'],
				value = {
					'type': type,
					'value': value
				}
			)

		# Arithmetic instructions
		elif currentInstruction.opcode == 'ADD':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == VariableType.INT and op1type == op2type:
				frameHelper.setVariable(
					currentInstruction.args['arg1'],
					value = {
						'type': VariableType.INT,
						'value': op1value + op2value
					}
				)
			else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
		elif currentInstruction.opcode == 'SUB':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == VariableType.INT and op1type == op2type:
				frameHelper.setVariable(
					currentInstruction.args['arg1'],
					value = {
						'type': VariableType.INT,
						'value': op1value - op2value
					}
				)
			else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
		elif currentInstruction.opcode == 'MUL':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == VariableType.INT and op1type == op2type:
				frameHelper.setVariable(
					currentInstruction.args['arg1'],
					value = {
						'type': VariableType.INT,
						'value': op1value * op2value
					}
				)
			else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
		elif currentInstruction.opcode == 'IDIV':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == VariableType.INT and op1type == op2type:
				if op2value is not 0:
					frameHelper.setVariable(
						currentInstruction.args['arg1'],
						value = {
							'type': VariableType.INT,
							'value': op1value // op2value
						}
					)
				else: errorHelper.fatalWithMessage(57, 'Zero division.')
			else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
		elif currentInstruction.opcode in [ 'LT', 'GT', 'EQ' ]:
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])
			result = False

			if currentInstruction.opcode == 'LT':
				if op1type in [ VariableType.INT, VariableType.BOOLEAN, VariableType.STRING ] and op1type == op2type:
					if op1value < op2value: result = True
				else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
			elif currentInstruction.opcode == 'GT':
				if op1type in [ VariableType.INT, VariableType.BOOLEAN, VariableType.STRING ] and op1type == op2type:
					if op1value > op2value: result = True
				else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
			elif currentInstruction.opcode == 'EQ':
				if (op1type in [ VariableType.INT, VariableType.BOOLEAN, VariableType.STRING, VariableType.NIL ]
				and op2type in [ VariableType.INT, VariableType.BOOLEAN, VariableType.STRING, VariableType.NIL ]):
					if op1value == op2value: result = True
				else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')

				frameHelper.setVariable(
					currentInstruction.args['arg1'],
					value = {
						'type': VariableType.BOOLEAN,
						'value': result
					}
				)
		elif currentInstruction.opcode in [ 'AND', 'OR', 'NOT' ]:
			result = False

			if currentInstruction.opcode == 'AND':
				op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
				op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

				if op1type is op2type is VariableType.BOOLEAN:
					result = op1value and op2value
				else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
			elif currentInstruction.opcode == 'OR':
				op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
				op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

				if op1type is op2type is VariableType.BOOLEAN:
					result = op1value or op2value
				else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
			elif currentInstruction.opcode == 'NOT':
				op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])

				if op1type is VariableType.BOOLEAN:
					result = not op1value
				else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')

			frameHelper.setVariable(
				currentInstruction.args['arg1'],
				value = {
					'type': VariableType.BOOLEAN,
					'value': result
				}
			)
		elif currentInstruction.opcode == 'INT2CHAR':
			type, value = frameHelper.getVariable(currentInstruction.args['arg2'])
			convertedValue = None

			if type is VariableType.INT:
				try:
					convertedValue = chr(int(value))
				except Exception as e:
					errorHelper.fatalWithMessage(58, 'Invalid char conversion via \'INT2CHAR\'.')

				if convertedValue is not None:
					frameHelper.setVariable(
						currentInstruction.args['arg1'],
						value = {
							'type': VariableType.STRING,
							'value': value
						}
					)
			else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
		elif currentInstruction.opcode == 'STRI2INT':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])
			result = None

			if op1type is VariableType.STRING and op2type is VariableType.INT:
				if op2value >= 0 and op2value <= len(op1value) - 1:
					try:
						result = ord(op1value[op2value])
					except Exception as e:
						errorHelper.fatalWithMessage(58, str(e))

					if result is not None:
						frameHelper.setVariable(
							currentInstruction.args['arg1'],
							value = {
								'type': VariableType.STRING,
								'value': result
							}
						)
				else: errorHelper.fatalWithMessage(58, 'Invalid index using \'INT2CHAR\'.')
			else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')

		# I/O instructions
		elif currentInstruction.opcode == 'READ':
			type, value = frameHelper.getVariable(currentInstruction.args['arg2'])
			stdInput = ''

			if argumentHelper.args.input:
				stdInput = inputFile.readline()
			else:
				try:
					stdInput = input()
				except Exception:
					pass

			if value == 'int':
				try:
					stdInput = int(stdInput)
				except Exception as e:
					errorHelper.fatalWithMessage(57, '[\'READ\', #{}] {}'.format(instructionStack.position, str(e)))
			elif value == 'bool':
				if stdInput.lower() == 'true': stdInput = True
				else: stdInput = False
			elif value == 'string':
				try:
					stdInput = str(stdInput)
				except Exception as e:
					errorHelper.fatalWithMessage(57, '[\'READ\', #{}] {}'.format(instructionStack.position, str(e)))

			frameHelper.setVariable(
				currentInstruction.args['arg1'],
				value = {
					'type': type,
					'value': stdInput
				}
			)

		elif currentInstruction.opcode == 'WRITE':
			type, value = frameHelper.getVariable(currentInstruction.args['arg1'])

			if type is not None and value is not None:
				print(value, end='')
			else: errorHelper.fatalWithMessage(56, 'Attempting to print a variable with no value.')

		# String instructions
		elif currentInstruction.opcode == 'CONCAT':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == op2type == VariableType.STRING:
				frameHelper.setVariable(
					currentInstruction.args['arg1'],
					value = {
						'type': VariableType.STRING,
						'value': op1value + op2value
					}
				)
			else: errorHelper.fatalWithMessage(53, '[\'CONCAT\', #{}] Invalid argument type. (String required)'.format(instructionStack.position))
		elif currentInstruction.opcode == 'STRLEN':
			type, value = frameHelper.getVariable(currentInstruction.args['arg2'])

			if type == VariableType.STRING:
				frameHelper.setVariable(
					currentInstruction.args['arg1'],
					value = {
						'type': VariableType.INT,
						'value': len(value)
					}
				)
			else: errorHelper.fatalWithMessage(53, '[\'STRLEN\', #{}] Invalid argument type. (String required)'.format(instructionStack.position))
		elif currentInstruction.opcode == 'GETCHAR':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == VariableType.STRING and op2type == VariableType.INT:
				if op2value >= 0 and op2value <= len(op1value) - 1:
					frameHelper.setVariable(
						currentInstruction.args['arg1'],
						value = {
							'type': VariableType.STRING,
							'value': op1value[op2value]
						}
					)
				else: errorHelper.fatalWithMessage(58, '[\'GETCHAR\', #{}] Invalid index \'{}\'.'.format(instructionStack.position, op2value))
			else: errorHelper.fatalWithMessage(53, '[\'GETCHAR\', #{}] Invalid argument type. (String and integer required)'.format(instructionStack.position))
		elif currentInstruction.opcode == 'SETCHAR':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg1'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op3type, op3value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == VariableType.STRING and op2type == VariableType.INT and op3type == VariableType.STRING:
				if op2value >= 0 and op2value <= len(op1value) - 1:
					if op3value != '':
						result = list(op1value)
						result[op2value] = op3value[0]
						result = ''.join(result)

						frameHelper.setVariable(
							currentInstruction.args['arg1'],
							value = {
								'type': VariableType.STRING,
								'value': result
							}
						)
					else: errorHelper.fatalWithMessage(58, '[\'SETCHAR\', #{}] Last argument cannot be an empty string.'.format(instructionStack.position))
				else: errorHelper.fatalWithMessage(58, '[\'SETCHAR\', #{}] Invalid index \'{}\'.'.format(instructionStack.position, op2value))
			else: errorHelper.fatalWithMessage(53, '[\'SETCHAR\', #{}] Invalid argument type. (Integer and string required)'.format(instructionStack.position))

		# Type instruction
		elif currentInstruction.opcode == 'TYPE':
			type, value = frameHelper.getVariable(currentInstruction.args['arg2'])
			resultType = ''

			if type == VariableType.INT: resultType = 'int'
			elif type == VariableType.BOOLEAN: resultType = 'bool'
			elif type == VariableType.STRING: resultType = 'string'
			elif type == VariableType.NIL: resultType = 'nil'

			frameHelper.setVariable(
				currentInstruction.args['arg1'],
				value = {
					'type': VariableType.STRING,
					'value': resultType
				}
			)

		# Program flow instructions
		elif currentInstruction.opcode == 'LABEL':
			pass
		elif currentInstruction.opcode == 'JUMP':
			instructionStack.jumpToLabel(currentInstruction.args['arg1'])
		elif currentInstruction.opcode in [ 'JUMPIFEQ', 'JUMPIFNEQ' ]:
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg2'])
			op2type, op2value = frameHelper.getVariable(currentInstruction.args['arg3'])

			if op1type == op2type:
				if currentInstruction.opcode == 'JUMPIFEQ':
					if op1value == op2value:
						instructionStack.jumpToLabel(currentInstruction.args['arg1'])
				elif currentInstruction.opcode == 'JUMPIFNEQ':
					if op1value != op2value:
						instructionStack.jumpToLabel(currentInstruction.args['arg1'])
			else: errorHelper.fatalWithMessage(53, 'Invalid operand types.')
		elif currentInstruction.opcode == 'EXIT':
			op1type, op1value = frameHelper.getVariable(currentInstruction.args['arg1'])

			if op1type == VariableType.INT and (op1value >= 0 and op1value <= 49):
				sys.exit(op1value)
			else: errorHelper.fatalWithMessage(57, '[\'EXIT\', #{}] Invalid argument value \'{}\'.'.format(instructionStack.position, op1value))

		# Debug instructions
		elif currentInstruction.opcode == 'DPRINT':
			type, value = frameHelper.getVariable(currentInstruction.args['arg1'])

			if type != None and value != None:
				strValue = None

				try:
					strValue = str(value)
				except Exception as e:
					errorHelper.fatalWithMessage(57, '[\'DPRINT\', #{}] {}'.format(instructionStack.position, str(e)))

				if strValue != None:
					sys.stderr.write(strValue)
			else: errorHelper.fatalWithMessage(56, '[\'DPRINT\', #{}] Use of an undefined variable.'.format(instructionStack.position))
		elif currentInstruction.opcode == 'BREAK':
			sys.stderr.write('\nDEBUG:\n')
			sys.stderr.write('---------------------------\n')
			sys.stderr.write('Current position: {} \n'.format(instructionStack.position))
			sys.stderr.write('---------------------------\n')
			sys.stderr.write('Global frame: {} \n'.format(frameHelper.GLOBAL))
			sys.stderr.write('Local frame stack: {} \n'.format(frameHelper.STACK))
			sys.stderr.write('Temporary frame: {} \n'.format(frameHelper.TEMPORARY))
			sys.stderr.write('---------------------------\n')

		currentInstruction = instructionStack.getNextInstruction()

if __name__ == '__main__':
	Interpret()