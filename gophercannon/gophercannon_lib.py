from random import random

# Constants
MICROSECONDS_PER_SECOND = 1000000
RESPONSE_PREAMBLE = 1 + 11 + 29 + 1 + 1 + 1 + 1 + 2 + 4
RESPONSE_EPILOGUE = 15 + 1 + 1 + 1 + 7 + 3
REQUEST_PREAMBLE = 1 + 11 + 29 + 1 + 1 + 1 + 1 + 2
REQUEST_EPILOGUE = 15 + 1 + 1 + 1 + 7 + 3

# Helper functions
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

def get_gophercan_id(priority, destination_node, source_node, id):
    return (priority << 28) | (destination_node << 22) | (source_node << 16) | (id)

# Classes
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
                message.timer = message.reset_value + (random() - 0.5) * self.noise # Simulate sway in message timing
        
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
