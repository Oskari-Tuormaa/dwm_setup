#!/usr/bin/env python3

import subprocess as sp
import re
import os
from datetime import datetime
import regex
import psutil
import time

COL = dict(
    BAT_DECHARGE="^c#af5f5f^",
    BAT_CHARGE="^c#5f87af^",
    AUDIO_MUTED="^c#af5f5f^",
    CPU_BG="^c#191919^",
    CPU_FG="^c#5f87af^",
    TEXT="^c#aaaaaa^",
)

START_BOX = f"^r00,02,01,14^^r00,02,03,01^^r00,16,03,01^^f5^{COL['TEXT']}"
END_BOX = "^d^^r04,02,01,14^^r02,02,03,01^^r02,16,03,01^^f5^"


def get_temp():
    out = sp.check_output("acpi -t", shell=True)
    out = out.decode("utf-8")

    res = re.search("\d*\.\d*(?= degrees)", out).group(0)

    res += COL["TEXT"]
    return res


def get_audio():
    res = ""

    out = sp.check_output("amixer get Master", shell=True)
    out = out.decode("utf-8")

    if re.search("\[(on|off)\]", out).group(0) == "[off]":
        res += COL["AUDIO_MUTED"]

    vol = re.search("\d+%", out).group(0)
    res += vol

    res += COL["TEXT"]
    return res


def get_connection():
    res = ""

    out = sp.check_output("nmcli d status", shell=True)
    out = out.decode("utf-8")

    con_type = re.search("wifi|ethernet", out).group(0)

    if con_type == "ethernet":
        return "Ethernet"
    elif con_type == "wifi":
        dev = re.search("[\w]*?(?= +?wifi)", out).group(0)
        out = sp.check_output(f"iwconfig {dev}", shell=True)
        out = out.decode("utf-8")
        ESSID = re.search('(?<=ESSID:").*(?=")', out).group(0)
        link_qual = int(re.search("(?<=Link Quality=)\d+", out).group(0))
        link_qual *= 100 / 70

        res += ESSID
        res += " "

        res += "^r00,12,4,02^" if 0 <= link_qual else "^c#444444^^r00,13,04,01^"
        res += "^r05,10,4,04^" if 25 <= link_qual else "^c#444444^^r05,13,04,01^"
        res += "^r10,08,4,06^" if 50 <= link_qual else "^c#444444^^r10,13,04,01^"
        res += "^r15,05,4,09^" if 75 <= link_qual else "^c#444444^^r15,13,04,01^"

        res += "^d^^f19^"

    return res


def get_battery():
    res = ""

    out = sp.check_output("acpi", shell=True)
    out = out.decode("utf-8")

    bat_lines = []
    for line in out.splitlines():
        bat = ""

        charge = re.findall("\d+%", line)[0]
        if re.search("Discharging", line) is not None:
            bat += COL["BAT_DECHARGE"]
        if re.search("Charging", line) is not None:
            bat += COL["BAT_CHARGE"]
        bat += charge
        bat += "^d^"

        bat_lines.append(bat)

    res += " ".join(bat_lines)

    return res


def get_ram():
    used = psutil.virtual_memory().used / (1024**3)
    total = psutil.virtual_memory().total / (1024**3)

    return "%.2f/%.2f GiB" % (used, total)


def get_cpu():
    res = ""

    cpu = psutil.cpu_percent(percpu=True, interval=0.2)

    for v in cpu:
        h = v * 14 / 100
        res += f'{COL["CPU_FG"]}^r02,02,04,14^'
        res += f'{COL["CPU_BG"]}^r02,02,04,{14-h}^^f8^'

    return res


def main():
    get_cpu()

    cmd = f'xsetroot -name " {START_BOX}'

    info = [
        get_audio(),
        get_battery(),
        get_connection(),
        get_temp() + "°C" + " | " + get_ram() + " | " + get_cpu(),
        datetime.now().strftime("%d/%m/%y"),
        datetime.now().strftime("%H:%M:%S"),
    ]
    cmd += f"{END_BOX}  {START_BOX}".join(info)

    cmd += f'{END_BOX} "'

    os.system(cmd)


if __name__ == "__main__":
    main()
