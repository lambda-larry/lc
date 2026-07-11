import gdb
import gdb.printing
import gdb.types

pp = gdb.printing.RegexpCollectionPrettyPrinter('lc')

class lc_sv(gdb.ValuePrinter):
    def __init__(self, val):
        self.__val = val

    def display_hint(self):
        return 'string'

    def to_string(self):
        len = int(self.__val['length'])
        return self.__val['s'].string(length=len)

class lc_rune(gdb.ValuePrinter):
    def __init__(self, val):
        self.__val = val

    def to_string(self):
        codepoint = int(self.__val)
        return f"'{chr(codepoint)}'"


class lc_clock(gdb.ValuePrinter):
    def __init__(self, val):
        self.__val = val

    def to_string(self):
        sec  = int(self.__val['sec'])
        nsec = int(self.__val['nsec'])
        return f"{sec}.{nsec:09}s"



pp.add_printer('lc_sv', '^lc_sv$', lc_sv)
pp.add_printer('lc_rune', '^lc_rune$', lc_rune)
pp.add_printer('lc_clock', '^lc_clock$', lc_clock)


gdb.printing.register_pretty_printer(gdb.current_objfile(), pp, replace=True)
