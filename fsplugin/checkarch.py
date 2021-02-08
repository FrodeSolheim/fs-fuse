#!/usr/bin/env python3
import sys
import platform

machine = platform.machine().lower()
if machine in ["x86_64", "amd64", "i386", "i486", "i585", "i686"]:
    if platform.architecture()[0] == "32bit":
        print("x86")
        sys.exit(0)
    if platform.architecture()[0] == "64bit":
        print("x86-64")
        sys.exit(0)
elif machine in ["armv7l"]:
    if platform.architecture()[0] == "32bit":
        # Assume we might have optimized software for ARMv8
        print("ARMv8")
        sys.exit(0)
elif machine.startswith("power"):
    if platform.architecture()[0] == "32bit":
        print("PPC")
        sys.exit(0)

print("WARNING: Unrecognized architecture:", machine, file=sys.stderr)
print("Unknown")
