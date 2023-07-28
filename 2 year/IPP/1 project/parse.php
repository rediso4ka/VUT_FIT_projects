<?php

ini_set('display_errors', 'stderr');

///////////////
// functions //
///////////////

/**
 * Prints help info if --help is given
 */
function print_help() {
    echo("\n\t\tPARSER.PHP\n\n");
    echo("This is a filter type script, which reads the source code\n");
    echo("in IPPcode23 language from a standard input, checks the\n");
    echo("lexical and syntactic correctness of the code, and prints\n");
    echo("an XML representation of the program to the standard output.\n\n");
    echo("Usage: php8.1 parse.php <input_file\n\n");
}

/**
 * User's arguments parsing
 * 
 * @param int $argc Amount of arguments
 * @param string[] $argv List of given arguments
 */
function parse_args($argc, $argv) {
    if ($argc > 1){
        if ($argc == 2) {
            if ($argv[1] == "--help") {
                print_help();
                exit(0);
            } else {
                fprintf(STDERR, "INVALID SECOND ARGUMENT!\n");
                exit(10);
            }
        } else {
            fprintf(STDERR, "TOO MUCH ARGUMENTS!\n");
            exit(10);
        }
    }
}

/**
 * Start of creating xml
 * 
 * @param xml $xml Empty xml
 */
function xml_start($xml) {
    $xml->startDocument('1.0', 'utf-8');
    $xml->setIndent(true);
    $xml->startElement('program');
    $xml->writeAttribute('language', 'IPPcode23');
}

/**
 * Ending xml
 * 
 * @param xml $xml
 */
function xml_stop($xml) {
    $xml->endElement();
    $xml->endDocument();
    fwrite(STDOUT, $xml->outputMemory());
    $xml->flush();
}

/**
 * Printing an instruction to xml
 * 
 * @param xml $xml
 * @param string $instruction
 * @param int $order Order of instruction in the whole program
 */
function xml_print_instr($xml, $instruction, $order) {
    $xml->startElement('instruction');
    $xml->writeAttribute('order', $order);
    $xml->writeAttribute('opcode', strtoupper($instruction));
}

/**
 * Printing an operand to xml
 * 
 * @param xml $xml
 * @param int $number Order of an operand in instruction
 * @param string $type
 * @param string $info Content of the operand
 */
function xml_print_operand($xml, $number, $type, $info) {
    $xml->startElement($number);
    $xml->writeAttribute('type', $type);
    $xml->text($info);
    $xml->endElement();
}

/**
 * Matching variable
 * 
 * @param xml $xml
 * @param string $var
 * @param int $pos
 */
function match_var($xml, $var, $pos) {
    if (preg_match('/^(LF|TF|GF)@[a-zA-Z_\-\$&%\*!\?][a-zA-Z0-9_\-\$&%\*!\?]*$/', $var)) {
        xml_print_operand($xml, 'arg'.$pos, 'var', $var);
    } else {
        fprintf(STDERR, "WRONG VAR FORMAT!\n");
        exit(23);
    }
}

/**
 * Matching label
 * 
 * @param xml $xml
 * @param string $label
 * @param int $pos
 */
function match_label($xml, $label, $pos) {
    if (preg_match('/^[a-zA-Z_\-\$&%\*!\?][a-zA-Z0-9_\-\$&%\*!\?]*$/', $label)) {
        xml_print_operand($xml, 'arg'.$pos, 'label', $label);
    } else {
        fprintf(STDERR, "WRONG LABEL FORMAT!\n");
        exit(23);
    }
}

/**
 * Matching symbol
 * 
 * @param xml $xml
 * @param string $symb
 * @param int $pos
 */
function match_symb($xml, $symb, $pos) {
    $without_dec = preg_replace('/\\\\\d\d\d/', '', $symb);
    if (preg_match('/^int@[-+]?\d+$/', $symb)) {
        $symb = preg_replace('/^int@/', '', $symb);
        xml_print_operand($xml, 'arg'.$pos, 'int', $symb);
    } else if (preg_match('/^bool@(false|true)$/', $symb)) {
        $symb = preg_replace('/^bool@/', '', $symb);
        xml_print_operand($xml, 'arg'.$pos, 'bool', $symb);
    } else if (preg_match('/^string@((?!.*\\\\).+)*$/', $without_dec)) {
        $symb = preg_replace('/^string@/', '', $symb);
        xml_print_operand($xml, 'arg'.$pos, 'string', $symb);
    } else if (preg_match('/^nil@nil$/', $symb)){
        $symb = preg_replace('/^nil@/', '', $symb);
        xml_print_operand($xml, 'arg'.$pos, 'nil', $symb);
    } else if (preg_match('/^(LF|TF|GF)@[a-zA-Z_\-\$&%\*!\?][a-zA-Z0-9_\-\$&%\*!\?]*/', $symb)) {
        xml_print_operand($xml, 'arg'.$pos, 'var', $symb);
    } else {
        fprintf(STDERR, "WRONG SYMB FORMAT!\n");
        exit(23);
    }
}

/**
 * Matching type
 * 
 * @param xml $xml
 * @param string $type
 * @param int $pos
 */
function match_type($xml, $type, $pos) {
    if (preg_match('/^(int|string|bool)$/', $type)) {
        xml_print_operand($xml, 'arg'.$pos, 'type', $type);
    } else {
        fprintf(STDERR, "WRONG TYPE FORMAT!\n");
        exit(23);
    }
}

///////////
// cases //
///////////

/**
 * Processing instructions with <var> <symb> format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_var_symb($xml, $splitted, $order) {
    if (count($splitted) != 3) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, VAR AND SYMB REQUIRED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
    match_var($xml, $splitted[1], 1);
    match_symb($xml, $splitted[2], 2);
}

/**
 * Processing instructions with <var> format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_var($xml, $splitted, $order) {
    if (count($splitted) != 2) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, ONLY VAR REQUIRED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
    match_var($xml, $splitted[1], 1);
}

/**
 * Processing instructions with <label> format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_label($xml, $splitted, $order) {
    if (count($splitted) != 2) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, ONLY LABEL REQUIRED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
    match_label($xml, $splitted[1], 1);
}

/**
 * Processing instructions with <symb> format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_symb($xml, $splitted, $order) {
    if (count($splitted) != 2) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, ONLY SYMB REQUIRED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
    match_symb($xml, $splitted[1], 1);
}

/**
 * Processing instructions with <var> <symb> <symb> format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_var_symb_symb($xml, $splitted, $order) {
    if (count($splitted) != 4) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, VAR, SYMB AND SYMB REQUIRED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
    match_var($xml, $splitted[1], 1);
    match_symb($xml, $splitted[2], 2);
    match_symb($xml, $splitted[3], 3);
}

/**
 * Processing instructions with <var> <type> format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_var_type($xml, $splitted, $order) {
    if (count($splitted) != 3) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, VAR AND TYPE REQUIRED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
    match_var($xml, $splitted[1], 1);
    match_type($xml, $splitted[2], 2);
}

/**
 * Processing instructions with <label> <symb> <symb> format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_label_symb_symb($xml, $splitted, $order) {
    if (count($splitted) != 4) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, LABEL, SYMB AND SYMB REQUIRED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
    match_label($xml, $splitted[1], 1);
    match_symb($xml, $splitted[2], 2);
    match_symb($xml, $splitted[3], 3);
}

/**
 * Processing instructions with no parameters format
 * 
 * @param xml $xml
 * @param string[] $splitted Given arguments
 * @param int $order Order of instruction in the whole program
 */
function case_no_params($xml, $splitted, $order) {
    if (count($splitted) != 1) {
        fprintf(STDERR, "WRONG AMOUNT OF ARGUMENTS, NO NEEDED!\n");
        exit(23);
    }
    xml_print_instr($xml, $splitted[0], $order);
}

///////////////
// main body //
///////////////

parse_args($argc, $argv);
$xml = new XMLWriter();
$xml->openMemory();
xml_start($xml);

$order = 0;
$header = false;

while ($line = fgets(STDIN)) {
    // delete comments
    $line = preg_replace('/#.*$/', '', $line);

    // empty row
    if (preg_match('/^\s*$/', $line)) {
        continue;
    }

    // remove whitespaces from the beginning and the end
    $line = trim($line);

    // split by whitespaces
    $splitted = preg_split('/\s+/', $line);

    // header check
    if (!$header) {
        if (strtoupper($splitted[0]) == ".IPPCODE23" && count($splitted) == 1) {
            $header = true;
        } else {
            fprintf(STDERR, "NO HEADER OR INVALID FORMAT OF HEADER!\n");
            exit(21);
        }
    } else {
        $order++;
        switch(strtoupper($splitted[0]))
        {
            // <var>, <symb>
            case 'MOVE':
            case 'NOT':
            case 'INT2CHAR':
            case 'STRLEN':
            case 'TYPE':
                case_var_symb($xml, $splitted, $order);
                break;
            // <var>
            case 'DEFVAR':
            case 'POPS':
                case_var($xml, $splitted, $order);
                break;
            // <label>
            case 'CALL':
            case 'LABEL':
            case 'JUMP':
                case_label($xml, $splitted, $order);
                break;
            // <symb>
            case 'PUSHS':
            case 'WRITE':
            case 'EXIT':
            case 'DPRINT':
                case_symb($xml, $splitted, $order);
                break;
            // <var> <symb> <symb>
            case 'ADD':
            case 'SUB':
            case 'MUL':
            case 'IDIV':
            case 'LT':
            case 'GT':
            case 'EQ':
            case 'AND':
            case 'OR':
            case 'STRI2INT':
            case 'CONCAT':
            case 'GETCHAR':
            case 'SETCHAR':
                case_var_symb_symb($xml, $splitted, $order);
                break;
            // <var> <type>
            case 'READ':
                case_var_type($xml, $splitted, $order);
                break;
            // <label> <symb> <symb>
            case 'JUMPIFEQ':
            case 'JUMPIFNEQ':
                case_label_symb_symb($xml, $splitted, $order);
                break;
            // no parameters
            case 'CREATEFRAME':
            case 'PUSHFRAME':
            case 'POPFRAME':
            case 'RETURN':
            case 'BREAK':
                case_no_params($xml, $splitted, $order);
                break;
            default:
                fprintf(STDERR, "INVALID INSTRUCTION!\n");
                exit(22);
        }
        $xml->endElement();
    }

}

xml_stop($xml);

?>