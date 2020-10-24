from modules.ErrorHelper import ErrorHelper
from modules.Argument import Argument, ArgumentType

class Instruction(ErrorHelper):
	def __init__(self, instruction):
		self.opcode = instruction.attrib['opcode'].upper()
		self.reqArgCount = 0
		self.reqArgTypes = []
		self.order = instruction.attrib['order'].upper()

		self.args = {}

	def process(self):
		# Convert order attrib to numeric value
		try: self.order = int(self.order)
		except Exception as e: self.fatalWithMessage(31, str(e))

		# Resolve instruction's opcode
		if self.opcode == 'MOVE':
			self.reqArgCount = 2
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB
			}
		elif self.opcode == 'DEFVAR':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR
			}
		elif self.opcode == 'CALL':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.LABEL
			}
		elif self.opcode == 'PUSHS':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.SYMB
			}
		elif self.opcode == 'POPS':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR
			}
		elif self.opcode == 'ADD':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'SUB':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'MUL':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'IDIV':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'LT':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'GT':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'EQ':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'AND':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'OR':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'NOT':
			self.reqArgCount = 2
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB
			}
		elif self.opcode == 'INT2CHAR':
			self.reqArgCount = 2
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB
			}
		elif self.opcode == 'STRI2INT':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'READ':
			self.reqArgCount = 2
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.TYPE
			}
		elif self.opcode == 'WRITE':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.SYMB
			}
		elif self.opcode == 'CONCAT':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'STRLEN':
			self.reqArgCount = 2
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB
			}
		elif self.opcode == 'GETCHAR':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'SETCHAR':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'TYPE':
			self.reqArgCount = 2
			self.reqArgTypes = {
				'arg1': ArgumentType.VAR,
				'arg2': ArgumentType.SYMB
			}
		elif self.opcode == 'LABEL':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.LABEL
			}
		elif self.opcode == 'JUMP':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.LABEL
			}
		elif self.opcode == 'JUMPIFEQ':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.LABEL,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'JUMPIFNEQ':
			self.reqArgCount = 3
			self.reqArgTypes = {
				'arg1': ArgumentType.LABEL,
				'arg2': ArgumentType.SYMB,
				'arg3': ArgumentType.SYMB
			}
		elif self.opcode == 'EXIT':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.SYMB
			}
		elif self.opcode == 'DPRINT':
			self.reqArgCount = 1
			self.reqArgTypes = {
				'arg1': ArgumentType.SYMB
			}
		elif self.opcode in [ 'CREATEFRAME', 'PUSHFRAME', 'POPFRAME', 'RETURN', 'BREAK' ]:
			# Opcodes with no arguments
			pass
		else: self.fatalWithMessage(32, 'Unknown instruction opcode \'%s\'.' % self.opcode)

	def addArgument(self, argument):
		argumentInstance = Argument(argument, self.reqArgTypes[argument.tag])
		if argumentInstance != None: self.args.update({ argument.tag: argumentInstance})

	def getOrder(self):
		return self.order