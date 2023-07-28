"""
Autor: xshevc01
"""
import re
import sys
import xml.etree.ElementTree as ET


def error_message(message, code):
    """
    Prints error message to stderr and exits program
    :param message: given message
    :param code: exit code
    """
    print(message, file=sys.stderr)
    sys.exit(code)


def exit_if_none(arg):
    """
    Help function to exit program if an instruction has few arguments
    :param arg: arg to compare with None
    """
    if arg is None:
        error_message("ERROR: not enough arguments!", 32)
    return arg


def is_var(arg):
    """
    Checks if arg is variable
    :param arg: given argument
    :return: True if success, False otherwise
    """
    if arg.attrib.get('type') == "var":
        return True
    else:
        return False


def is_type(arg):
    """
    Checks if arg is type
    :param arg: given argument
    :return: True if success, False otherwise
    """
    if arg.attrib.get('type') == "type":
        return True
    else:
        return False


def is_label(arg):
    """
    Checks if arg is label
    :param arg: given argument
    :return: True if success, False otherwise
    """
    if arg.attrib.get('type') == "label":
        return True
    else:
        return False


def is_const(arg):
    """
    Checks if arg is constant
    :param arg: given argument
    :return: True if success, False otherwise
    """
    tp = arg.attrib.get('type')
    if tp == "string" or tp == "bool" or tp == "nil" or tp == "int" or tp == 'float':
        return True
    else:
        return False


def none_to_empstr(obj):
    """
    Converts None to empty string
    :param obj: given object
    :return: obj
    """
    if obj is None:
        obj = ""
    return obj


def converter(my_string):
    """
    Converts string given in IPPcode23 to normal string
    :param my_string: given string
    :return: converted string
    """
    try:
        my_string = my_string.replace("&lt;", "<")
        my_string = my_string.replace("&gt;", ">")
        my_string = my_string.replace("&amp;", "&")
        my_string = my_string.replace("&quot;", "\"")
        my_string = my_string.replace("&apos;", "\'")

        pattern = re.compile(r'\\(\d{3})')
        matches = pattern.findall(my_string)

        for match in matches:
            decimal = int(match)
            char = chr(decimal)
            my_string = my_string.replace('\\' + match, char)

    except:
        return my_string
    return my_string


class Frame:
    """
    Class representing frames in symtable
    """

    def __init__(self, tp):
        self.type = tp
        self.items = {}
        self.len = 0


class Symtable:
    """
    Class representing symtable
    """

    def __init__(self):
        self.TF = None
        self.GF = Frame("GF")
        self.frame_stack = []

    def pop_frame(self):
        """
        Pops frame from frame stack and makes it TF
        """
        if len(self.frame_stack) == 0:
            error_message("ERROR: nonexistent LF!", 55)

        popped = self.frame_stack.pop()
        popped.type = "TF"
        self.TF = popped

    def create_frame(self):
        """
        Creates new TF
        """
        self.TF = Frame("TF")

    def push_frame(self):
        """
        Pushes TF frame to frame stack
        """
        if self.TF is None:
            error_message("ERROR: nonexistent TF!", 55)

        self.TF.type = "LF"
        self.frame_stack.append(self.TF)
        self.TF = None

    def find_item(self, item):
        """
        Finds an item in symtable
        :param item: given item
        :return: True if success, False otherwise
        """
        splitted = item.split("@")
        frame = splitted[0]
        var = splitted[1]
        if frame == "GF":
            try:
                return self.GF.items[var]
            except:
                return False
        elif frame == "LF":
            if len(self.frame_stack) == 0:
                error_message("ERROR: nonexistent LF!", 55)

            try:
                return self.frame_stack[len(self.frame_stack) - 1].items[var]
            except:
                return False
        elif frame == "TF":
            if self.TF is None:
                error_message("ERROR: nonexistent TF!", 55)

            try:
                return self.TF.items[var]
            except:
                return False
        else:
            error_message("ERROR: nonexistent frame!", 55)

    def update_item(self, item, new_value, new_data_type):
        """
        Updates item in symtable
        :param item: given item
        :param new_value: new value of item
        :param new_data_type: new data type of item
        """
        splitted = item.split("@")
        frame = splitted[0]
        var = splitted[1]
        if frame == "TF":
            self.TF.items[var] = [new_value, new_data_type]
        elif frame == "GF":
            self.GF.items[var] = [new_value, new_data_type]
        else:
            self.frame_stack[len(self.frame_stack) - 1].items[var] = [new_value, new_data_type]

    def append_item(self, item):
        """
        Appends new item to symtable
        :param item: given item
        """
        splitted = item.split("@")
        frame = splitted[0]
        var = splitted[1]
        if not self.find_item(item):
            if frame == "GF":
                self.GF.items[var] = ["", None]
                self.GF.len += 1
            elif frame == "LF":
                self.frame_stack[len(self.frame_stack) - 1].items[var] = ["", None]
                self.frame_stack[len(self.frame_stack) - 1].len += 1
            else:
                self.TF.items[var] = ["", None]
                self.TF.len += 1

        else:
            error_message("ERROR: redefinition of variable!", 52)


class Instruction:
    """
    Class representing instruction
    """

    def __init__(self, opcode, interpreter, instruction):
        self.opcode = opcode
        self.interpreter = interpreter
        self.instruction = instruction
        self.stack_instr = False

    def run_instruction(self, cnt):
        """
        Runs instruction, not depending on opcode so far
        :param cnt: position in code
        :return: particular instruction to run for each opcode
        """
        opc = self.opcode.lower()
        if opc[-1] == 's':
            self.stack_instr = True
            if opc != 'pops' and opc != 'pushs' and opc != 'clears':
                opc = opc[:-1]

        instruction_to_run = getattr(self, f"run_{opc}", None)
        if instruction_to_run is None:
            error_message("ERROR: bad operation code!", 32)
        return instruction_to_run(cnt)

    def var_and_find(self, arg):
        """
        Checks if arg is variable and exists in symtable
        :param arg: given argument
        """
        if not is_var(self.instruction.find(arg)):
            error_message("ERROR: bad operand type!", 53)
        else:
            if not self.interpreter.symtable.find_item(self.instruction.find(arg).text):
                error_message("ERROR: nonexistent variable!", 54)

    def check_too_much_args(self, arg):
        """
        Help function to exit program if an instruction has too many arguments
        :param arg: given argument
        """
        if self.instruction.find(arg) is not None:
            error_message("ERROR: too many arguments!", 32)

    def run_move(self, cnt):
        """
        Runs MOVE instruction (all functions below also run some instruction, so there is no comment)
        :param cnt: position in code
        :return: cnt
        """
        self.check_too_much_args('arg3')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        arg2 = exit_if_none(self.instruction.find('arg2'))
        if is_var(arg1):
            var = arg1.text
            if not self.interpreter.symtable.find_item(var):
                error_message("ERROR: nonexistent variable!", 54)
        else:
            error_message("ERROR: bad operand type!", 53)

        symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
        if symb_data_type2 is None:
            error_message("ERROR: empty variable!", 56)
        self.interpreter.symtable.update_item(arg1.text, symb_value2, symb_data_type2)
        return cnt

    def run_createframe(self, cnt):
        self.check_too_much_args('arg1')

        self.interpreter.symtable.create_frame()
        return cnt

    def run_pushframe(self, cnt):
        self.check_too_much_args('arg1')

        self.interpreter.symtable.push_frame()
        return cnt

    def run_popframe(self, cnt):
        self.check_too_much_args('arg1')

        self.interpreter.symtable.pop_frame()
        return cnt

    def run_defvar(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        if is_var(arg1):
            var = arg1.text
            self.interpreter.symtable.append_item(var)
        else:
            error_message("ERROR: bad operand type!", 53)
        return cnt

    def run_call(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        self.interpreter.stack.append([cnt, 'int'])
        if is_label(arg1):
            try:
                cnt = int(self.interpreter.array_of_labels[arg1.text])
            except:
                error_message("ERROR: undefined label call!", 52)
        else:
            error_message("ERROR: call without valid label!", 53)
        return cnt

    def run_return(self, cnt):
        self.check_too_much_args('arg1')

        if len(self.interpreter.stack) > 0:
            popped = self.interpreter.stack.pop()
            if popped[1] != 'int':
                error_message("ERROR: not int value in return!", 57)
            try:
                cnt = int(popped[0])
            except:
                error_message("ERROR: bad XML structure int conversion!", 32)
        else:
            error_message("ERROR: empty stack!", 56)
        return cnt

    def run_pushs(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        symb_value, symb_data_type = self.interpreter.get_symb(arg1)
        symb_value = none_to_empstr(symb_value)

        if symb_data_type is None:
            error_message("ERROR: empty variable!", 56)

        self.interpreter.stack.append([symb_value, symb_data_type])
        return cnt

    def run_pops(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        if len(self.interpreter.stack) > 0:
            if is_var(arg1):
                if not self.interpreter.symtable.find_item(arg1.text):
                    error_message("ERROR: nonexistent variable!", 54)
                popped = self.interpreter.stack.pop()
                self.interpreter.symtable.update_item(arg1.text, popped[0], popped[1])
            else:
                error_message("ERROR: bad operand type!", 53)
        else:
            error_message("ERROR: empty stack!", 56)
        return cnt

    def run_clears(self, cnt):
        self.interpreter.stack.clear()
        return cnt

    def run_add(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if not (
                symb_data_type2 == 'int' and symb_data_type3 == 'int' or symb_data_type2 == 'float' and symb_data_type3 == 'float'):
            error_message("ERROR: bad operand type!", 53)

        if symb_data_type2 == 'int':
            try:
                summ = int(symb_value2) + int(symb_value3)
            except:
                error_message("ERROR: bad XML structure int conversion!", 32)

        if symb_data_type2 == 'float':
            try:
                summ = (float.fromhex(symb_value2) + float.fromhex(symb_value3)).hex()
            except:
                error_message("ERROR: bad XML structure float conversion!", 32)

        if self.stack_instr:
            self.interpreter.stack.append([str(summ), symb_data_type2])
        else:
            self.interpreter.symtable.update_item(arg1.text, str(summ), symb_data_type2)
        return cnt

    def run_sub(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if not (
                symb_data_type2 == 'int' and symb_data_type3 == 'int' or symb_data_type2 == 'float' and symb_data_type3 == 'float'):
            error_message("ERROR: bad operand type!", 53)

        if symb_data_type2 == 'int':
            try:
                diff = int(symb_value2) - int(symb_value3)
            except:
                error_message("ERROR: bad XML structure int conversion!", 32)

        if symb_data_type2 == 'float':
            try:
                diff = (float.fromhex(symb_value2) - float.fromhex(symb_value3)).hex()
            except:
                error_message("ERROR: bad XML structure float conversion!", 32)

        if self.stack_instr:
            self.interpreter.stack.append([str(diff), symb_data_type2])
        else:
            self.interpreter.symtable.update_item(arg1.text, str(diff), symb_data_type2)
        return cnt

    def run_mul(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if not (
                symb_data_type2 == 'int' and symb_data_type3 == 'int' or symb_data_type2 == 'float' and symb_data_type3 == 'float'):
            error_message("ERROR: bad operand type!", 53)

        if symb_data_type2 == 'int':
            try:
                prod = int(symb_value2) * int(symb_value3)
            except:
                error_message("ERROR: bad XML structure int conversion!", 32)

        if symb_data_type2 == 'float':
            try:
                prod = (float.fromhex(symb_value2) * float.fromhex(symb_value3)).hex()
            except:
                error_message("ERROR: bad XML structure float conversion!", 32)

        if self.stack_instr:
            self.interpreter.stack.append([str(prod), symb_data_type2])
        else:
            self.interpreter.symtable.update_item(arg1.text, str(prod), symb_data_type2)
        return cnt

    def run_div(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'float' or symb_data_type3 != 'float':
            error_message("ERROR: bad operand type!", 53)

        if float.fromhex(symb_value3) == 0:
            error_message("ERROR: division by 0!", 57)

        try:
            quot = (float.fromhex(symb_value2) / float.fromhex(symb_value3)).hex()
        except:
            error_message("ERROR: bad XML structure float conversion!", 32)

        if self.stack_instr:
            self.interpreter.stack.append([str(quot), 'float'])
        else:
            self.interpreter.symtable.update_item(arg1.text, str(quot), 'float')
        return cnt

    def run_idiv(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'int' or symb_data_type3 != 'int':
            error_message("ERROR: bad operand type!", 53)

        if symb_value3 == '0':
            error_message("ERROR: division by 0!", 57)

        try:
            quot = int(symb_value2) // int(symb_value3)
        except:
            error_message("ERROR: bad XML structure int conversion!", 32)

        if self.stack_instr:
            self.interpreter.stack.append([str(quot), 'int'])
        else:
            self.interpreter.symtable.update_item(arg1.text, str(quot), 'int')
        return cnt

    def run_lt(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != symb_data_type3 or symb_data_type2 == 'nil' or symb_data_type3 == 'nil':
            error_message("ERROR: not same operands types!", 53)

        if symb_data_type2 == 'string' and symb_data_type3 == 'string':
            symb_value2 = converter(symb_value2)
            symb_value3 = converter(symb_value3)

        if symb_data_type2 == 'int':
            try:
                int(symb_value2)
                int(symb_value3)
            except:
                error_message("ERROR: not int given!", 57)
            if int(symb_value2) < int(symb_value3):
                less = 'true'
            else:
                less = 'false'
        elif symb_data_type2 == 'float':
            try:
                float.fromhex(symb_value2)
                float.fromhex(symb_value3)
            except:
                error_message("ERROR: not float given!", 57)
            if float.fromhex(symb_value2) < float.fromhex(symb_value3):
                less = 'true'
            else:
                less = 'false'
        else:
            if symb_value2 < symb_value3:
                less = 'true'
            else:
                less = 'false'

        if self.stack_instr:
            self.interpreter.stack.append([less, 'bool'])
        else:
            self.interpreter.symtable.update_item(arg1.text, less, 'bool')
        return cnt

    def run_gt(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != symb_data_type3 or symb_data_type2 == 'nil' or symb_data_type3 == 'nil':
            error_message("ERROR: not same operands types!", 53)

        if symb_data_type2 == 'string' and symb_data_type3 == 'string':
            symb_value2 = converter(symb_value2)
            symb_value3 = converter(symb_value3)

        if symb_data_type2 == 'int':
            try:
                int(symb_value2)
                int(symb_value3)
            except:
                error_message("ERROR: not int given!", 57)
            if int(symb_value2) > int(symb_value3):
                greater = 'true'
            else:
                greater = 'false'
        elif symb_data_type2 == 'float':
            try:
                float.fromhex(symb_value2)
                float.fromhex(symb_value3)
            except:
                error_message("ERROR: not float given!", 57)
            if float.fromhex(symb_value2) > float.fromhex(symb_value3):
                greater = 'true'
            else:
                greater = 'false'
        else:
            if symb_value2 > symb_value3:
                greater = 'true'
            else:
                greater = 'false'

        if self.stack_instr:
            self.interpreter.stack.append([greater, 'bool'])
        else:
            self.interpreter.symtable.update_item(arg1.text, greater, 'bool')
        return cnt

    def run_eq(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != symb_data_type3:
            if symb_data_type2 == 'nil' or symb_data_type3 == 'nil':
                if self.stack_instr:
                    self.interpreter.stack.append(['false', 'bool'])
                else:
                    self.interpreter.symtable.update_item(arg1.text, 'false', 'bool')
                return cnt
            error_message("ERROR: not same operands types!", 53)

        if symb_data_type2 == 'string' and symb_data_type3 == 'string':
            symb_value2 = converter(symb_value2)
            symb_value3 = converter(symb_value3)

        if symb_data_type2 == 'int':
            try:
                int(symb_value2)
                int(symb_value3)
            except:
                error_message("ERROR: not int given!", 57)
            if int(symb_value2) == int(symb_value3):
                equals = 'true'
            else:
                equals = 'false'
        elif symb_data_type2 == 'nil':
            equals = 'true'
        elif symb_data_type2 == 'float':
            try:
                float.fromhex(symb_value2)
                float.fromhex(symb_value3)
            except:
                error_message("ERROR: not float given!", 57)
            if float.fromhex(symb_value2) == float.fromhex(symb_value3):
                equals = 'true'
            else:
                equals = 'false'
        else:
            if symb_value2 == symb_value3:
                equals = 'true'
            else:
                equals = 'false'

        if self.stack_instr:
            self.interpreter.stack.append([equals, 'bool'])
        else:
            self.interpreter.symtable.update_item(arg1.text, equals, 'bool')
        return cnt

    def run_and(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'bool' or symb_data_type3 != 'bool':
            error_message("ERROR: not bool operands types!", 53)

        result = 'false'
        if symb_value2 == 'true' and symb_value3 == 'true':
            result = 'true'

        if self.stack_instr:
            self.interpreter.stack.append([result, 'bool'])
        else:
            self.interpreter.symtable.update_item(arg1.text, result, 'bool')
        return cnt

    def run_or(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'bool' or symb_data_type3 != 'bool':
            error_message("ERROR: not bool operands types!", 53)

        result = 'false'
        if symb_value2 == 'true' or symb_value3 == 'true':
            result = 'true'

        if self.stack_instr:
            self.interpreter.stack.append([result, 'bool'])
        else:
            self.interpreter.symtable.update_item(arg1.text, result, 'bool')
        return cnt

    def run_not(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg3')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            self.var_and_find('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb(arg2)

        symb_value = none_to_empstr(symb_value)

        if symb_data_type is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type != 'bool':
            error_message("ERROR: not bool operands types!", 53)

        result = 'false'
        if symb_value == 'false':
            result = 'true'

        if self.stack_instr:
            self.interpreter.stack.append([result, 'bool'])
        else:
            self.interpreter.symtable.update_item(arg1.text, result, 'bool')
        return cnt

    def run_int2float(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg3')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            self.var_and_find('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb(arg2)

        if symb_data_type is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type != 'int':
            error_message("ERROR: not int operands types!", 53)

        try:
            number = float(int(symb_value)).hex()
        except:
            error_message("ERROR: not int given!", 57)

        if self.stack_instr:
            self.interpreter.stack.append([number, 'float'])
        else:
            self.interpreter.symtable.update_item(arg1.text, number, 'float')
        return cnt

    def run_float2int(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg3')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            self.var_and_find('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb(arg2)

        if symb_data_type is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type != 'float':
            error_message("ERROR: not int operands types!", 53)

        in_hex = True
        try:
            number = int(float.fromhex(symb_value))
        except:
            in_hex = False
        if not in_hex:
            try:
                number = int(float(symb_value))
            except:
                error_message("ERROR: not float given!", 57)

        if self.stack_instr:
            self.interpreter.stack.append([number, 'int'])
        else:
            self.interpreter.symtable.update_item(arg1.text, number, 'int')
        return cnt

    def run_int2char(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg3')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            self.var_and_find('arg1')

            symb_value, symb_data_type = self.interpreter.get_symb(arg2)

        if symb_data_type is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type != 'int':
            error_message("ERROR: not int operands types!", 53)

        try:
            character = chr(int(symb_value))
        except:
            error_message("ERROR: not int given!", 58)

        if self.stack_instr:
            self.interpreter.stack.append([character, 'string'])
        else:
            self.interpreter.symtable.update_item(arg1.text, character, 'string')
        return cnt

    def run_stri2int(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg1')

            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))
            self.var_and_find('arg1')

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'string' or symb_data_type3 != 'int':
            error_message("ERROR: not same operands types!", 53)

        try:
            symb_value3 = int(symb_value3)
        except:
            error_message("ERROR: bad XML structure int conversion!", 32)

        symb_value2 = converter(symb_value2)

        if symb_value3 > len(symb_value2) - 1 or symb_value3 < 0:
            error_message("ERROR: bad indexing!", 58)

        if self.stack_instr:
            self.interpreter.stack.append([str(ord(symb_value2[symb_value3])), 'int'])
        else:
            self.interpreter.symtable.update_item(arg1.text, str(ord(symb_value2[symb_value3])), 'int')
        return cnt

    def run_read(self, cnt):
        self.check_too_much_args('arg3')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        arg2 = exit_if_none(self.instruction.find('arg2'))
        self.var_and_find('arg1')

        if not is_type(arg2):
            error_message("ERROR: bad operand type!", 53)
        type_value = arg2.text

        value = self.interpreter.input_file.readline()

        if len(value) == 0:
            value = 'nil'
            type_value = 'nil'
            self.interpreter.symtable.update_item(arg1.text, value, type_value)
            return cnt

        value = value.rstrip('\n')
        if len(value) == 0 and type_value != 'string':
            value = 'nil'
            type_value = 'nil'
        elif type_value == 'int':
            try:
                value = int(value)
            except:
                value = 'nil'
                type_value = 'nil'
        elif type_value == 'float':
            in_hex = True
            try:
                value = float.fromhex(value).hex()
            except:
                in_hex = False
            if not in_hex:
                try:
                    value = float(value).hex()
                except:
                    error_message("ERROR: bad float value!", 57)
        elif type_value == 'bool':
            if value.lower() == 'true':
                value = 'true'
            else:
                value = 'false'
        elif type_value == 'string':
            value = converter(value)
        else:
            value = 'nil'
            type_value = 'nil'

        self.interpreter.symtable.update_item(arg1.text, value, type_value)
        return cnt

    def run_write(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))

        symb_value, symb_data_type = self.interpreter.get_symb(arg1)
        symb_value = none_to_empstr(symb_value)

        if symb_data_type is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type == 'string':
            symb_value = converter(symb_value)

        if symb_data_type != 'nil':
            self.interpreter.program_output += str(symb_value)
        return cnt

    def run_concat(self, cnt):
        self.check_too_much_args('arg4')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        arg2 = exit_if_none(self.instruction.find('arg2'))
        arg3 = exit_if_none(self.instruction.find('arg3'))
        self.var_and_find('arg1')

        symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
        symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'string' or symb_data_type3 != 'string':
            error_message("ERROR: not string operands types!", 53)

        self.interpreter.symtable.update_item(arg1.text, symb_value2 + symb_value3, 'string')
        return cnt

    def run_strlen(self, cnt):
        self.check_too_much_args('arg3')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        arg2 = exit_if_none(self.instruction.find('arg2'))
        self.var_and_find('arg1')

        symb_value, symb_data_type = self.interpreter.get_symb(arg2)
        symb_value = none_to_empstr(symb_value)
        symb_value = converter(symb_value)

        if symb_data_type is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type != 'string':
            error_message("ERROR: not string operands types!", 53)

        self.interpreter.symtable.update_item(arg1.text, str(len(symb_value)), 'int')
        return cnt

    def run_getchar(self, cnt):
        self.check_too_much_args('arg4')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        arg2 = exit_if_none(self.instruction.find('arg2'))
        arg3 = exit_if_none(self.instruction.find('arg3'))
        self.var_and_find('arg1')

        symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
        symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'string' or symb_data_type3 != 'int':
            error_message("ERROR: not string or integer operands types!", 53)

        try:
            symb_value3 = int(symb_value3)
        except:
            error_message("ERROR: bad XML structure int conversion!", 32)

        symb_value2 = converter(symb_value2)

        if symb_value3 > len(symb_value2) - 1 or symb_value3 < 0:
            error_message("ERROR: bad indexing!", 58)

        self.interpreter.symtable.update_item(arg1.text, symb_value2[symb_value3], 'string')
        return cnt

    def run_setchar(self, cnt):
        self.check_too_much_args('arg4')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        arg2 = exit_if_none(self.instruction.find('arg2'))
        arg3 = exit_if_none(self.instruction.find('arg3'))
        self.var_and_find('arg1')

        var = self.interpreter.symtable.find_item(arg1.text)
        var_data_type = var[1]
        var_value = var[0]

        if not var_data_type == "string" and not var_data_type is None:
            error_message("ERROR: bad operand type!", 53)

        if var_data_type is None:
            error_message("ERROR: empty variable!", 56)

        symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
        symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        symb_value2 = none_to_empstr(symb_value2)

        if symb_value3 is None:
            error_message("ERROR: empty string!", 58)

        if var_data_type is None or symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != 'int' or symb_data_type3 != 'string':
            error_message("ERROR: not string or integer operands types!", 53)

        try:
            symb_value2 = int(symb_value2)
        except:
            error_message("ERROR: bad XML structure int conversion!", 32)

        var_value = converter(var_value)
        symb_value3 = converter(symb_value3)

        if symb_value2 > len(var_value) - 1 or symb_value2 < 0:
            error_message("ERROR: bad indexing!", 58)

        var_value = var_value[:symb_value2] + symb_value3[0] + var_value[symb_value2 + 1:]

        self.interpreter.symtable.update_item(arg1.text, var_value, 'string')
        return cnt

    def run_type(self, cnt):
        self.check_too_much_args('arg3')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        arg2 = exit_if_none(self.instruction.find('arg2'))
        self.var_and_find('arg1')

        symb_value, symb_data_type = self.interpreter.get_symb(arg2)

        if symb_data_type is None:
            self.interpreter.symtable.update_item(arg1.text, '', 'string')
        else:
            self.interpreter.symtable.update_item(arg1.text, symb_data_type, 'string')
        return cnt

    def run_label(self, cnt):
        self.check_too_much_args('arg2')
        exit_if_none(self.instruction.find('arg1'))
        return cnt

    def run_jump(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        if is_label(arg1):
            try:
                cnt = int(self.interpreter.array_of_labels[arg1.text])
            except:
                error_message("ERROR: undefined label call!", 52)
        else:
            error_message("ERROR: call without valid label!", 53)
        return cnt

    def run_jumpifeq(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg2')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        if not is_label(arg1):
            error_message("ERROR: call without valid label!", 53)

        try:
            new_position = int(self.interpreter.array_of_labels[arg1.text])
        except:
            error_message("ERROR: undefined label call!", 52)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 == 'string' and symb_data_type3 == 'string':
            symb_value2 = converter(symb_value2)
            symb_value3 = converter(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != symb_data_type3 and symb_data_type2 != 'nil' and symb_data_type3 != 'nil':
            error_message("ERROR: not same type or nil operands type!", 53)

        if symb_data_type2 == 'float' and symb_data_type3 == 'float':
            in_hex2 = True
            try:
                symb_value2 = float.fromhex(symb_value2).hex()
            except:
                in_hex2 = False
            if not in_hex2:
                try:
                    symb_value2 = float(symb_value2).hex()
                except:
                    error_message("ERROR: bad float value!", 57)
            in_hex3 = True
            try:
                symb_value3 = float.fromhex(symb_value3).hex()
            except:
                in_hex3 = False
            if not in_hex3:
                try:
                    symb_value3 = float(symb_value3).hex()
                except:
                    error_message("ERROR: bad float value!", 57)

        if symb_value2 == symb_value3:
            cnt = new_position
        return cnt

    def run_jumpifneq(self, cnt):
        if self.stack_instr:
            self.check_too_much_args('arg2')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            symb_value3, symb_data_type3 = self.interpreter.get_symb_stack()
            symb_value2, symb_data_type2 = self.interpreter.get_symb_stack()
        else:
            self.check_too_much_args('arg4')

            arg1 = exit_if_none(self.instruction.find('arg1'))
            arg2 = exit_if_none(self.instruction.find('arg2'))
            arg3 = exit_if_none(self.instruction.find('arg3'))

            symb_value2, symb_data_type2 = self.interpreter.get_symb(arg2)
            symb_value3, symb_data_type3 = self.interpreter.get_symb(arg3)

        if not is_label(arg1):
            error_message("ERROR: call without valid label!", 53)

        try:
            new_position = int(self.interpreter.array_of_labels[arg1.text])
        except:
            error_message("ERROR: undefined label call!", 52)

        symb_value2 = none_to_empstr(symb_value2)
        symb_value3 = none_to_empstr(symb_value3)

        if symb_data_type2 == 'string' and symb_data_type3 == 'string':
            symb_value2 = converter(symb_value2)
            symb_value3 = converter(symb_value3)

        if symb_data_type2 is None or symb_data_type3 is None:
            error_message("ERROR: empty variable!", 56)

        if symb_data_type2 != symb_data_type3 and symb_data_type2 != 'nil' and symb_data_type3 != 'nil':
            error_message("ERROR: not same type or nil operands type!", 53)

        if symb_data_type2 == 'float' and symb_data_type3 == 'float':
            in_hex2 = True
            try:
                symb_value2 = float.fromhex(symb_value2).hex()
            except:
                in_hex2 = False
            if not in_hex2:
                try:
                    symb_value2 = float(symb_value2).hex()
                except:
                    error_message("ERROR: bad float value!", 57)
            in_hex3 = True
            try:
                symb_value3 = float.fromhex(symb_value3).hex()
            except:
                in_hex3 = False
            if not in_hex3:
                try:
                    symb_value3 = float(symb_value3).hex()
                except:
                    error_message("ERROR: bad float value!", 57)

        if symb_value2 != symb_value3:
            cnt = new_position
        return cnt

    def run_exit(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        symb_value, symb_data_type = self.interpreter.get_symb(arg1)
        if symb_value == "":
            error_message("ERROR: no valid symbol!", 56)

        try:
            symb_value = int(symb_value)
        except:
            error_message("ERROR: bad XML structure int conversion!", 53)
        if symb_value < 0 or symb_value > 49:
            error_message("ERROR: bad exit value!", 57)
        else:
            print(self.interpreter.program_output, end='')
            sys.exit(symb_value)

    def run_dprint(self, cnt):
        self.check_too_much_args('arg2')

        arg1 = exit_if_none(self.instruction.find('arg1'))
        symb_value, symb_data_type = self.interpreter.get_symb(arg1)
        symb_value = none_to_empstr(symb_value)
        print(symb_value, file=sys.stderr)
        return cnt

    def run_break(self, cnt):
        self.check_too_much_args('arg1')

        print(
            f'Position in code: {cnt}\nContent of frame: {self.interpreter.stack}\nNumber of executed instructions: {self.interpreter.number_of_instructions}',
            file=sys.stderr)
        return cnt


class Interpreter:
    """
    Class representing interpreter
    """

    def __init__(self, array_of_labels, input_file):
        self.symtable = Symtable()
        self.array_of_labels = array_of_labels
        self.stack = []
        self.number_of_instructions = 0
        self.input_file = input_file
        self.program_output = ""
        if self.input_file != sys.stdin:
            try:
                self.input_file = open(self.input_file, "r")
            except:
                error_message("ERROR: invalid file name or unable to open input file for reading!", 11)

    def get_symb_stack(self):
        """
        Gets symbol from data stack
        :return: item value, item data type
        """
        if len(self.stack) > 0:
            item = self.stack.pop()
            return item[0], item[1]
        else:
            error_message("ERROR: no value on stack!", 56)

    def get_symb(self, arg):
        """
        Gets symbol from variable in symtable or from given arg if is constant
        :param arg: given argument
        :return: symbol value, symbol data type
        """
        if is_var(arg):
            symb = arg.text
            item = self.symtable.find_item(symb)

            if item:
                symb_data_type = item[1]
                symb_value = item[0]
            else:
                error_message("ERROR: nonexistent variable!", 54)

        elif is_const(arg):
            symb_value = arg.text
            if arg.attrib.get('type') == 'float':
                in_hex = True
                try:
                    symb_value = float.fromhex(symb_value).hex()
                except:
                    in_hex = False
                if not in_hex:
                    try:
                        symb_value = float(symb_value).hex()
                    except:
                        error_message("ERROR: bad float value!", 57)
            symb_data_type = arg.attrib.get('type')

        else:
            error_message("ERROR: bad operand type!", 53)

        return symb_value, symb_data_type

    def run(self, instruction, i):
        """
        Initiates run process for each instruction
        :param instruction: given instruction
        :param i: position in code
        :return: i
        """
        self.number_of_instructions += 1
        instruction_to_run = Instruction(instruction.attrib.get('opcode'), self, instruction)
        i = instruction_to_run.run_instruction(i)
        return i


class ParserXML:
    """
    Class representing XML parser
    """
    instruction_order = 0

    def __init__(self, source_file):
        try:
            tree = ET.parse(source_file)
        except:
            error_message("ERROR: wrong XML structure!", 31)
        try:
            self.root = tree.getroot()
        except:
            error_message("ERROR: invalid format of XML!", 31)

    def order_checker(self, external_order):
        """
        Checks if orders of instructions are in ascending sequence and without repeating
        :param external_order: order of given instruction
        :return: True if success
        """
        if external_order is None:
            error_message("ERROR: missing instruction order!", 32)
        if self.instruction_order < int(external_order):
            self.instruction_order = int(external_order)
            return True
        else:
            error_message("ERROR: invalid instruction order!", 32)

    def is_XML_correct(self):
        """
        Checks correct XML structure
        """
        if self.root.tag != "program" or self.root.attrib.get('language') != "IPPcode23":
            error_message("ERROR: invalid XML program attribute!", 32)


def parse_args():
    """
    Parses given arguments
    :return: source file, input file
    """
    help_message = "\n\t\tINTERPRET.PY\n\n" \
                   "This script loads an XML representation of the program and this \n" \
                   "program using the input according to the command line parameters, \n" \
                   "interprets and generates the output.\n\n" \
                   "Usage: py interpret.py [--help] [--source=file] [--input=file]\n\n"

    source_file = None
    input_file = None
    got_help = False
    i = 1
    if len(sys.argv) > 3:
        error_message("ERROR: too much arguments, or --help used with something else!", 10)

    while i < len(sys.argv):
        if sys.argv[i] == '--help':
            got_help = True
            i += 1
        elif sys.argv[i].startswith("--source="):
            source_file = sys.argv[i][len("--source="):]
            if len(source_file) == 0:
                error_message("ERROR: bad source given!", 10)
            i += 1
        elif sys.argv[i].startswith("--input="):
            input_file = sys.argv[i][len("--input="):]
            if len(input_file) == 0:
                error_message("ERROR: bad input given!", 10)
            i += 1
        else:
            error_message("ERROR: given bad argument!", 10)

    if got_help and len(sys.argv) > 2:
        error_message("ERROR: using --help with something else!", 10)

    if got_help:
        print(help_message)
        sys.exit(0)

    if source_file is None and input_file is None:
        error_message("ERROR: source and input files are not given!", 10)

    if source_file is None:
        source_file = sys.stdin

    if input_file is None:
        input_file = sys.stdin

    return source_file, input_file


def sort_by_order(instruction):
    """
    Help function for sorting
    :param instruction: given instruction
    :return: int value of instruction order
    """
    try:
        return int(instruction.attrib.get('order'))
    except:
        error_message("ERROR: not int value in order!", 32)


def program_runner(source_file, input_file):
    """
    Runs the whole program
    :param source_file: source file
    :param input_file: input file
    """
    reader = ParserXML(source_file)
    reader.is_XML_correct()

    list_of_instructions = []
    array_of_labels = {}

    for instruction in reader.root.iter():
        if instruction.tag != 'instruction' and instruction.tag != 'program' and not instruction.tag.startswith('arg'):
            error_message("ERROR: received wrong tag!", 32)
        if instruction.tag == 'instruction':
            if instruction.attrib.get('opcode') is None or instruction.attrib.get('order') is None:
                error_message("ERROR: missing opcode or order!", 32)
            list_of_instructions.append(instruction)

    list_of_instructions.sort(key=sort_by_order)

    cnt = 0
    for instruction in list_of_instructions:
        if not reader.order_checker(instruction.attrib.get('order')):
            error_message("ERROR: missing order!", 32)
        if instruction.attrib.get('opcode').lower() == "label":
            if instruction.find('arg1').text in array_of_labels:
                error_message("ERROR: redefinition of label!", 52)
            else:
                array_of_labels[instruction.find('arg1').text] = cnt
        cnt += 1

    interpret = Interpreter(array_of_labels, input_file)
    i = 0
    while i < len(list_of_instructions):
        i = interpret.run(list_of_instructions[i], i)
        i += 1
    print(interpret.program_output, end='')


if __name__ == '__main__':
    sourceFile, inputFile = parse_args()
    program_runner(sourceFile, inputFile)
