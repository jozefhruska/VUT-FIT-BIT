<?php
	class Program {
		function __construct() {
			$this->helpContent = "\IPP Project - Test\n"
			."\nAuthor: Jozef Hruška (xhrusk25)"
			."\nUsage: Automatized testing of IPPcode19 parser and interpret.\n"
			."\nOptions:\n"
			."--help                    Shows this help screen.\n"
			."--directory=<path>        Path to the directory with tests.\n"
			."--recursive               Search for tests recursively.\n"
			."--parse-script=<path>     Path to the parser script.\n"
			."--int-script=<path>       Path to the interpret script.\n"
			."--parse-only              Run tests on the parser only.\n"
			."--int-only                Run tests on the interpret only.\n";

			/* List of possible program parameters */
			$this->longParams = [ 'help', 'directory:', 'recursive', 'parse-script:', 'int-script:', 'parse-only', 'int-only' ];

			/* Program options */
			$this->options = [];
			$this->options['parse-script'] = false;
			$this->options['int-script'] = false;
			$this->options['recursive'] = false;
			$this->options['parse-only'] = false;
			$this->options['int-only'] = false;

			/* Final stats */
			$this->stats['success'] = 0;
			$this->stats['fail'] = 0;
		}

		public function parseArguments() {
			/* Get all program arguments */
			$arguments = getopt(null, $this->longParams);

			/* Parse argument options */
			foreach ($arguments as $key => $value) {
				switch($key) {
					case 'help':
						if (count($arguments) == 1) {
							echo $this->helpContent;
							exit;
						} else $this->exitWithMessage(1, 'Error: Wrong use of program arguments. (\'--help\' cannot be combined with other arguments.)');
					break;

					case 'directory':
						$this->options['directory'] = realpath($value);

						if (!file_exists($this->options['directory'])) {
							$this->exitWithMessage(1, 'Error: Specified directory with tests does not exist.');
						}
					break;

					case 'recursive':
						$this->options['recursive'] = true;
					break;

					case 'parse-script':
						$this->options['parse-script'] = $value;

						if (!file_exists($this->options['parse-script'])) {
							$this->exitWithMessage(1, 'Error: Specified parser script does not exist.');
						}
					break;

					case 'int-script':
						$this->options['int-script'] = $value;

						if (!file_exists($this->options['int-script'])) {
							$this->exitWithMessage(1, 'Error: Specified interpret script does not exist.');
						}
					break;

					case 'parse-only':
						$this->options['parse-only'] = true;
					break;

					case 'int-only':
						$this->options['int-only'] = true;
					break;
				}
			}

			/* Check if 'parse-only' and 'int-only' is used correctly */
			if ($this->options['parse-only'] && $this->options['int-only'])
				$this->exitWithMessage(1, 'Error: Invalid combination of \'parse-only\' and \'int-only\' arguments.');
			else if (!$this->options['parse-only'] && !$this->options['int-only'])
				$this->options['both'] = true;

			/* Check if script paths are supplied */
			if ($this->options['parse-only'] && !$this->options['parse-script'])
				$this->exitWithMessage(1, 'Error: Parser script path is missing.');
			if ($this->options['int-only'] && !$this->options['int-script'])
				$this->exitWithMessage(1, 'Error: Interpret script path is missing.');
		}

		public function exitWithMessage($code, $message) {
			fprintf(STDERR, $message . PHP_EOL);
			exit($code);
		}
	}

	class DirectoryHelper {
		function __construct($program) {
			$this->program = $program;

			$this->directory = $program->options['directory'];
			$this->recursive = $program->options['recursive'];

			$this->testDirectories['parse-only'] = [];
			$this->testDirectories['int-only'] = [];
			$this->testDirectories['both'] = [];
			$this->testFiles = [];

			/* Search for the specific tests */
			$this->search('parse-only');
			$this->search('int-only');
			$this->search('both');
		}

		private function search($testType) {
			$directory = new RecursiveDirectoryIterator($this->directory . '/' . $testType);
			$iterator = ($this->recursive) ? new RecursiveIteratorIterator($directory) : new IteratorIterator($directory);

			$regex = new RegexIterator($iterator, '/^.+\.src$/i', RecursiveRegexIterator::GET_MATCH);
			foreach ($regex as $result) {
				$fileName = preg_replace('/^(.*\/)?(.+)\.src$/','\2', $result[0]);
				$fileDir = preg_replace('/^(.*\/).+\.(in|out|rc|src)$/','\1', $result[0]);

				if (!in_array($fileDir, $this->testDirectories[$testType]))
					$this->testDirectories[$testType][] = $fileDir;

				$this->testFiles[$testType][$fileDir][] = $fileName;

				if (!file_exists($fileDir . $fileName . '.rc'))
					file_put_contents($fileDir . $fileName, '0');

				if (!file_exists($fileDir . $fileName . '.in'))
					file_put_contents($fileDir . $fileName, '');

				if (!file_exists($fileDir . $fileName . '.out'))
					file_put_contents($fileDir . $fileName, '');
			}
		}

		public function getTestDirectories() {
			return $this->testDirectories;
		}

		public function getTestFiles() {
			return $this->testFiles;
		}
	}

	class TestResult {
		function __construct($number, $path, $expected, $result, $message, $case) {
			$this->number = $number;
			$this->path = $path;
			$this->expected = $expected;
			$this->result = $result;
			$this->message = $message;
			$this->case = $case;
		}
	}

	class StatsHelper {
		function __construct($program) {
			$this->program = $program;

			$this->success = [];
			$this->fail = [];
		}

		public function exportResult() {
			$html = '<!doctype html>

			<html lang="en">
			<head>
				<meta charset="utf-8">

				<title>IPP Test result</title>
				<meta name="author" content="Jozef Hruška">

				<style>
					body {
						display: flex;
						flex-direction: row;
						flex-wrap: wrap;
						justify-content: space-between;
						padding-top: 65px;
					}

					.column {
						width: 49%;
					}

					.test_item {
						display: flex;
						width: 100%;
						margin-right: 10px;
						min-height: 120px;
						border-radius: 5px;
						overflow: hidden;
						margin-bottom: 10px;
					}

					.test_item.success .test_item_content {
						background: #66bb6a;
					}

					.test_item_content {
						width: 100%;
						padding: 15px;
						background: #ef5350;
					}

					.test_item_header {
						font-weight: bold;
						padding-bottom: 15px;
						border-bottom: 1px solid #263238;
					}

					.test_item_body {
						padding-top: 10px;
					}

					.summary {
						position: fixed;
						top: 10px;
						left: 10px;
						right: 10px;
						background: #e0e0e0;
						border-radius: 5px;
						padding: 15px;
					}

					.summary p {
						display: inline;
						padding-right: 20px;
					}
				</style>

			</head>

			<body>

			<div class="summary">
				<p>Total tests: ' . ($this->program->stats['success'] + $this->program->stats['fail']) . ' </p>
				<p>Fail: ' . $this->program->stats['fail'] . ' </p>
				<p>Success: ' . $this->program->stats['success'] . ' </p>
			</div>

			<div class="column">';

			foreach ($this->fail as $testResult) {
				$html .= '<div class="test_item">';
					$html .= '<div class="test_item_content">';
						$html .= '<div class="test_item_header">';
							$html .= '#' . $testResult->number . ' - ' . $testResult->path;
						$html .= '</div>';
						$html .= '<div class="test_item_body">';
							$html .= 'Expected return code: <strong>' . $testResult->expected . '</strong>, Actual return code: <strong>' . $testResult->result . '</strong>';
						$html .= '</div>';
						if ($testResult->message) {
							$html .= '<div class="test_item_body">';
								$html .= '<div class="test_item_body">Message: ' . $testResult->message . '</div>';
							$html .= '</div>';
						}
						$html .= '<div class="test_item_body">';
							$html .= '<div class="test_item_body">Result: <strong>FAIL</strong></div>';
						$html .= '</div>';
					$html .= '</div>';
				$html .= '</div>';
			}

			$html .= '</div>';
			$html .= '<div class="column">';

			foreach ($this->success as $testResult) {
				$html .= '<div class="test_item success">';
				$html .= '<div class="test_item_content">';
					$html .= '<div class="test_item_header">';
						$html .= '#' . $testResult->number . ' - ' . $testResult->path;
					$html .= '</div>';
					$html .= '<div class="test_item_body">';
						$html .= 'Expected return code: <strong>' . $testResult->expected . '</strong>, Actual return code: <strong>' . $testResult->result . '</strong>';
					$html .= '</div>';
					$html .= '<div class="test_item_body">';
						$html .= '<div class="test_item_body">Result: <strong>SUCCESS</strong></div>';
					$html .= '</div>';
				$html .= '</div>';
				$html .= '</div>';
			}

			$html .= '</div>';

			$html .='</body></html>';

			echo($html);
		}
	}

	/* Initialize */
	$mainProgram = new Program();
	$mainProgram->parseArguments();

	$directoryHelper = new DirectoryHelper($mainProgram);
	$statsHelper = new StatsHelper($mainProgram);

	/* Decide which tests to run */
	$testsToRun = 'both';
	if ($mainProgram->options['parse-only']) $testsToRun = 'parse-only';
	else if ($mainProgram->options['int-only']) $testsToRun = 'int-only';

	/* Run tests */
	foreach ($directoryHelper->getTestDirectories()[$testsToRun] as $directory) {
		foreach ($directoryHelper->getTestFiles()[$testsToRun][$directory] as $fileName) {

			/***********************************/
			/* Test - Parser only [BEGIN]      */
			/***********************************/
			if ($mainProgram->options['parse-only']) {
				/* Prepare the data arrays */
				$routes['src'] = $directory . $fileName . '.src';
				$routes['rc'] = $directory . $fileName . '.rc';
				$routes['in'] = $directory . $fileName . '.in';
				$routes['out'] = $directory . $fileName . '.out';

				$test['parser']['rc'] = null;
				$test['parser']['output'] = null;

				$correct['parser']['rc'] = null;

				$diff['rc'] = null;

				/* Get the correct RC for the parser test */
				$rcFile = fopen($routes['rc'], 'r');
				$rcValue = fgets($rcFile);
				$correct['parser']['rc'] = trim($rcValue);
				fclose($rcFile);

				/* Create a temporary file to compare outputs */
				$tempFile = fopen('out.temp', 'a');
				$errFile = fopen(__dir__ . '/err.temp', 'a');

				exec('php7.3 ' . $mainProgram->options['parse-script'] . ' < ' . $routes['src'] . ' > ./out.temp 2> ./err.temp', $test['parser']['output'], $test['parser']['rc']);

				$errMessage = file_get_contents(__dir__ . '/err.temp');

				if ($correct['parser']['rc'] != $test['parser']['rc']) {
					$mainProgram->stats['fail']++;
					$statsHelper->fail[] = new TestResult(
						$mainProgram->stats['fail'] + $mainProgram->stats['success'],
						$routes['src'],
						$correct['parser']['rc'],
						$test['parser']['rc'],
						$errMessage,
						'RC'
					);
				} else {
					$mainProgram->stats['success']++;

					$statsHelper->success[] = new TestResult(
						$mainProgram->stats['fail'] + $mainProgram->stats['success'],
						$routes['src'],
						$correct['parser']['rc'],
						$test['parser']['rc'],
						$errMessage,
						'RC'
					);
				}

				fclose($errFile);
				unlink('err.temp');

				fclose($tempFile);
				unlink('out.temp');
			}
			/***********************************/
			/* Test - Parser only [END]        */
			/***********************************/


			/***********************************/
			/* Test - Interpret only [BEGIN]   */
			/***********************************/
			else if ($mainProgram->options['int-only']) {
				/* Prepare the data arrays */
				$routes['src'] = $directory . $fileName . '.src';
				$routes['rc'] = $directory . $fileName . '.rc';
				$routes['in'] = $directory . $fileName . '.in';
				$routes['out'] = $directory . $fileName . '.out';

				$test['parser']['rc'] = null;
				$test['parser']['output'] = null;
				$test['interpret']['rc'] = null;
				$test['interpret']['output'] = null;

				$correct['parser']['rc'] = null;
				$correct['interpret']['rc'] = null;

				$diff['rc'] = null;

				/* Get the correct RC for the interpret test */
				$rcFile = fopen($routes['rc'], 'r');
				$rcValue = fgets($rcFile);
				$correct['interpret']['rc'] = trim($rcValue);
				fclose($rcFile);

				/* Create a temporary file to compare outputs */
				$tempFile = fopen(__dir__ . '/temp.temp', 'a');
				$errFile = fopen(__dir__ . '/err.temp', 'a');

				exec('python3 ' . $mainProgram->options['int-script'] . ' --source=' . $routes['src'] . ' --input=' . $routes['in'] . ' > ./out.temp 2> ./err.temp', $test['interpret']['output'], $test['interpret']['rc']);

				exec('diff ./out.temp ' . $routes['out'], $diff['output'], $diff['rc']);

				$errMessage = file_get_contents(__dir__ . '/err.temp');

				if ($correct['interpret']['rc'] != $test['interpret']['rc']) {
					$mainProgram->stats['fail']++;

					$statsHelper->fail[] = new TestResult(
						$mainProgram->stats['fail'] + $mainProgram->stats['success'],
						$routes['src'],
						$correct['interpret']['rc'],
						$test['interpret']['rc'],
						$errMessage,
						'RC'
					);
				} else if ($diff['rc'] != 0) {
					$mainProgram->stats['fail']++;

					$statsHelper->fail[] = new TestResult(
						$mainProgram->stats['fail'] + $mainProgram->stats['success'],
						$routes['src'],
						$correct['interpret']['rc'],
						$test['interpret']['rc'],
						$errMessage,
						'DIFF'
					);
				} else {
					$mainProgram->stats['success']++;

					$statsHelper->success[] = new TestResult(
						$mainProgram->stats['fail'] + $mainProgram->stats['success'],
						$routes['src'],
						$correct['interpret']['rc'],
						$test['interpret']['rc'],
						null,
						true
					);
				}

				fclose($errFile);
				unlink('err.temp');

				fclose($tempFile);
				unlink('out.temp');
			}
			/***********************************/
			/* Test - Interpret only [END]     */
			/***********************************/


			/***********************************/
			/* Test - Both [START]             */
			/***********************************/
			else {
				/* Prepare the data arrays */
				$routes['src'] = $directory . $fileName . '.src';
				$routes['rc'] = $directory . $fileName . '.rc';
				$routes['in'] = $directory . $fileName . '.in';
				$routes['out'] = $directory . $fileName . '.out';

				$test['parser']['rc'] = null;
				$test['parser']['output'] = null;
				$test['interpret']['rc'] = null;
				$test['interpret']['output'] = null;

				$correct['rc'] = null;

				$diff['rc'] = null;

				/* Get the correct RC for the parser test */
				$rcFile = fopen($routes['rc'], 'r');
				$rcValue = fgets($rcFile);
				$correct['rc'] = trim($rcValue);
				fclose($rcFile);

				/* Create a temporary file to compare outputs */
				$tempFile = fopen(__dir__ . '/out.temp', 'a');
				$intFile = fopen(__dir__ . '/int-out.temp', 'a');
				$errFile = fopen(__dir__ . '/err.temp', 'a');

				exec('php7.3 ' . $mainProgram->options['parse-script'] . ' < ' . $routes['src'] . ' > ./out.temp 2> ./err.temp', $test['parser']['output'], $test['parser']['rc']);

				$errMessage = file_get_contents(__dir__ . '/err.temp');

				if ($test['parser']['rc'] != 0) {
					if ($correct['rc'] != $test['parser']['rc']) {
						$mainProgram->stats['fail']++;

						$statsHelper->fail[] = new TestResult(
							$mainProgram->stats['fail'] + $mainProgram->stats['success'],
							$routes['src'],
							$correct['rc'],
							$test['parser']['rc'],
							$errMessage,
							false
						);
					} else {
						$mainProgram->stats['success']++;

						$statsHelper->success[] = new TestResult(
							$mainProgram->stats['fail'] + $mainProgram->stats['success'],
							$routes['src'],
							$correct['rc'],
							$test['parser']['rc'],
							$errMessage,
							false
						);
					}
				} else {
					exec('python3 ' . $mainProgram->options['int-script'] . ' --source=./out.temp > int-out.temp 2> ./err.temp --input=' . $routes['in'], $test['interpret']['output'], $test['interpret']['rc']);

					$errMessage = file_get_contents(__dir__ . '/err.temp');

					exec('diff ./int-out.temp ' . $routes['out'], $diff['output'], $diff['rc']);

					if ($correct['rc'] != $test['interpret']['rc']) {
						$mainProgram->stats['fail']++;

						$statsHelper->fail[] = new TestResult(
							$mainProgram->stats['fail'] + $mainProgram->stats['success'],
							$routes['src'],
							$correct['rc'],
							$test['interpret']['rc'],
							$errMessage,
							false
						);
					} else if ($diff['rc'] != 0) {
						$mainProgram->stats['fail']++;

						$statsHelper->fail[] = new TestResult(
							$mainProgram->stats['fail'] + $mainProgram->stats['success'],
							$routes['src'],
							$correct['rc'],
							$test['interpret']['rc'],
							$errMessage,
							false
						);


					} else {
						$mainProgram->stats['success']++;

						$statsHelper->success[] = new TestResult(
							$mainProgram->stats['fail'] + $mainProgram->stats['success'],
							$routes['src'],
							$correct['rc'],
							$test['interpret']['rc'],
							$errMessage,
							false
						);
					}

					fclose($errFile);
					unlink('err.temp');

					fclose($intFile);
					unlink('int-out.temp');

					fclose($tempFile);
					unlink('out.temp');
				}
			}
			/***********************************/
			/* Test - Both [END]               */
			/***********************************/
		}
	}

	$statsHelper->exportResult()
?>