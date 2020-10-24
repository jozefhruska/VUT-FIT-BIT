<?php
	$helpContent = "\nIDS Project - Parser\n"
								."\nAuthor: Jozef Hruška (xhrusk25)"
								."\nUsage: This script takes IPPcode19 input from STDIN and outputs it to STDOUT.\n"
								."\nOptions:\n"
								."--help            Shows this help screen.\n"
								."--stats=<file>    Set desired statistics output file.\n"
								."--loc             Log number of lines of code to the statistics.\n"
								."--comments        Log number of comments to the statistics.\n"
								."--labels          Log number of labels statistics.\n"
								."--jumps           Log number of JUMP instructions to the statistics.\n";

	/* Create a statistics object */
	$GLOBALS['STATS'] = new Statistics();

	/* List of possible program parameters */
	$longParams = [ 'help', 'stats:', 'loc', 'comments', 'labels', 'jumps' ];

	/* Get all program parameters */
	$parameters = getopt(null, $longParams);

	/* Parameter found - [--help] */
	if (array_key_exists('help', $parameters) && count($parameters) == 1) {
		echo $helpContent;
		exit;
	}

	/* Parameter found - [--stats] */
	if (array_key_exists('stats', $parameters)) {
		$paramsInOrder = array_filter($parameters, function($value){
			return in_array($value, ['loc', 'comments', 'labels', 'jumps']);
		}, ARRAY_FILTER_USE_KEY);

		if (count($paramsInOrder) == 0) {
			print('Error: Wrong usage of program parameters!' . PHP_EOL);
			exit(10);
		}

		try {
			$statsFile = fopen($parameters['stats'], "w");
		} catch (Exception $e) {
			print($e->getMessage() . PHP_EOL);
			exit(12);
		}

		$GLOBALS['STATS']->create(true, $paramsInOrder, $statsFile);
	}
?>