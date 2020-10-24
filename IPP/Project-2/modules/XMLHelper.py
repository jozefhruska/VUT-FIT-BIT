import sys
import xml.etree.ElementTree as ET

from modules.ErrorHelper import ErrorHelper
from modules.Instruction import Instruction

class XMLHelper(ErrorHelper):
	def __init__(self):
		self.sourceXMLPath = None
		self.sourceXMLTree = None
		self.sourceXMLRaw = ''

	def parseXML(self, sourceXMLPath, instructionStack):
		self.sourceXMLPath = sourceXMLPath
		self.instructionStack = instructionStack

		# Parse raw source data to the XML tree format
		self.parseXMLTree()

		# Check source XML syntax rules
		self.checkXMLValidity()

	def parseXMLTree(self):
		try:
			if self.sourceXMLPath == None:
				for line in sys.stdin:
					self.sourceXMLRaw += line.strip()
			else:
				with open(self.sourceXMLPath) as f:
					for line in f:
						self.sourceXMLRaw += line.strip()
		except:
			self.fatal(11)

		try:
			self.sourceXMLTree = ET.fromstring(self.sourceXMLRaw)
		except:
			self.fatal(31)

	def checkXMLValidity(self):
		# Root element has to be called 'program'
		if self.sourceXMLTree.tag != 'program':
			self.fatalWithMessage(32, 'Root element has to be called \'program\'.')

		# Check validity of root attributes
		if 'language' in self.sourceXMLTree.attrib:
			for attribute in self.sourceXMLTree.attrib:
				if attribute not in ['language', 'name', 'description']:
					self.fatalWithMessage(32, 'Invalid argument \'%s\'.' % attribute)

				if attribute == 'language':
					if self.sourceXMLTree.attrib[str(attribute)].lower() != 'ippcode19':
						self.fatalWithMessage(33, 'Unknown language \'%s\'.' % self.sourceXMLTree.attrib[str(attribute)])
		else: self.fatalWithMessage(33, 'Language not defined.')

		# Check validity of instructions and process them if valid
		for instruction in self.sourceXMLTree:
			if self.checkInstructionValidity(instruction):
				self.processInstruction(instruction)

		# Sort instruction stack after the last instruction is parsed
		self.instructionStack.sortStack()


	def checkInstructionValidity(self, instruction):
		# Check instruction tag
		if instruction.tag != 'instruction':
			self.fatalWithMessage(32, 'Invalid tag \'%s\'.' % instruction.tag)

		# Check instruction arguments
		if len(instruction.attrib) == 2:
			if 'opcode' in instruction.attrib and 'order' in instruction.attrib: return True
			else: self.fatalWithMessage(32, 'Instruction argument \'opcode\' or \'order\' missing.')
		else: self.fatalWithMessage(32, 'Invalid number of attributes for an instruction tag.')

	def checkArgumentValidity(self, argument):
		if argument.tag in [ 'arg1', 'arg2', 'arg3' ]:
			if len(argument.attrib) == 1:
				if 'type' in argument.attrib:
					if argument.attrib['type'] in [ 'int', 'bool', 'string', 'nil', 'label', 'type', 'var' ]:
						if len(argument) == 0:
							return True
						else: self.fatalWithMessage(32, 'Argument tag cannot have child nodes.')
					else: self.fatalWithMessage(32, 'Invalid argument type \'%s\'.' % argument.attrib['type'])
				else: self.fatalWithMessage(32, 'Argument attribute \'type\' missing.')
			else: self.fatalWithMessage(32, 'Invalid number of attributes for an argument tag.')
		else: self.fatalWithMessage(32, 'Invalid argument tag \'%s\'.' % argument.tag)

	def processInstruction(self, instruction):
		instrInstance = Instruction(instruction)
		instrInstance.process()

		# Check if these is the right number of arguments
		if instrInstance.reqArgCount != len(instruction):
			self.fatalWithMessage(32, 'Invalid number of arguments in instruction \'%s\'.' % instrInstance.opcode)

		# Check validity of arguments and process them if are valid
		for argument in instruction:
			if self.checkArgumentValidity(argument):
				if argument.tag not in instrInstance.args:
					instrInstance.addArgument(argument)
				else: self.fatalWithMessage(32, 'Duplicate argument tags in instruction.')

		# Store instruction to the instuction stack
		self.instructionStack.insertInstruction(instrInstance)