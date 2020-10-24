<?php
	class Argument extends Element {
		private $type;

		private $dataType;
		private $dataValue;
		private $dataLocation;

		function __construct($raw, $requiredType) {
			/* Store argument's type */
			$this->type = $requiredType;

			/* Resolve argument's data */
			$this->resolve($raw);
		}

		/**
		 * Stores data to internal properties.
		 */
		private function storeData($dataType, $dataValue, $dataLocation = null) {
			$this->dataType = $dataType;
			$this->dataValue = $dataValue;
			$this->dataLocation = $dataLocation;
		}

		/**
		 * Decides what type of an argument it recieved.
		 * @param string $raw Argument data in a raw form
		 */
		protected function resolve($raw) {
			/* INTEGER */
			if (preg_match('/^int@/', $raw)) {
				$this->storeData(
					ArgumentValue::INTEGER,
					htmlspecialchars(substr($raw, strpos($raw, '@') + 1), ENT_NOQUOTES)
				);

				$this->checkDataType();
				return;
			}

			/* BOOLEAN */
			if (preg_match('/^bool@true$/', $raw) || preg_match('/^bool@true$/', $raw)) {
				$this->storeData(
					ArgumentValue::BOOLEAN,
					htmlspecialchars(substr($raw, strpos($raw, '@') + 1), ENT_NOQUOTES)
				);

				$this->checkDataType();
				return;
			}

			/* NIL */
			if (preg_match('/^nil@nil$/', $raw)) {
				$this->storeData(
					ArgumentValue::NIL,
					htmlspecialchars(substr($raw, strpos($raw, '@') + 1), ENT_NOQUOTES)
				);

				$this->checkDataType();
				return;
			}

			/* STRING */
			if (preg_match('/^string@/', $raw)) {
				/* Check for incorrect unicode escape sequences */
				$backslashPositions = array();
				$lastPosition = 0;

				while (($lastPosition = strpos($raw, '\\', $lastPosition))!== false) {
					$backslashPositions[] = $lastPosition;
					$lastPosition++;
				}

				foreach ($backslashPositions as $pos) {
					if (!preg_match('/^\\\\[0-9][0-9][0-9]$/', substr($raw, $pos, 4))) {
						print('Error: Something went wrong while attempting to parse an argument!' . PHP_EOL);
						exit(23);
					}
				}

				$this->storeData(
					ArgumentValue::STRING,
					htmlspecialchars(substr($raw, strpos($raw, '@') + 1), ENT_NOQUOTES)
				);

				$this->checkDataType();
				return;
			}

			/* VARIABLE */
			if (preg_match('/^(GF|LF|TF)@([a-zA-Z]|(_|-|\$|\&|%|\*|!|\?))(([a-zA-Z]|(_|-|\$|\&|%|\*|!|\?))|\d)*$/', $raw)) {
				$this->storeData(
					ArgumentValue::VARIABLE,
					htmlspecialchars(substr($raw, strpos($raw, '@') + 1), ENT_NOQUOTES),
					substr($raw, 0, strpos($raw, '@'))
				);

				$this->checkDataType();
				return;
			}

			/* LABEL */
			if (preg_match('/^([a-zA-Z]|(_|-|\$|\&|%|\*|!|\?))(([a-zA-Z]|(_|-|\$|\&|%|\*|!|\?))|\d)*$/', $raw)) {
				/* TYPE */
				if ($raw === 'int' || $raw === 'bool' || $raw === 'string') {
					$this->storeData(
						ArgumentValue::TYPE,
						$raw
					);

					$this->checkDataType();
					return;
				}

				$this->storeData(
					ArgumentValue::LABEL,
					htmlspecialchars($raw, ENT_NOQUOTES)
				);

				/* Log to statistics */
				$GLOBALS['STATS']->add(StatisticsType::LABELS);

				$this->checkDataType();
				return;
			}

			print('Error: Something went wrong while attempting to parse an argument!' . PHP_EOL);
			exit(23);
		}

		/**
		 * Checks if data type corresponds with that given by instruction.
		 */
		private function checkDataType() {
			switch($this->type) {
				case ArgumentType::VARIABLE:
					if ($this->dataType != ArgumentValue::VARIABLE) {
						print('Error: Something went wrong while attempting to parse an argument!' . PHP_EOL);
						exit(23);
					}
					return;

				case ArgumentType::SYMBOL:
					if (
						$this->dataType != ArgumentValue::VARIABLE
						&& $this->dataType != ArgumentValue::INTEGER
						&& $this->dataType != ArgumentValue::BOOLEAN
						&& $this->dataType != ArgumentValue::STRING
						&& $this->dataType != ArgumentValue::NIL
						) {
						print('Error: Something went wrong while attempting to parse an argument!' . PHP_EOL);
						exit(23);
					}
					return;

				case ArgumentType::LABEL:
					if ($this->dataType != ArgumentValue::LABEL) {
						print('Error: Something went wrong while attempting to parse an argument!' . PHP_EOL);
						exit(23);
					}
					return;

				case ArgumentType::TYPE:
					if ($this->dataType != ArgumentValue::TYPE) {
						print('Error: Something went wrong while attempting to parse an argument!' . PHP_EOL);
						exit(23);
					}
					return;
			}
		}

		/**
		 * Exports argument as a XML element.
		 * @param $xml Parent XML element
		 * @param $orderNum argument's order number
		 */
		public function export($instrNode, $orderNum) {
			switch($this->dataType) {
				case ArgumentValue::INTEGER:
					$argNode = $instrNode->addChild('arg' . $orderNum, $this->dataValue);
					$argNode->addAttribute('type', 'int');
					break;
				case ArgumentValue::BOOLEAN:
					$argNode = $instrNode->addChild('arg' . $orderNum, $this->dataValue);
					$argNode->addAttribute('type', 'bool');
					break;
				case ArgumentValue::STRING:
					$argNode = $instrNode->addChild('arg' . $orderNum, $this->dataValue);
					$argNode->addAttribute('type', 'string');
					break;
				case ArgumentValue::NIL:
					$argNode = $instrNode->addChild('arg' . $orderNum, $this->dataValue);
					$argNode->addAttribute('type', 'nil');
					break;
				case ArgumentValue::VARIABLE:
					$argNode = $instrNode->addChild('arg' . $orderNum, $this->dataLocation . '@' . $this->dataValue);
					$argNode->addAttribute('type', 'var');
					break;
				case ArgumentValue::LABEL:
					$argNode = $instrNode->addChild('arg' . $orderNum, $this->dataValue);
					$argNode->addAttribute('type', 'label');
					break;
				case ArgumentValue::TYPE:
					$argNode = $instrNode->addChild('arg' . $orderNum, $this->dataValue);
					$argNode->addAttribute('type', 'type');
					break;
			}
		}
	}
?>