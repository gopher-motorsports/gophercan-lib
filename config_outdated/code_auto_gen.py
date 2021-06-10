# code_auto_gen.py
#  this script is used to generate the auto-generated section of GopherCAN.c and GopherCAN.h


import csv

types = ('COMMAND', 'U8', 'U16', 'U32', 'U64', 'S8', 'S16', 'S32', 'S64', 'FLOAT')
macros = ('COMMAND', 'UNSIGNED8', 'UNSIGNED16', 'UNSIGNED32', 'UNSIGNED64', 'SIGNED8', 'SIGNED16', 'SIGNED32', 'SIGNED64', 'FLOATING')

# function to convert the type found in GopherCAN_parameters.csv to the macro used in GopherCAN.c
def convert_type_to_macro(t):
  return macros[types.index(t)]

module_list = []
parameter_list = []
command_list = []
error_list = []

# GopherCAN.h strings
module_ids = '// module IDs:\ntypedef enum\n{\n'
parameter_ids = '// parameter IDs:\ntypedef enum\n{\n'
command_ids = '// custom command IDs:\ntypedef enum\n{\n'
error_ids = '// error IDs:\n'

# GopherCAN.c strings
global_structs = '// all of the global parameter structs\n'
all_parameter_structs = '// this is the struct that will be used to reference based on ID\nvoid* all_parameter_structs[NUM_OF_PARAMETERS] =\n{\n'
parameter_data_types = '// this stores the data_type for each parameter, referenced by ID\nU8 parameter_data_types[NUM_OF_PARAMETERS] =\n{\n'
module_bus_number = '// if there are multiple busses, this shows which bus they are on\n#if MULTI_BUS == TRUE\nU8 module_bus_number[NUM_OF_MODULES] =\n{\n'

# get 2D arrays of each CSV file and put them in the correct list
with open('GopherCAN_modules.csv', newline = '\n') as csvfile:
  reader = csv.reader(csvfile, delimiter = ',', quotechar = '\"')
  for row in reader:
    module_list = module_list + [row]

with open('GopherCAN_parameters.csv', newline = '\n') as csvfile:
  reader = csv.reader(csvfile, delimiter = ',', quotechar = '\"')
  for row in reader:
    parameter_list = parameter_list + [row]

with open('GopherCAN_commands.csv', newline = '\n') as csvfile:
  reader = csv.reader(csvfile, delimiter = ',', quotechar = '\"')
  for row in reader:
    command_list = command_list + [row]
    
with open('GopherCAN_errors.csv', newline = '\n') as csvfile:
  reader = csv.reader(csvfile, delimiter = ',', quotechar = '\"')
  for row in reader:
    error_list = error_list + [row]
    
# delete the first two rows in each lsit, they are just notes
module_list.pop(0)
module_list.pop(0)

parameter_list.pop(0)
parameter_list.pop(0)

command_list.pop(0)
command_list.pop(0)

error_list.pop(0)
error_list.pop(0)

# build strings of what each auto-gen section of the documents are

# module_ids section
for row in module_list:
  comma = ','
  # each row is a list of a specific modules details. The first element is not needed
  row.pop(0)
  
  # if this is the last row, do not add a comma. If it is not, a comma is needed
  if row == module_list[len(module_list) - 1]:
    comma = ''
    
  module_ids = module_ids + '\t' + row[0] + '_ID = ' + row[1] + comma + '\t\t// ' + row[3] + '\n'

# finish up the module_id section
module_ids = module_ids + '} MODULE_ID;\n\n#define NUM_OF_MODULES ' + str(len(module_list)) + '\n\n'

# parameter_ids section
for row in parameter_list:
  comma = ','
  # each row is a list of a specific parameter details. The first element is not needed
  row.pop(0)
  
  # if this is the last row, do not add a comma. If it is not, a comma is needed
  if row == parameter_list[len(parameter_list) - 1]:
    comma = ''
    
  parameter_ids = parameter_ids + '\t' + row[0] + '_ID = ' + row[1] + comma + '\t\t// ' + row[4] + '\n'

# finish up the parameter_id section
parameter_ids = parameter_ids + '} GCAN_PARAM;\n\n#define NUM_OF_PARAMETERS ' + str(len(parameter_list)) + '\n\n'

# command_ids section
for row in command_list:
  comma = ','
  # each row is a list of a specific command details. The first element is not needed
  row.pop(0)
  
  # if this is the last row, do not add a comma. If it is not, a comma is needed
  if row == command_list[len(command_list) - 1]:
    comma = ''
    
  command_ids = command_ids + '\t' + row[0] + ' = ' + row[1] + comma + '\t\t// ' + row[3] + '\n'

# finish up the command_id section
command_ids = command_ids + '} GCAN_COMMAND;\n\n#define NUM_OF_COMMANDS ' + str(len(command_list)) + '\n\n'

# error_ids section
for row in error_list:
  # each row is a list of a specific error details. The first element is not needed
  row.pop(0)
  
  # errors are not enums, so they are a bit easier
  error_ids = error_ids + '#define ' + row[0] + ' ' + row[1] + '\t\t// ' + row[2] + '\n'
  
# finish up the command_id section
error_ids = error_ids + '\n'

# global_structs, all_parameter_structs, and parameter_data_types sections
for row in parameter_list:
  comma = ','
  # global_structs
  if row[2] == 'COMMAND':
    global_structs = global_structs + 'CAN_COMMAND_STRUCT ' + row[0] + ';\t\t// ' + row[4] + '\n'
    
  else:
    global_structs = global_structs + row[2] + '_CAN_STRUCT ' + row[0] + ';\t\t// ' + row[4] + '\n'
    
  # test if this is the last row
  if row == parameter_list[len(parameter_list) - 1]:
    comma = ''
    
  # all_parameter_structs section
  all_parameter_structs = all_parameter_structs + '\t&' + row[0] + comma + '\t\t// ' + row[4] + '\n'
  
  # parameter_data_types section
  parameter_data_types = parameter_data_types + '\t' + convert_type_to_macro(row[2]) + comma + '\n'
  
# finishing up
global_structs = global_structs + '\n'
all_parameter_structs = all_parameter_structs + '};\n\n'
parameter_data_types = parameter_data_types + '};\n\n'

# module_bus_number section
for row in module_list:
  comma = ','
  
  # if this is the last row, do not add a comma. If it is not, a comma is needed
  if row == module_list[len(module_list) - 1]:
    comma = ''
    
  module_bus_number = module_bus_number + '\t' + row[2] + comma + '\t\t// ' + row[3] + '\n'

# finish up the module_bus_number section
module_bus_number = module_bus_number + '};\n#endif\n'


print(global_structs)
print(module_ids)
print(parameter_ids)
print(command_ids)
print(error_ids)
print(all_parameter_structs)
print(parameter_data_types)
print(module_bus_number)


# End of code_auto_gen.py