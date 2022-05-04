import yaml
import argparse
import munch
import os
import ntpath
from jinja2 import Template

def check_ids(dict_with_id):
    temp_list = list(dict_with_id.values())
    for index, item1 in enumerate(temp_list):
        for item2 in temp_list[index+1:]:
            if item1['id'] == item2['id']:
                print()
                print("ERROR:", item1['name'], "has the same GCAN ID as", item2['name'])
                print("Exiting...")
                quit()

parser = argparse.ArgumentParser(prog='gophercannon.py',
    description='Program to generate GopherCAN configuration files and perform static network validation.')

parser.add_argument('network_file',
                    metavar='network file',
                    type=str,
                    help='the network description file')

args = parser.parse_args()

filename = args.network_file

HASH_LENGTH = 16 # Use as many hexits as will fit in a single message

#repo = git.Repo(search_parent_directories=True)
#repo_hash = repo.head.object.hexsha[0:HASH_LENGTH]
repo_hash = "REP_HASH_NOT_IMP"

print("Welcome to GopherCANnon.")

print("Loading network definition file \"{0}\".".format(filename))
with open(filename) as can_file:

    pretty_name = ntpath.basename(os.path.splitext(can_file.name)[0])

    # Load network definition file
    raw_vehicle = yaml.full_load(can_file)
    vehicle = munch.Munch(raw_vehicle)
    modules = vehicle.modules
    parameters = vehicle.parameters
    commands = vehicle.commands
    errors = vehicle.errors
    
    # get the yaml string and convert it to a byte string
    ascii_values = []
    concat = ", "
    yamlStr = open(filename)
    yamlStr = yamlStr.read()
    for char in yamlStr:
        ascii_values.append(hex(ord(char)))
    yamlStr = concat.join(ascii_values)

    steady_usage = 0
    transient_usage = 0
    message_table = []

    print("Network definition file \"{0}\" loaded.".format(filename))
    
    # Dictionary to convert the param type to the correct param struct
    type_to_struct = {
        "UNSIGNED8" : "U8_CAN_STRUCT",
        "UNSIGNED16" : "U16_CAN_STRUCT",
        "UNSIGNED32" : "U32_CAN_STRUCT",
        "UNSIGNED64" : "U64_CAN_STRUCT",
        "SIGNED8" : "S8_CAN_STRUCT",
        "SIGNED16" : "S16_CAN_STRUCT",
        "SIGNED32" : "S32_CAN_STRUCT",
        "SIGNED64" : "S64_CAN_STRUCT",
        "FLOATING" : "FLOAT_CAN_STRUCT"
    }
    type_to_name_type = {
        "UNSIGNED8" : "UINT_8",
        "UNSIGNED16" : "UINT_16",
        "UNSIGNED32" : "UINT_32",
        "UNSIGNED64" : "UINT_64",
        "SIGNED8" : "SINT_8",
        "SIGNED16" : "SINT_16",
        "SIGNED32" : "SINT_32",
        "SIGNED64" : "SINT_64",
        "FLOATING" : "FLOAT"
    }
    
    # check to make sure there are no duplicates in the IDs
    check_ids(modules)
    check_ids(parameters)
    check_ids(commands)
    check_ids(errors)

    # Generate common header file
    print("\nGenerating common header file.")
    filename = 'GopherCAN_ids_TEMPLATE.h.jinja2'
    with open(os.path.join('templates', filename)) as file_:
        template = Template(file_.read())
        
        output = template.render(modules=modules.values(), 
                                repo_hash=repo_hash,
                                parameters=parameters.values(),
                                commands=commands.values(),
                                errors=errors.values(),
                                type_struct=type_to_struct)
        filename = "GopherCAN_ids.h"
        with open('../' + filename, "w") as fh:
            fh.write(output)
            fh.write('\n')
            
            
    print("Generating common source file.")
    filename = 'GopherCAN_ids_TEMPLATE.c.jinja2'
    with open(os.path.join('templates', filename)) as file_:
        template = Template(file_.read())
        
        output = template.render(modules=modules.values(), 
                                repo_hash=repo_hash,
                                parameters=parameters.values(),
                                commands=commands.values(),
                                errors=errors.values(),
                                type_struct=type_to_struct,
                                yamlStr=yamlStr)
        filename = "GopherCAN_ids.c"
        with open('../' + filename, "w") as fh:
            fh.write(output)
            fh.write('\n')
            
            
    print("Generating names source file.")
    filename = 'GopherCAN_names_TEMPLATE.c.jinja2'
    with open(os.path.join('templates', filename)) as file_:
        template = Template(file_.read())
        
        output = template.render(parameters=parameters.values(),
                                 type_name=type_to_name_type)
        filename = "GopherCAN_names.c"
        with open('../' + filename, "w") as fh:
            fh.write(output)
            fh.write('\n')
    
    
    print("Generating names header file.")
    filename = 'GopherCAN_names_TEMPLATE.h.jinja2'
    with open(os.path.join('templates', filename)) as file_:
        template = Template(file_.read())
        
        output = template.render(param_count=len(parameters) + 1,
                                 parameters=parameters.values())
        filename = "GopherCAN_names.h"
        with open('../' + filename, "w") as fh:
            fh.write(output)
            fh.write('\n')
