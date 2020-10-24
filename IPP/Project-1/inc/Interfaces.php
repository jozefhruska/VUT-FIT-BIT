<?php
	interface InstructionType {

		/* Frames, function calls
		============================================================================= */
		const MOVE = 000;
		const CREATEFRAME = 001;
		const PUSHFRAME = 002;
		const POPFRAME = 003;
		const DEFVAR = 004;
		const CALL = 005;
		const RETURN = 006;

		/* Data stack
		============================================================================= */
		const PUSHS = 100;
		const POPS = 101;

		/* Arithmetic
		============================================================================= */
		const ADD = 200;
		const SUB = 201;
		const MUL = 202;
		const IDIV = 203;
		const LT = 204;
		const GT = 205;
		const EQ = 206;
		const AND = 207;
		const OR = 208;
		const NOT = 209;
		const INT2CHAR = 210;
		const STRI2INT = 211;

		/* I/O
		============================================================================= */
		const READ = 300;
		const WRITE = 301;

		/* Strings
		============================================================================= */
		const CONCAT = 400;
		const STRLEN = 401;
		const GETCHAR = 402;
		const SETCHAR = 403;

		/* Types
		============================================================================= */
		const TYPE = 500;

		/* Program flow
		============================================================================= */
		const LABEL = 600;
		const JUMP = 601;
		const JUMPIFEQ = 602;
		const JUMPIFNEQ = 603;
		const EXIT = 604;

		/* Debug
		============================================================================= */
		const DPRINT = 700;
		const BREAK = 701;

	}

	interface ArgumentType {
		const VARIABLE = 0;
		const SYMBOL = 1;
		const LABEL = 2;
		const TYPE = 3;
	}

	interface ArgumentValue {
		const INTEGER = 0;
		const BOOLEAN = 1;
		const STRING = 2;
		const NIL = 3;
		const VARIABLE = 4;
		const LABEL = 5;
		const TYPE = 6;
	}

	interface StatisticsType {
		const LOC = 0;
		const COMMENTS = 1;
		const LABELS = 2;
		const JUMPS = 3;
	}
?>