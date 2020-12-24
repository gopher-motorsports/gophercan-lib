#!./venv/bin/python
# The shebang on the previous line should be removed if you're using a different interpreter
import yaml
import git
import munch
from jinja2 import Template
import argparse

my_parser = argparse.ArgumentParser(prog='gophercannon.py',
    description='Program to generate GopherCAN configuration files and perform static network validation.')

my_parser.add_argument('network_file',
                       metavar='network file',
                       type=str,
                       help='the network description file')

args = my_parser.parse_args()

filename = args.network_file

hash_length = 16 # Use as many hexits as will fit in a single message

repo = git.Repo(search_parent_directories=True)
repo_hash = repo.head.object.hexsha[0:hash_length]

class Bus:
    def __init__(self, name, baud_rate):
        self.name = name
        self.baud_rate = baud_rate
        self.modules = []
    
    def add_module(self, module):
        self.modules.append(module)

print("Welcome to GopherCANnon.")

print("Loading network definition file \"{0}\".".format(filename))
with open(filename) as can_file:
    # Load network definition file
    raw_vehicle = yaml.full_load(can_file)
    vehicle = munch.Munch(raw_vehicle)
    modules = vehicle.modules
    parameters = vehicle.parameters
    raw_busses = vehicle.busses

    print("Network definition file \"{0}\" loaded.".format(filename))
    print("\nAnalyzing parameters.")
    for module in modules:
        for parameter in modules[module]['parameters_produced']:
            if parameters[parameter]['producer'] != module:
                print("Error: Expected producer of {0} to be {1}, found {2} instead!".format(parameter, module, parameters[parameter]['producer']))
                print("Incoherent parameters. Exiting")
                exit()
    print("Parameter and module definitions are coherent.")

    busses = []

    for bus in raw_busses.values():
        busses.append(Bus(bus['name'], bus['baud_rate']))

    error = False

    print("\nFound {0} bus(ses). Attempting to build.".format(len(busses)))

    # Assign modules to busses and ensure all busses are defined
    for module in modules.values():
        for module_bus in module['busses']:
            for bus in busses:
                if bus.name == module_bus:
                    bus.add_module(module)
                    break
                error = True
            if error:
                error = False
                print("Error: Bus \"{0}\" referenced by {1} is missing!".format(str(module_bus), module['name']))
                print("Network cannot be generated. Exiting.")
                exit()
    
    print("Successfully built {0} bus(ses).".format(len(busses)))

    for bus in busses:
        print("Analyzing {0}.".format(bus.name))
        produced = []
        consumed = []
        for module in bus.modules:
            if module['parameters_produced']:
                produced.extend(module['parameters_produced'])
            if module['parameters_consumed']:
                consumed.extend(module['parameters_consumed'])
        
        # Handle duplicate consumers
        consumed = list(set(consumed))

        # Orphaned parameters are consumed but not produced
        orphaned = [item for item in consumed if item not in produced]
        # Unused parameters are produced but not consumed
        unused = [item for item in produced if item not in consumed]

        print("{0} produced, {1} consumed, {2} orphaned, and {3} unused parameters.".format(len(produced), len(consumed), len(orphaned), len(unused)))
        if len(orphaned) != 0:
            print("WARNING: Orphaned parameters detected! Header files will still be generated, but make sure you know what you're doing!")

        response_preamble = 1 + 11 + 29 + 1 + 1 + 1 + 1 + 2 + 4
        response_epilogue = 15 + 1 + 1 + 1 + 7 + 3

        request_preamble = 1 + 11 + 29 + 1 + 1 + 1 + 1 + 2
        request_epilogue = 15 + 1 + 1 + 1 + 7 + 3

        # Analyze steady-state bus traffic
        print("Analyzing steady-state traffic on {0} from consumed parameters.".format(bus.name))
        bits_per_second = 0
        for parameter in consumed:
            bits_per_request = request_preamble + request_epilogue + response_preamble + response_epilogue + 8 * parameters[parameter]["bytes"]
            bits_per_second += bits_per_request * parameters[parameter]["frequency"]
        
        usage = 100 * bits_per_second / bus.baud_rate

        print("Steady-state bandwidth utilization is {0} bits/second which is {1}% of {2}\'s maximum capacity.".format(bits_per_second, usage, bus.name))

    # Generate common header file
    print("\nAnalysis complete. Generating common header file.")
    with open('GopherCAN_Common_TEMPLATE.h.jinja2') as file_:
        template = Template(file_.read())
        
        output = template.render(modules=modules.values(), repo_hash=repo_hash)
        with open("GopherCAN_Common.h", "w") as fh:
            fh.write(output)

    # Generate module-specific header files
    for module in modules.values():
        print("Generating {0} header file.".format(module['name']))
        with open('GopherCAN_Module_Specific_TEMPLATE.h.jinja2') as file_:
            template = Template(file_.read())
            
            output = template.render(module=module, modules=modules, parameters=parameters)
            filename = "GopherCAN_{0}.h".format(module["name"])
            with open(filename, "w") as fh:
                fh.write(output)
