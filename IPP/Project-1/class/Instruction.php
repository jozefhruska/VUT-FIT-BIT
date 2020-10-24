<?php
	/**
	 * Class that holds meta information about a specific instruction.
	 */
	class Instruction extends Element {
		private $name;
		private $type;
		private $length;

		private $argumentTypes = array();
		private $arguments = array();
		private $argumentsCount = 0;

		function __construct($raw) {
			/* Strip raw instruction line of whitespaces */
			$raw = trim($raw);
			$raw = preg_replace('/\s/', ' ', $raw);

			/* Split instruction by words */
			$rawSplit = explode(' ', $raw);

			/* Resolve raw instruction */
			$this->resolve($rawSplit);
		}

		/**
		 * Decides what type of an instruction it recieved.
		 * @param string $instructionName Raw instruction name
		 */
		private function getInstructionMeta($instructionName) {
			/* Force uppercase characters in instruction name */
			$instructionName = strtoupper($instructionName);
			$this->name = $instructionName;

			switch ($instructionName) {

				/* Frames, function calls
				============================================================================= */
				case 'MOVE':
					$this->type = InstructionType::MOVE;
					$this->argumentsCount = 2;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL
					];
					break;
				case 'CREATEFRAME':
					$this->type = InstructionType::CREATEFRAME;
					$this->argumentTypes = null;
					break;
				case 'PUSHFRAME':
					$this->type = InstructionType::PUSHFRAME;
					$this->argumentTypes = null;
					break;
				case 'POPFRAME':
					$this->type = InstructionType::POPFRAME;
					$this->argumentTypes = null;
					break;
				case 'DEFVAR':
					$this->type = InstructionType::DEFVAR;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::VARIABLE ];
					break;
				case 'CALL':
					$this->type = InstructionType::CALL;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::LABEL ];
					break;
				case 'RETURN':
					$this->type = InstructionType::RETURN;
					$this->argumentTypes = null;
					break;

				/* Data stack
				============================================================================= */
				case 'PUSHS':
					$this->type = InstructionType::PUSHS;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::SYMBOL ];
					break;
				case 'POPS':
					$this->type = InstructionType::POPS;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::VARIABLE ];
					break;

				/* Arithmetic
				============================================================================= */
				case 'ADD':
					$this->type = InstructionType::ADD;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'SUB':
					$this->type = InstructionType::SUB;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'MUL':
					$this->type = InstructionType::MUL;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'IDIV':
					$this->type = InstructionType::IDIV;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'LT':
					$this->type = InstructionType::LT;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'GT':
					$this->type = InstructionType::GT;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'EQ':
					$this->type = InstructionType::EQ;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'AND':
					$this->type = InstructionType::AND;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'OR':
					$this->type = InstructionType::OR;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'NOT':
					$this->type = InstructionType::NOT;
					$this->argumentsCount = 2;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL
					];
					break;
				case 'INT2CHAR':
					$this->type = InstructionType::INT2CHAR;
					$this->argumentsCount = 2;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL
					];
					break;
				case 'STRI2INT':
					$this->type = InstructionType::STRI2INT;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;

				/* I/O
				============================================================================= */
				case 'READ':
					$this->type = InstructionType::READ;
					$this->argumentsCount = 2;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::TYPE
					];
					break;
				case 'WRITE':
					$this->type = InstructionType::WRITE;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::SYMBOL ];
					break;

				/* Strings
				============================================================================= */
				case 'CONCAT':
					$this->type = InstructionType::CONCAT;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'STRLEN':
					$this->type = InstructionType::STRLEN;
					$this->argumentsCount = 2;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL
					];
					break;
				case 'GETCHAR':
					$this->type = InstructionType::GETCHAR;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;
				case 'SETCHAR':
					$this->type = InstructionType::SETCHAR;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];
					break;

				/* Types
				============================================================================= */
				case 'TYPE':
					$this->type = InstructionType::TYPE;
					$this->argumentsCount = 2;
					$this->argumentTypes = [
						ArgumentType::VARIABLE,
						ArgumentType::SYMBOL
					];
					break;

				/* Program flow
				============================================================================= */
				case 'LABEL':
					$this->type = InstructionType::LABEL;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::LABEL ];
					break;
				case 'JUMP':
					$this->type = InstructionType::JUMP;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::LABEL ];

					/* Log to statistics */
					$GLOBALS['STATS']->add(StatisticsType::JUMPS);
					break;
				case 'JUMPIFEQ':
					$this->type = InstructionType::JUMPIFEQ;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::LABEL,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];

					/* Log to statistics */
					$GLOBALS['STATS']->add(StatisticsType::JUMPS);
					break;
				case 'JUMPIFNEQ':
					$this->type = InstructionType::JUMPIFNEQ;
					$this->argumentsCount = 3;
					$this->argumentTypes = [
						ArgumentType::LABEL,
						ArgumentType::SYMBOL,
						ArgumentType::SYMBOL
					];

					/* Log to statistics */
					$GLOBALS['STATS']->add(StatisticsType::JUMPS);
					break;
				case 'EXIT':
					$this->type = InstructionType::EXIT;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::SYMBOL ];
					break;

				/* Debug
				============================================================================= */
				case 'DPRINT':
					$this->type = InstructionType::DPRINT;
					$this->argumentsCount = 1;
					$this->argumentTypes = [ ArgumentType::SYMBOL ];
					break;
				case 'BREAK':
					$this->type = InstructionType::BREAK;
					$this->argumentTypes = null;
					break;

				default:
					/* Incoming instruction is unknown */
					print('Error: Unknown instruction: ' . $this->name . PHP_EOL);
					exit(22);
			}
		}

		/**
		 * Gets all meta data and sends arguments to parse.
		 * @param array $rawSplit Line of code splitted by words
		 */
		protected function resolve($rawSplit) {
			/* Parse instruction name (first word) */
			$this->getInstructionMeta($rawSplit[0]);

			/* Check if parsed instruction has the right count of arguments */
			if (count($rawSplit) - 1 == $this->argumentsCount) {
				for ($i = 1; $i < count($rawSplit); $i++) {
					array_push(
						$this->arguments,
						new Argument(
							$rawSplit[$i],
							($this->argumentsCount != 0) ? $this->argumentTypes[$i - 1] : null
						)
					);
				}
			} else {
				print('Error: Incorrect number of instruction\'s arguments on line: ' . implode(' ', $rawSplit) . PHP_EOL);
				exit(23);
			}
		}

		/**
		 * Exports instruction as a XML element.
		 * @param $xml Parent XML element
		 * @param $orderNum instruction's order number
		 */
		public function export($xml, $orderNum) {
			/* Export base instruction node */
			$instrNode = $xml->addChild('instruction');
			$instrNode->addAttribute('order', $orderNum);
			$instrNode->addAttribute('opcode', $this->name);

			/* Export child notes (arguments) */
			for($i = 0; $i < count($this->arguments); $i++) {
				$this->arguments[$i]->export($instrNode, $i + 1);
			}
		}
	}
?>