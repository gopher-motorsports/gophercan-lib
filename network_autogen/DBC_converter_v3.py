"""Generate a golden-format DBC from a GopherCAN network YAML.

Usage:
    python DBC_converter_v2.py configs/go4-25.yaml --golden path/to/golden.dbc

The YAML intentionally does not record each message's transmitting node or
per-signal wire signedness.  The golden DBC supplies those details.  IDs,
group layouts, scales, offsets, units, and comments come from the latest YAML.
"""

import argparse
import os
import re
import sys
from collections import Counter

import yaml


EXPLICIT_SIGNED_TYPES = {"SIGNED8", "SIGNED16", "SIGNED32", "SIGNED64"}
FORCED_SIGNED_SIGNALS = {
    "steeringAngle_deg",
    "vnavSlipAngle_FL",
    "vnavSlipAngle_FR",
    "vnavSlipAngle_RL",
    "vnavSlipAngle_RR",
}

# New groups whose signals do not exist in the golden DBC cannot have their
# transmitter inferred. Keep those intentional additions explicit here.
EXPLICIT_GROUP_OWNERS = {
    0x25C: "FVC",
    0x30A: "BMS",
    0x30B: "BMS",
}


DBC_HEADER = '''VERSION ""

NS_ :
    NS_DESC_
    CM_
    BA_DEF_
    BA_
    VAL_
    CAT_DEF_
    CAT_
    FILTER
    BA_DEF_DEF_
    EV_DATA_
    ENVVAR_DATA_
    SGTYPE_
    SGTYPE_VAL_
    BA_DEF_SGTYPE_
    BA_SGTYPE_
    SIG_TYPE_REF_
    VAL_TABLE_
    SIG_GROUP_
    SIG_VALTYPE_
    SIGTYPE_VALTYPE_
    BO_TX_BU_
    BA_DEF_REL_
    BA_REL_
    BA_DEF_DEF_REL_
    BU_SG_REL_
    BU_EV_REL_
    BU_BO_REL_
    SG_MUL_VAL_

BS_:

'''


def parse_args():
    parser = argparse.ArgumentParser(
        description="Generate a golden-format DBC from a GopherCAN YAML config."
    )
    parser.add_argument("config", help="Network YAML, e.g. configs/go4-25.yaml")
    parser.add_argument(
        "--golden",
        required=True,
        help="Existing golden DBC used for message ownership and signedness",
    )
    parser.add_argument(
        "--output",
        help="Output DBC path (default: dbcs/<config-name>.dbc beside this script)",
    )
    return parser.parse_args()


def parse_golden(path):
    signal_owner = {}
    signal_sign = {}
    current_owner = None

    with open(path, encoding="utf-8", errors="replace") as dbc_file:
        for line in dbc_file:
            message = re.match(r"BO_\s+\d+\s+\S+:\s+\d+\s+(\S+)", line)
            if message:
                current_owner = message.group(1)
                continue

            signal = re.match(
                r"\s*SG_\s+(\w+)\s*:\s*\d+\|\d+@\d([+-])", line
            )
            if signal and current_owner:
                name = signal.group(1)
                signal_owner[name] = current_owner
                signal_sign[name] = signal.group(2)

    return signal_owner, signal_sign


def choose_owner(group, signal_owner):
    message_id = int(group["id"])
    if message_id in EXPLICIT_GROUP_OWNERS:
        return EXPLICIT_GROUP_OWNERS[message_id]

    owners = [signal_owner[p["name"]] for p in group["parameters"] if p["name"] in signal_owner]
    if not owners:
        raise ValueError(
            f"Cannot determine transmitter for group {message_id:#x}; "
            "none of its signals exist in the golden DBC"
        )

    counts = Counter(owners)
    owner, _ = counts.most_common(1)[0]
    if len(counts) > 1:
        print(
            f"[!] Group {int(group['id']):#x} contains signals from multiple "
            f"golden transmitters {dict(counts)}; using {owner}"
        )
    return owner


def signal_sign(name, param, golden_sign):
    if name in FORCED_SIGNED_SIGNALS:
        return "-"
    if name in golden_sign:
        return golden_sign[name]
    return "-" if param["type"] in EXPLICIT_SIGNED_TYPES else "+"


def physical_range(length_bits, scale, offset, sign):
    if sign == "-":
        raw_min = -(2 ** (length_bits - 1))
        raw_max = (2 ** (length_bits - 1)) - 1
    else:
        raw_min = 0
        raw_max = (2**length_bits) - 1
    return raw_min * scale + offset, raw_max * scale + offset


def message_dlc(group):
    return max(p["start"] + p["length"] for p in group["parameters"])


def write_message(out, group, params, owner, golden_sign):
    message_id = int(group["id"])
    message_name = f"{owner}_{message_id:03X}"
    out.write(f"BO_ {message_id} {message_name}: {message_dlc(group)} {owner}\n")

    for field in group["parameters"]:
        name = field["name"]
        param = params[name]
        length_bits = field["length"] * 8
        is_msb = param["encoding"] == "MSB"
        byte_order = 0 if is_msb else 1
        start_bit = field["start"] * 8 + (7 if is_msb else 0)
        sign = signal_sign(name, param, golden_sign)
        minimum, maximum = physical_range(
            length_bits, param["scale"], param["offset"], sign
        )
        out.write(
            f' SG_ {name} : {start_bit}|{length_bits}@{byte_order}{sign} '
            f'({param["scale"]},{param["offset"]}) '
            f'[{minimum}|{maximum}] "{param["unit"]}" Vector__XXX\n'
        )
    out.write("\n")


def escape_comment(value):
    return str(value).replace("\\", "\\\\").replace('"', '\\"')


def main():
    args = parse_args()
    config_path = os.path.abspath(args.config)
    golden_path = os.path.abspath(args.golden)

    for label, path in (("YAML", config_path), ("golden DBC", golden_path)):
        if not os.path.isfile(path):
            print(f"[-] {label} does not exist: {path}")
            return 1

    with open(config_path, encoding="utf-8") as config_file:
        config = yaml.safe_load(config_file)

    signal_owner, golden_sign = parse_golden(golden_path)
    output_name = re.sub(
        r"\.ya?ml$", ".dbc", os.path.basename(config_path), flags=re.IGNORECASE
    )
    output_path = os.path.abspath(
        args.output
        or os.path.join(os.path.dirname(os.path.abspath(__file__)), "dbcs", output_name)
    )
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    groups = config["groups"]
    params = config["parameters"]
    owners = {choose_owner(group, signal_owner) for group in groups}
    configured_nodes = list(config.get("modules", {}).keys())
    nodes = configured_nodes + sorted(owners - set(configured_nodes))
    if "Vector__XXX" not in nodes:
        nodes.append("Vector__XXX")

    with open(output_path, "w", encoding="utf-8", newline="\n") as out:
        out.write(DBC_HEADER)
        out.write("BU_: " + " ".join(nodes) + "\n\n")
        for group in groups:
            owner = choose_owner(group, signal_owner)
            write_message(out, group, params, owner, golden_sign)

        for group in groups:
            message_id = int(group["id"])
            for field in group["parameters"]:
                param = params[field["name"]]
                comment = param.get("motec_name")
                if comment:
                    out.write(
                        f'CM_ SG_ {message_id} {field["name"]} '
                        f'"{escape_comment(comment)}";\n'
                    )

    print(f"[+] Generated {len(groups)} messages at: {output_path}")
    print("[+] Golden message naming, transmitters, signedness, nodes, and comments preserved")
    return 0


if __name__ == "__main__":
    sys.exit(main())
