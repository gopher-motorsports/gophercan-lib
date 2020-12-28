# GopherCANnon User Guide
## Overview
GopherCANnon is a tool that can be used with the GopherCAN communication protocol. It helps validate network architectures, conducts basic traffic analysis, and generates C header files from a central configuration document.
## Setup
To use GopherCANon, you have do the following on a machine with Python installed
1. Create a virtual environment named venv  
This can be done on windows with `py -m virtrualenv venv` or on linux with `python -m virtualenv venv`
2. Activate the virtual environment. This can be done linux with `source venv/bin/activate` or on windows with `venv/scripts/activate` (or something like that I don't totally remember)
3. Install required packages in `requirements.txt` with `pip install -r requirements.txt`
4. Create a valid network architecture file. An example can be found in `go4-22c.yaml`
5. Ensure the `gophercannon.py` file has execution privileges (`chmod +x gophercannon.py` on linux)
6. Invoke the script in the form of `./gophercannon.py <filename>`. Run `gophercannon.py --help` for help  

## Notes
GopherCANnon has two modes of operation: network analysis and code generation.
### Network Analysis
After successfully loading in a network architecture file, GopherCANnon will attempt to find errors in the network configuration with the following tests:
- Ensuring that each parameter is produced by a single module and that module is configured to produce it
- Checking that each module is assigned to a valid bus
- Comparing produced and consumed parameters on each bus and finding any orphaned or unused parameters
- Computing the steady-state bandwidth utilization for each bus assuming a standard remote-response transaction per each parameter  

This list is not final, checks/tests may be added or removed in future versions.

### Code Generation
Once critical checks are passed, GopherCANnon will begin to auto-generate GopherCAN header files for each device in the network. Using the Jinja2 rendering engine, pre-formatted templates will be filled with the appropriate data for each device in their own header file as well as common header for all devices in the network. The exact nature of these header files is determined by the GopherCAN standard and is subject to change.