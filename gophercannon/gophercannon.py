import yaml
import git
import munch
from jinja2 import Template
import argparse
import time
import tabulate
from random import random
import matplotlib.pyplot as plt
import os
from json2html import *
from functools import partial

MICROSECONDS_PER_SECOND = 1000000

RESPONSE_PREAMBLE = 1 + 11 + 29 + 1 + 1 + 1 + 1 + 2 + 4
RESPONSE_EPILOGUE = 15 + 1 + 1 + 1 + 7 + 3

REQUEST_PREAMBLE = 1 + 11 + 29 + 1 + 1 + 1 + 1 + 2
REQUEST_EPILOGUE = 15 + 1 + 1 + 1 + 7 + 3

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
MICROSECONDS_PER_SECOND = 1000000

repo = git.Repo(search_parent_directories=True)
repo_hash = repo.head.object.hexsha[0:HASH_LENGTH]

def get_gophercan_id(priority, destination_node, source_node, id):
    return (priority << 28) | (destination_node << 22) | (source_node << 16) | (id)

def my_html_row_with_attrs(celltag, cell_values, colwidths, colaligns):
    alignment = { "left":    '',
                  "right":   ' style="text-align: right;"',
                  "center":  ' style="text-align: center;"',
                  "decimal": ' style="text-align: right;"' }
    values_with_attrs =\
        ["<{0}{1} class=\"my-cell\">{2}</{0}>"
            .format(celltag, alignment.get(a, ''), c)
         for c, a in zip(cell_values, colaligns)]
    return "<tr class=\"my-row\">" + \
            "".join(values_with_attrs).rstrip() + \
            "</tr>"

class Module:
    def __init__(self, name, fifo_mailbox, noise):
        self.name = name
        self.fifo_mailbox = fifo_mailbox
        self.noise = noise
        self.messages = []
        self.tx_buffer = []
    
    def add_message(self, message):
        self.messages.append(message)
    
    def tick(self):
        for message in self.messages:
            message.timer -= 1

        for message in self.messages:
            if message.timer <= 0:
                self.tx_buffer.append(message)
                self.tx_buffer.sort(key=lambda x: get_gophercan_id( int(x.priority), 
                                                                    int(x.producer),
                                                                    int(x.consumer),
                                                                    int(x.id)), reverse=False)
                message.timer = message.reset_value + (random() - 0.5) * args.wobble # Simulate sway in message timing
        
        for message in self.tx_buffer:
            message.buffer_delay += 1

class Message:
    def __init__(self, name, length, priority, frequency, producer, consumer, id):
        self.name = name
        self.length = length
        self.priority = priority
        self.frequency = frequency
        self.producer = producer
        self.consumer = consumer
        self.id = id
        self.timer = 0
        self.buffer_delay = 0
        self.reset_value = MICROSECONDS_PER_SECOND / frequency
        self.buffer_delays = []
    
    def get_length(self):
        return REQUEST_PREAMBLE + REQUEST_EPILOGUE + RESPONSE_PREAMBLE + (8 * self.length) + RESPONSE_EPILOGUE

class Bus:
    def __init__(self, name, baud_rate):
        self.name = name
        self.baud_rate = baud_rate
        self.modules = []
        self.timer = 0
        self.master_timer = 0
        self.idle_timer = 0
        self.line = []
        self.arbitrators = []
    
    def add_module(self, module):
        self.modules.append(module)

    def simulate(self):
        competing_modules = [x for x in self.modules if len(x.tx_buffer) > 0]
        self.arbitrators.append(len(competing_modules))
        if self.timer > 0:
            self.timer -= 1
        else:
            if len(competing_modules) > 0:
                competing_modules.sort(key=lambda x: get_gophercan_id(  int(x.tx_buffer[0].priority), 
                                                                        int(x.tx_buffer[0].producer),
                                                                        int(x.tx_buffer[0].consumer),
                                                                        int(x.tx_buffer[0].id)), reverse=False)
                mod = competing_modules.pop(0)
                msg = mod.tx_buffer.pop(0)
                msg.buffer_delays.append(msg.buffer_delay)
                msg.buffer_delay = 0
                self.timer = msg.get_length() - 1 # We spent one time quanta arbitrating
            else:
                self.idle_timer += 1

print("Welcome to GopherCANnon.")

print("Loading network definition file \"{0}\".".format(filename))
with open(filename) as can_file:
    # Load network definition file
    raw_vehicle = yaml.full_load(can_file)
    vehicle = munch.Munch(raw_vehicle)
    modules = vehicle.modules
    parameters = vehicle.parameters
    raw_busses = vehicle.busses

    steady_usage = 0
    transient_usage = 0
    message_table = []

    print("Network definition file \"{0}\" loaded.".format(filename))
    print("\nAnalyzing parameters.")
    for module in modules:
        if modules[module]['parameters_produced'] is not None:
            for parameter in modules[module]['parameters_produced']:
                if parameters[parameter]['producer'] != module:
                    print("Error: Expected producer of {0} to be {1}, found {2} instead!".format(   parameter, 
                                                                                                    module, 
                                                                                                    parameters[parameter]['producer']))
                    if args.narc:
                        print("Incoherent parameters. Exiting.")
                        exit(1)

        if modules[module]['parameters_consumed'] is not None:
            for parameter in modules[module]['parameters_consumed']:
                if parameters[parameter]['producer'] == module:
                    print("Error: Cannibalism detected! {0} both produces and consumes {1}.".format(module,
                                                                                                    parameter))
                    if args.narc:
                        print("Incoherent parameters. Exiting.")
                        exit(1)

    busses = []

    for bus in raw_busses.values():
        busses.append(Bus(bus['name'], bus['baud_rate']))

    error = False

    print("\nFound {0} bus(ses).".format(len(busses)))

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
                if args.narc:
                    print("Network cannot be generated. Exiting.")
                    exit(1)
    
    for bus in busses:
        print("Analyzing {0}.".format(bus.name))
        produced = []
        consumed = []
        for module in bus.modules:
            if module['parameters_produced']:
                produced.extend(module['parameters_produced'])
            if module['parameters_consumed']:
                consumed.extend(module['parameters_consumed'])
        
        # Orphaned parameters are consumed but not produced
        orphaned = [item for item in consumed if item not in produced]
        # Unused parameters are produced but not consumed
        unused = [item for item in produced if item not in consumed]

        print("{0} produced, {1} consumed, {2} orphaned, and {3} unused parameters.".format(len(produced), 
                                                                                            len(consumed), 
                                                                                            len(orphaned), 
                                                                                            len(unused)))
        if len(orphaned) != 0:
            print("Error: Orphaned parameter detected. {0} is consumed but not produced.".format(orphaned[0]))
            if args.narc:
                print("{0} configuration is not valid. Exiting.".format(bus.name))
                exit(1)

        if args.steady_state:
            # Analyze steady-state bus traffic
            print("\nAnalyzing steady-state traffic on {0} from consumed parameters.".format(bus.name))
            bits_per_second = 0
            for parameter in consumed:
                bits_per_request = REQUEST_PREAMBLE + REQUEST_EPILOGUE + RESPONSE_PREAMBLE + RESPONSE_EPILOGUE + 8 * parameters[parameter]["bytes"]
                bits_per_second += bits_per_request * parameters[parameter]["frequency"]
            
            steady_usage = 100 * bits_per_second / bus.baud_rate

            print("Steady-state bandwidth utilization is {0} bits/second which is {1}% of {2}\'s maximum capacity.".format(bits_per_second, steady_usage, bus.name))

    for buss in busses:
        if args.transient is not None and args.transient > 0:
            # Analyze 10 seconds of transient bus traffic to get worst-case latency
            print("\nAnalyzing {0} second(s) of transient traffic on {1} from consumed parameters.".format(args.transient, bus.name))
            bus = Bus("bus", bus.baud_rate)
            sim_modules = []
            i = 0
            for module in buss.modules:
                if module['parameters_consumed']:
                    sim_modules.append(Module(module, True, 10))
                    for parameter in module['parameters_consumed']:
                        sim_modules[i].add_message(Message( parameter, 
                                                            parameters[parameter]['bytes'], 
                                                            parameters[parameter]['priority'], 
                                                            parameters[parameter]['frequency'],
                                                            modules[parameters[parameter]['producer']]['id'],
                                                            module['id'],
                                                            parameters[parameter]['id']))
                    i += 1
            for module in sim_modules:
                bus.add_module(module)

            i = 0
            while i < args.transient - 1:
                for module in bus.modules:
                    module.tick()
                bus.simulate()
                bus.master_timer += 1
                if bus.master_timer >= MICROSECONDS_PER_SECOND:
                    i += 1
                    print("Simulated {0} seconds of traffic.".format(i))
                    bus.idle_timer = 0
                    bus.master_timer = 0

            while i < args.transient:
                for module in bus.modules:
                    module.tick()
                bus.simulate()
                bus.master_timer += 1
                if bus.master_timer >= MICROSECONDS_PER_SECOND:
                    i += 1
                    transient_usage = 100 * (MICROSECONDS_PER_SECOND - bus.idle_timer) / bus.master_timer
                    print("Utilization over the last second: {0}%".format(transient_usage))
                    bus.idle_timer = 0
                    bus.master_timer = 0
                    message_table = []
                    for module in bus.modules:
                        for message in module.messages:
                            message_table.append([message.name, 
                                                    len(message.buffer_delays), 
                                                    min(message.buffer_delays), 
                                                    max(message.buffer_delays), 
                                                    sum(message.buffer_delays) / len(message.buffer_delays)])
                    print(tabulate.tabulate(message_table, [ "Parameter", 
                                                    "TX Count", 
                                                    "Min Delay (us)", 
                                                    "Max Delay(us)", 
                                                    "Avg. Delay(us)"], tablefmt="pipe"))

                    if args.reports:
                        os.makedirs('reports', exist_ok=True)
                        fig, ax = plt.subplots(2,figsize=(20,10))
                        ax[0].set_xlabel('Simulation Time (us)')
                        ax[0].set_ylabel('TX Buffer Wait time (us)')
                        for module in bus.modules:
                            for message in module.messages:
                                ax[0].plot([i / message.frequency for i in range(1, len(message.buffer_delays) + 1)], message.buffer_delays, label = "{0}, {1}".format(message.name, module.name['name']))
                        ax[1].set_xlabel('Simulation Time (us)')
                        ax[1].set_ylabel('Modules Waiting for Arbitration')
                        ax[1].plot([i / bus.baud_rate for i in range(1, len(bus.arbitrators) + 1)], bus.arbitrators)
                        fig.tight_layout()
                        ax[0].legend()
                        filename = "{0}_transient_graph.png".format(repo_hash)
                        plt.savefig(os.path.join('reports', filename))

    if not args.dry_run:
        os.makedirs('outputs', exist_ok=True)
        # Generate common header file
        print("\nAnalysis complete. Generating common header file.")
        filename = 'GopherCAN_Common_TEMPLATE.h.jinja2'
        with open(os.path.join('templates', filename)) as file_:
            template = Template(file_.read())
            
            output = template.render(modules=modules.values(), repo_hash=repo_hash)
            filename = "GopherCAN_Common.h"
            with open(os.path.join('outputs', filename), "w") as fh:
                fh.write(output)

        # Generate module-specific header files
        for module in modules.values():
            print("Generating {0} header file.".format(module['name']))
            filename = 'GopherCAN_Module_Specific_TEMPLATE.h.jinja2'
            with open(os.path.join('templates', filename)) as file_:
                template = Template(file_.read())
                output = template.render(module=module, modules=modules, parameters=parameters)
                filename = "GopherCAN_{0}.h".format(module["name"])
                with open(os.path.join('outputs', filename), "w") as fh:
                    fh.write(output)

    if args.reports:
        network_table = json2html.convert(json = raw_vehicle, table_attributes="id=\"info-table\" class=\"table table-condensed table-bordered table-hover\"")
        metadata_table = json2html.convert(json = vars(args), table_attributes="id=\"info-table\" class=\"table table-condensed table-bordered table-hover\"")
        MyHTMLFormat = tabulate.TableFormat(
            lineabove=tabulate.Line("<table class=\"table table-condensed table-bordered table-hover\">", "", "", ""),
            linebelowheader=None,
            linebetweenrows=None,
            linebelow=tabulate.Line("</table>", "", "", ""),
            headerrow=partial(my_html_row_with_attrs, "th"),
            datarow=partial(my_html_row_with_attrs, "td"),
            padding=0, with_header_hide=None)
        transient_table = tabulate.tabulate(message_table, [ "Parameter", 
                                                    "TX Count", 
                                                    "Min Delay (us)", 
                                                    "Max Delay(us)", 
                                                    "Avg. Delay(us)"], tablefmt=MyHTMLFormat)
        image_file = "{0}_transient_graph.png".format(repo_hash)
        filename = 'report.html.jinja2'
        with open(os.path.join('templates', filename)) as file_:
            template = Template(file_.read())
            output = template.render(   network_table=network_table, 
                                        metadata_table=metadata_table, 
                                        steady_usage=steady_usage, 
                                        transient_table=transient_table,
                                        transient_duration=args.transient,
                                        transient_usage=transient_usage,
                                        image_file=image_file)
            filename = "{0}_transient_report.html".format(repo_hash)
            with open(os.path.join('reports', filename), "w") as fh:
                fh.write(output)
