from modules.ErrorHelper import ErrorHelper

class VariableType():
	INT = 1,
	BOOLEAN = 2,
	STRING = 3,
	TYPE = 4,
	NIL = 5

class FrameHelper(ErrorHelper):
	def __init__(self):
		self.GLOBAL = {}
		self.TEMPORARY = None
		self.STACK = []
		self.DATA = []

	def createFrame(self):
		self.TEMPORARY = {}

	def pushFrame(self):
		if self.TEMPORARY is not None:
			self.STACK.append(self.TEMPORARY)
			self.TEMPORARY = None
		else: self.fatalWithMessage(55, 'Attempting to push an undefined frame.')

	def popFrame(self):
		if self.STACK:
			self.TEMPORARY = self.STACK.pop()
		else: self.fatalWithMessage(55, 'Attempting to pop a frame from an empty stack.')

	def getFrame(self, location):
		if location == 'TF': return self.TEMPORARY
		elif location == 'LF':
			if len(self.STACK) > 0: return self.STACK[-1]
			else: return None
		else: return self.GLOBAL

	def processArgument(self, argument):
		frame, name = argument.text.split('@', 1)
		return (frame, name)

	def defineVariable(self, argument):
		frame, name = self.processArgument(argument)

		frame = self.getFrame(frame)
		if frame is not None:
			if name not in frame:
				frame.update({ name: { 'type': None, 'value': None } })
			else: self.fatalWithMessage(52, 'Variable already exists in this frame.')
		else: self.fatalWithMessage(55, 'Attempting to define a variable on undefined frame.')

	def setVariable(self, target, **kwargs):
		targetFrame, targetName = self.processArgument(target)

		if 'argument' in kwargs:
			metaType, metaValue = self.getVariable(kwargs['argument'])

			if metaValue is not None and metaType is not None:
				frame = self.getFrame(targetFrame)
				if frame is not None:
					if targetName in frame:
						frame.update({ targetName: { 'type': metaType, 'value': metaValue } })
					else: self.fatalWithMessage(54, 'Attempting to access a non-existing variable.')
				else: self.fatalWithMessage(55, 'Attempting to access a variable on undefined frame.')
		else:
			frame = self.getFrame(targetFrame)
			if frame is not None:
				if targetName in frame:
					frame.update({ targetName: { 'type': kwargs['value']['type'], 'value': kwargs['value']['value'] } })
				else: self.fatalWithMessage(54, 'Attempting to access a non-existing variable.')
			else: self.fatalWithMessage(55, 'Attempting to access a variable on undefined frame.')

	def getVariable(self, argument):
		type = None
		value = None

		if argument.type in [ 'int', 'bool', 'string', 'nil', 'type' ]:
			if argument.type == 'int':
				try:
					value = int(argument.text)
				except Exception as e:
					self.fatalWithMessage(32, str(e))
				finally:
					type = VariableType.INT
			elif argument.type == 'bool':
				if argument.text == 'true': value = True
				else: value = False

				type = VariableType.BOOLEAN
			elif argument.type == 'nil':
				value = 'nil'
				type = VariableType.NIL
			elif argument.type == 'type':
				value = VariableType.INT
				type = VariableType.TYPE
			elif argument.type == 'string':
				if argument.text != None:
					try:
						value = str(argument.text)
					except Exception as e:
						self.fatalWithMessage(32, str(e))
				else: value = ''

				type = VariableType.STRING
		else:
			frame, name = self.processArgument(argument)

			frame = self.getFrame(frame)
			if frame is not None:
				if name in frame:
					type = frame[name]['type']
					value = frame[name]['value']
				else: self.fatalWithMessage(54, 'Attempting to access a non-existing variable.')
			else: self.fatalWithMessage(55, 'Attempting to access a variable on undefined frame.')

		return (type, value)

	def dataStackPush(self, argument):
		type, value = self.getVariable(argument)
		self.DATA.append((type, value))

	def dataStackPop(self):
		if self.DATA:
			return self.DATA.pop()
		else: self.fatalWithMessage(56, '[\'FrameHelper\'] Attempting to pop an empty data stack.')

