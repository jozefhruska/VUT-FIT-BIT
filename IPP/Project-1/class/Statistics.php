<?php
	/**
	 * Class for easier manipulation, recording and exporting statistics.
	 */
	class Statistics {
		public $enabled = false;
		private $stats = array();
		private $file;

		/**
		 * Initializes statistics meta data.
		 * @param bool $enabled If statistics are enabled (--stats param)
		 * @param array $paramsInOrder Program parameters filtered and sorted
		 * @param $file Output file for statistics
		 */
		public function create($enabled, $paramsInOrder, $file) {
			if ($enabled) {
				foreach ($paramsInOrder as $param => $value) {
					switch($param) {
						case 'loc':
							$this->stats[StatisticsType::LOC] = 0;
							break;
						case 'comments':
							$this->stats[StatisticsType::COMMENTS] = 0;
							break;
						case 'labels':
							$this->stats[StatisticsType::LABELS] = 0;
							break;
						case "jumps":
							$this->stats[StatisticsType::JUMPS] = 0;
							break;
					}
				}

				$this->enabled = true;
				$this->file = $file;
			}
		}

		/**
		 * Used to record new statistics occurence.
		 * @param $type Type of occurence to record
		 */
		public function add($type) {
			if ($this->enabled) {
				if (array_key_exists($type, $this->stats)) $this->stats[$type]++;
			}
		}

		/**
		 * Write all statistics to an output file.
		 */
		public function write() {
			if ($this->enabled) {
				if (count($this->stats) > 0) {
					foreach ($this->stats as $stat) {
						try {
							fwrite($this->file, $stat . "\n");
						} catch (Exception $e) {
							print($e->getMessage() . PHP_EOL);
							exit(12);
						}
					}
				}

				if ($this->file) fclose($this->file);
			}
		}
	}
?>