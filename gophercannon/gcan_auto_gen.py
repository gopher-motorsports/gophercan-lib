import yaml
import git
from jinja2 import Template
import argparse
import time
import tabulate
import munch
import os
from json2html import *
from functools import partial
import ntpath
import gophercannon_lib
from gophercannon_lib import Module, Bus, Message

parser = argparse.ArgumentParser(prog='gophercannon.py',
    description='Program to generate GopherCAN configuration files and perform static network validation.')

parser.add_argument('network_file',
                    metavar='network file',
                    type=str,
                    help='the network description file')

parser.add_argument('-n',
                    '--narc',
                    required=False,
                    action='store_true',
                    help='enforce strict rules compliance')

parser.add_argument('-t', 
                    '--transient',
                    required=False,
                    const=0,
                    nargs='?',
                    default=0,
                    type=int,
                    help='conduct transient analysis of bus traffic')

parser.add_argument('-s', 
                    '--steady_state',
                    required=False,
                    action='store_true',
                    help='conduct steady-state analysis of bus traffic')

parser.add_argument('-r', 
                    '--reports',
                    required=False,
                    action='store_true',
                    help='generate reports')

parser.add_argument('-d', 
                    '--dry_run',
                    required=False,
                    action='store_true',
                    help='don\'t generate header files')

parser.add_argument('-f', 
                    '--fifo',
                    required=False,
                    action='store_true',
                    help='use fifo mailboxes for transient simulation')

parser.add_argument('-w', 
                    '--wobble',
                    required=False,
                    const=0,
                    nargs='?',
                    default=0,
                    type=int,
                    help='simulate random wobble in the enqueuing of messages to a module\'s tx buffer')

args = parser.parse_args()

filename = args.network_file

HASH_LENGTH = 16 # Use as many hexits as will fit in a single message

repo = git.Repo(search_parent_directories=True)
repo_hash = repo.head.object.hexsha[0:HASH_LENGTH]

print("Welcome to GopherCANnon.")

print("Loading network definition file \"{0}\".".format(filename))
with open(filename) as can_file:

    pretty_name = ntpath.basename(os.path.splitext(can_file.name)[0])

    # Load network definition file
    raw_vehicle = yaml.full_load(can_file)
    vehicle = munch.Munch(raw_vehicle)
    raw_busses = vehicle.busses
    modules = vehicle.modules
    parameters = vehicle.parameters
    commands = vehicle.commands
    errors = vehicle.errors

    steady_usage = 0
    transient_usage = 0
    message_table = []

    print("Network definition file \"{0}\" loaded.".format(filename))

    if not args.dry_run:
        os.makedirs('outputs', exist_ok=True)
        # Generate common header file
        print("\nGenerating common header file.")
        filename = 'GopherCAN_ids_TEMPLATE.h.jinja2'
        with open(os.path.join('templates', filename)) as file_:
            template = Template(file_.read())
            
            output = template.render(modules=modules.values(), 
                                    repo_hash=repo_hash,
                                    parameters=parameters.values(),
                                    commands=commands.values(),
                                    errors=errors.values())
            filename = "GopherCAN_ids.h"
            with open(os.path.join('outputs', filename), "w") as fh:
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
                                    errors=errors.values())
            filename = "GopherCAN_ids.c"
            with open(os.path.join('outputs', filename), "w") as fh:
                fh.write(output)
                fh.write('\n')

        
