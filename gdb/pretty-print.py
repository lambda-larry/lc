import gdb
import gdb.printing
import gdb.types

pp = gdb.printing.RegexpCollectionPrettyPrinter('main')

class lc_sv(gdb.ValuePrinter):
    def __init__(self, val):
        self.__val = val

    def display_hint(self):
        return 'string'

    def to_string(self):
        len = int(self.__val['length'])
        return self.__val['s'].string(length=len)

pp.add_printer('lc_sv', '^lc_sv$', lc_sv)

gdb.printing.register_pretty_printer(gdb.current_objfile(), pp, replace=True)
