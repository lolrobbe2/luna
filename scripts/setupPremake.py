import os
import platform
import subprocess
import threading
import time
import zipfile

import urllib.request


premake_download_url = "https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip"


def install_premake_thread(installer_path):
    subprocess.run([installer_path, "/S"], check=True)


def run_premake_vs2022():
    subprocess.run(["vendor\\premake5", "vs2019"], check=True)


def download_and_extract_premake():
    zip_file_path = "premake.zip"
    urllib3.request.urlretrieve(premake_download_url, zip_file_path)

    with zipfile.ZipFile(zip_file_path, "r") as zip_ref:
        # Extract premake5.exe to the vendor folder
        zip_ref.extract("premake5.exe", "vendor")

    # Cleanup: Delete the downloaded ZIP file
    os.remove(zip_file_path)


def install_premake(force_install=False):
    # ask for permission to install premake
    if not force_install:
        permissionGranted = False
        while not permissionGranted:
            reply = (
                str(input("Would you like to install premake {0:s}? [Y/N]: "))
                .lower()
                .strip()[:1]
            )
            if reply == "n":
                return
            permissionGranted = reply == "y"
    else:
        print("force installing premake")

    # start downloading and extracting
    download_thread = threading.Thread(target=download_and_extract_premake)
    download_thread.start()

    dots_printed = 0
    while download_thread.is_alive():
        print("\033[A                             \033[A")
        print("installing premake:")
        for i in range(dots_printed):
            print(".", end="", flush=True)
        dots_printed += 1
        time.sleep(1)
        if dots_printed == 3:
            dots_printed = 0

    print("installed premake succesfully")


def is_premake_installed():
    if platform.system() == "Windows":
        return os.path.exists("vendor\\premake5.exe")
    else:
        return False


def premake(force_install):
    print("\n/*------------------------------------------*/")
    print("/*                   premake                */")
    print("/*------------------------------------------*/\n")
    if not is_premake_installed():
        print("premake not installed")
        install_premake(force_install)
    else:
        run_premake_vs2022()
        print("Premake5 executed successfully.")
