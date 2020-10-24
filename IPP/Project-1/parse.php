<?php
	require('inc/Interfaces.php');

	require('class/Statistics.php');
	require('class/Element.php');
	require('class/Argument.php');
	require('class/Instruction.php');

	include('inc/ParamHandler.php');

	/**
	 * Trims line of comment.
	 * @param string $line Line of code in a raw form
	 */
	function trimLineComment($line){
		if (($hashPosition = strpos($line, '#')) !== false) {
			/* Log to statistics */
			$GLOBALS['STATS']->add(StatisticsType::COMMENTS);

			if ($hashPosition == 0) return false;
			else return substr($line, 0, $hashPosition);
		} else return $line;
	}

	$stdin = fopen('php://stdin', 'r');
	try {
		/* File header test */
		$line = trim(trimLineComment(fgets($stdin)));
		if (strcasecmp($line, '.IPPcode19') != 0) {
			print('Error: IPPCode header is missing or is incorrectly formatted!' . PHP_EOL);
			exit(21);
		}

		/* Log to statistics */
		$GLOBALS['STATS']->add(StatisticsType::LOC);

		/* Create an empty array for instructions */
		$INSTRUCTION_STACK = array();

		while (($line = trim(fgets($stdin))) != false) {
			/* Log to statistics */
			$GLOBALS['STATS']->add(StatisticsType::LOC);

			/* Skip line if starts with '#' (Comment) */
			$line = trimLineComment($line);
			if (!$line) continue;

			array_push($INSTRUCTION_STACK, new Instruction($line));
		}
	} catch (Exception $e) {
		print($e->getMessage() . PHP_EOL);
		exit(11);
	} finally {
		fclose($stdin);

		/* Create the XML output document */
		$xml = new SimpleXMLElement('<?xml version="1.0" encoding="UTF-8"?>'
		.'<program language="IPPcode19"></program>');

		for($i = 0; $i < count($INSTRUCTION_STACK); $i++) {
			$INSTRUCTION_STACK[$i]->export($xml, $i + 1);
		}

		try {
			$GLOBALS['STATS']->write();
		} catch (Exception $e) {
			print($e->getMessage() . PHP_EOL);
			exit(12);
		}

		/* Print the result XML document */
		$dom = new DOMDocument("1.0");
		$dom->preserveWhiteSpace = false;
		$dom->formatOutput = true;
		$dom->loadXML($xml->asXML());
		echo $dom->saveXML();
	}
?>