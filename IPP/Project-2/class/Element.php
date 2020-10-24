<?php
	/**
	 * Starter class for an Instruction and Argument classes.
	 * @abstract
	 */
	abstract class Element {
		abstract protected function resolve($raw);
		abstract protected function export($xml, $orderNumber);
	}
?>