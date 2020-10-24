import re

from modules.ErrorHelper import ErrorHelper

class ArgumentType():
	VAR = 1,
	SYMB = 2,
	LABEL = 3,
	TYPE = 4

class Argument(ErrorHelper):
	def __init__(self, argument, reqType):
		self.code = argument.tag
		self.type = argument.attrib['type']
		self.text = argument.text
		self.reqType = reqType

		self.checkArgumentType()

	def checkArgumentType(self, reqType = None):
		if reqType == None: reqType = self.reqType

		if reqType == ArgumentType.VAR:
			if self.type != 'var':
				self.fatalWithMessage(32, 'Invalid attribute \'type\'.')
			else:
				if self.text == None or not re.match('^(GF|LF|TF)@(_|-|\$|&|%|\*|[a-zA-Z])(_|-|\$|&|%|\*|[a-zA-Z0-9])*$', self.text):
					self.fatalWithMessage(32, 'Invalid argument value \'%s\' for type \'%s\'.' % (self.text, self.type))
				else: return

		elif reqType == ArgumentType.SYMB:
			if self.type == 'int':
				if self.text == None or not re.match('^([+-]?[1-9][0-9]*|[+-]?[0-9])$', self.text):
					self.fatalWithMessage(32, 'Invalid argument value \'%s\' for type \'%s\'.' % (self.text, self.type))
				else: return
			elif self.type == 'bool':
				if self.text == None or not self.text in ['true', 'false']:
					self.fatalWithMessage(32, 'Invalid argument value \'%s\' for type \'%s\'.' % (self.text, self.type))
				else: return
			elif self.type == 'string':
				if self.text == None:
					self.text == ''
				elif not re.search('^(\\\\[0-9]{3}|[^\s\\\\#])*$', self.text):
					self.fatalWithMessage(32, 'Invalid argument value \'%s\' for type \'%s\'.' % (self.text, self.type))
				else: self.text = re.sub(r'\\([0-9]{3})', lambda x: chr(int(x.group(1))), self.text)
			elif self.type == 'nil':
				if self.text == None or self.text != 'nil':
					self.fatalWithMessage(32, 'Invalid argument value \'%s\' for type \'%s\'.' % (self.text, self.type))
				else: return
			elif self.type == 'var': self.checkArgumentType(ArgumentType.VAR)
			else: self.fatalWithMessage(32, 'Invalid attribute \'type\'.')

		elif reqType == ArgumentType.LABEL:
			if self.type != 'label':
				self.fatalWithMessage(32, 'Invalid attribute \'type\'.')
			else:
				if self.text == None or not re.match('^(_|-|\$|&|%|\*|[a-zA-Z])(_|-|\$|&|%|\*|[a-zA-Z0-9])*$', self.text):
					self.fatalWithMessage(32, 'Invalid argument value \'%s\' for type \'%s\'.' % (self.text, self.type))
				else: return

		elif reqType == ArgumentType.TYPE:
			if self.type != 'type':
				self.fatalWithMessage(32, 'Invalid attribute \'type\'.')
			else:
				if self.text == None or not re.match('^(int|bool|string)$', self.text):
					self.fatalWithMessage(32, 'Invalid argument value \'%s\' for type \'%s\'.' % (self.text, self.type))
				else: return