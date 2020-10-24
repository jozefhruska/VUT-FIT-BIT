from modules.ErrorHelper import ErrorHelper

class InstructionStack(ErrorHelper):
	def __init__(self):
		self.stack = {}
		self.sortedKeys = None
		self.position = 0
		self.labels = {}
		self.callStack = []

	def insertInstruction(self, instruction):
		if instruction.order not in self.stack:
			self.stack.update({ instruction.order: instruction })
		else: self.fatalWithMessage(32, 'Invalid or duplicate order attribute value \'%s\'.' % str(instruction.order))

		if instruction.opcode == 'LABEL':
			if instruction.args['arg1'].text not in self.labels:
				self.labels.update({ instruction.args['arg1'].text: instruction.order })
			else: self.fatalWithMessage(52, 'Duplicate label names.')

	def getNextInstruction(self):
		if self.position < len(self.stack):
			self.position += 1
			return self.stack[self.position]
		else: return False

	def sortStack(self):
		self.sortedKeys = sorted(self.stack)

	def getStack(self):
		return self.stack

	def call(self):
		self.callStack.append(self.position)

	def ret(self):
		if self.callStack:
			self.position = self.callStack.pop()
		else: self.fatalWithMessage(56, '[\'InstructionStack\', #{}] Value in the call stack is missing.'.format(self.position))


	def jumpToLabel(self, argument):
		labelName = argument.text

		if labelName in self.labels:
			self.position = self.labels[labelName]
		else: self.fatalWithMessage(52, 'Attempting to jump on undefined label.')