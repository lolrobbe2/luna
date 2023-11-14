import argparse
import os
import subprocess
import platform
import threading
import time
import zipfile
from scripts.setupPremake import premake
from scripts.setupVulkan import vulkan
from scripts.setupPython import PythonConfiguration as PythonRequirements


def main():
    # parse options
    parser = argparse.ArgumentParser(description="Install Vulkan SDK and Premake.")
    parser.add_argument(
        "--force-vulkan",
        action="store_true",
        help="Install Vulkan SDK without asking for permission.",
    )
    parser.add_argument(
        "--force-premake",
        action="store_true",
        help="Install premake without asking for permission.",
    )

    parser.add_argument(
        "--force-python",
        action="store_true",
        help="Install pyhton packages without asking for permission.",
    )
    args = parser.parse_args()

    PythonRequirements.Validate(args.force_python)

    vulkan(args.force_vulkan)
    premake(args.force_premake)


if __name__ == "__main__":
    main()