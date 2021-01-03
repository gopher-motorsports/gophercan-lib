# Proposed GopherCAN Auto-Generation System (GopherCANnon)
This document is intended to describe a system that would allow for automatic generation of GopherCAN configuration headers and source files, DBC files to allow decoding bus traffic with CAN analyzers, documentation, and potentially even sensor calibration files and other metadata.  

## High-level overview
### YAML File Example
This system would use YAML files as the primary way for developers to add or modify GopherCAN data. An example might look something like this:
```
#gophercan.yaml
Busses:
    bus_0:
        Baud rate: 1000
        Human-readable name: "Front Bus"
    bus_1:
        Baud rate: 1000
        Human-readable name: "Rear Left Bus"
    bus_2:
        Baud rate: 1000
        Human-readable name: "Rear Right Bus"

Parameters:
    batt_volts:
        Human-readable name: "Battery Voltage"
        Unit: "Volts"
        Unit shorthand: "V"
        Type: uint16_t
        Multiplier: 1
        Divisor: 10
        Offset: 0

    wheelspeed_front_left:
        Human-readable name: "Front Left Wheelspeed"
        Unit: "RPM"
        Type: uint16_t
        Multiplier: 5
        Divisor: 16
        Offset: 0
    ...

Modules:
    ACM:
        Id: 4
        Busses: 
            - bus_0
        Human-readable name: "Aerodynamic Control Module"
        Parameters produced:
            - airspeed_front
            - airspeed_rear
            - drs_button_state
            ...
        Parameters consumed:
            - lateral_acceleration
            - longitudinal_acceleration
            - steering_angle
            - yaw_rate
            - roll_rate
            ...
    ...
```
This example shows how many different aspects of GopherCAN can be conveniently defined in a single file. The YAML format is very flexible so this is just a subset of what metadata could be captured. Note that relationships between items can be defined at this stage (ACM is on bus_x, produces parameters y and z, and consumes parameters b and c). This allows such relationships to be specified at the earliest possible point allowing for high quality documentation as well as adding the ability to sanity-check the vehicle's configuration (ex. module x is on bus z and consumes parameter y but no module on bus z produces it).  
Once data is captured in YAML, it can be processed into many different outputs. Python seems to be an ideal candidate for such a task, being both easy to work with and having tons of packages available to help automate workflows.
### GopherCAN Source and Header Generation
Jinja2 provides a powerful framework for basic c file generation. It's a templating language and renderer which can be used in the following way:
```
<!DOCTYPE html>
<html lang="en">
<head>
    <title>My Webpage</title>
</head>
<body>
    <ul id="navigation">
    {% for item in navigation %}
        <li><a href="{{ item.href }}">{{ item.caption }}</a></li>
    {% endfor %}
    </ul>

    <h1>My Webpage</h1>
    {{ a_variable }}

    {# a comment #}
</body>
</html>
```
Though the example is an html file, it shows how the templating language is used for simple textual replacement. It also allows for constructs like for and foreach loops, making it a great fit to generate c headers. Standard .c and .h template files can be maintained and turned into real files once the YAML is parsed.
### Documentation
I don't have an end format in mind, but we could use the YAML file to generate some easy-to-read documentation as a part of the autogen process. Some simple HTML files could be incredibly useful to have at track as they would be much easier to parse than YAML or actual c. I can provide some examples of what this might look like at a later time.
### DBC Files
This is definitely not as important as the GopherCAN file generation, but having the data in this format would allow us to automatically generate the DBC files that CAN analyzers use to interpret bus traffic. [Kvaser](https://www.kvaser.com/developer-blog/handling-can-databases-python/) provides a very straightforward example of how to generate a DBC in python. It is as follows: 
```
from canlib import kvadblib

# Create a new database
db = kvadblib .Dbc(name=’Histogram ’ )

# Add messages
message = db.new_message(name=’ECM_004’ ,
                         id=504,
                         dlc=8)

message . new_signal(name=’Fuel ’ ,
                     type=kvadblib . SignalType .FLOAT
                     size=kvadblib . ValueSize( startbit=0, length=32) ,
                     limits=kvadblib . ValueLimits(min=0, max=300) ,
                     unit=’ l /100 km’ ,
                     comment=’Current fuel consumption . ’ )

# write database file
db. write_file ( ’db_histogram . dbc ’ )
db. close ()
```
As we would presumably have all of this data parsed into a python script, it would be trivial to add this to the autogen process, allowing us to ensure our CAN analyzers are able to decode traffic for every single GopherCAN configuration

## Other Things That We Could Use This For
Maintaining a centralized configuration for the entire vehicle opens up a lot of possibilities. Other things that may be able to be added here include:
- Alerts: priority levels/vehicle-level responses
- Vehicle-level states (related to alerts?)
- DLM routing tables/policies
- DAM/module sensor calibration assignment (this might be better suited to a separate file but can still be parsed by the same system)
- Telemetry configuration (this might be better suited to a separate file but can still be parsed by the same system)