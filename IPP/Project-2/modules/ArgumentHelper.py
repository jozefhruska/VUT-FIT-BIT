# Modules
import sys

from argparse import ArgumentParser
from argparse import RawTextHelpFormatter
from modules.ErrorHelper import ErrorHelper

class ArgumentParserWrapper(ArgumentParser, ErrorHelper):
	def error(self, message):
		self.fatal(10)

class ArgumentHelper(ArgumentParser, ErrorHelper):
	def __init__(self):
		# Check that arguments are used correctly
		if (('--help' in sys.argv or '-h' in sys.argv) and len(sys.argv) > 2):
			self.fatal(10)

		# Initialize ArgumentParserWrapper
		self.parser = ArgumentParserWrapper(
			description = 'IDS Project - Interpret\nAuthor: Jozef Hruska (xhrusk25)\nUsage: This script takes output of an IPPCode19 parser and interprets it.',
			formatter_class = RawTextHelpFormatter
		)

		# Add program arguments
		inputGroup = self.parser.add_argument_group('input')
		inputGroup.add_argument(
			'--source',
			help = 'Path to the XML source file.'
		)

		inputGroup.add_argument(
			'--input',
			help = 'Path to the inputs for the source interpretation.'
		)

		# Parse program arguments
		self.args = self.parser.parse_args()

		# Check that arguments are used correctly
		self.checkArgumentUsage()

	def checkArgumentUsage(self):
		if (self.args.source == None and self.args.input == None):
			self.fatal(10)