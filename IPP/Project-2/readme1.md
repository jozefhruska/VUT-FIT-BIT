# Documentation of Project Implementation for IPP 2017/2018
**Name and surname**: Jozef Hruška  
**Login**: xhrusk25

## Parse.php
Script opens *stdin* and reads code line by line. Each line is stripped down of comments with **trimLineComment()** function. Line is then used construction variable for **Instruction** class.

### Element.php
Abstract class used as a starting point for **Instruction** and **Argument** classes. Contains two protected functions **resolve()** and **export()**.

### Instruction.php
Wrapper class for all instruction data. Constructor function splits raw line by words to separate instruction name and arguments. Instruction name is then compared to all known instructions. If the match is found, meta data (instruction type, number of arguments and their types) is stored as object properties. Arguments are then wrapped one by one with an **Argument** class.

### Argument.php
Wrapper class for all argument data. Constructor function decides what type of argument it is based on comparing with **preg_match()**. After argument type is known it's decited if this type corresponds with required one sent by an **Instruction**.

### Statistics.php
This class is used for easier manipulation with logging & outputing statistics file at the end of a script run. Contains two public function **add()** used to record occurence to statistics and **write()** used to export final statistics to a file on a requested location.

### Other files
#### ParamHandler.php
Used for a manipulation with program's arguments.

#### Interfaces.php
Interfaces that hold constants (for a easier manipulation, classification of multiple types).
