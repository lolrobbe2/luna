# Define the version number separately
import os
import platform
import subprocess
import threading
import time

import urllib3


vulkan_major_version = "1.3."
vulkan_install_version = "1.3.216.0"

# Construct the URL with the version number
vulkanSDKDownloadUrl = f"https://sdk.lunarg.com/sdk/download/{vulkan_install_version}/windows/VulkanSDK-{vulkan_install_version}-Installer.exe"


def is_vulkan_installed():
    if get_installed_vulkan_version() is None:
        return False
    elif vulkan_major_version in get_installed_vulkan_version():
        return True
    else:
        print(
            f"required vulkan major version not installed: got{get_installed_vulkan_version()} needed {vulkan_install_version}"
        )
        return False

def get_installed_vulkan_path():
    return os.environ.get("VULKAN_SDK")


def get_installed_vulkan_version():
    vulkan_sdk_path = get_installed_vulkan_path()
    try:
        # Assume the latest version is the one with the highest numeric value
        return vulkan_sdk_path.split("\\")[-1]
    except (OSError, ValueError):
        pass
    return None


def install_vulkan(force_install=False):
    # Replace 'YOUR_VULKAN_SDK_DOWNLOAD_URL' with the actual download link of the Vulkan SDK
    download_url = "YOUR_VULKAN_SDK_DOWNLOAD_URL"
    if not force_install:
        permission_granted = False
        while not permission_granted:
            reply = (
                str(
                    input(
                        "Would you like to install VulkanSDK {0:s}? [Y/N]: ".format(
                            vulkan_install_version
                        )
                    )
                )
                .lower()
                .strip()[:1]
            )
            if reply == "n":
                return
            permission_granted = reply == "y"
    else:
        print(" > force installing vulkan!")
    # Download the Vulkan SDK installer
    installer_path = "vulkan_sdk_installer.exe"
    urllib3.request.urlretrieve(download_url, installer_path)

    # Run the installer in a separate thread
    install_thread = threading.Thread(
        target=install_vulkan_thread, args=(installer_path,)
    )
    install_thread.start()

    # Print dots during installation
    print("Installing Vulkan SDK: ", end="", flush=True)
    print(f" - version:{vulkan_install_version}")
    dots_printed = 0
    while install_thread.is_alive():
        print("\033[A                             \033[A")
        print("installing vulkan:")
        for i in range(dots_printed):
            print(".", end="", flush=True)
        dots_printed += 1
        time.sleep(1)
        if dots_printed == 3:
            dots_printed = 0

    # Join the install_thread to ensure it's finished before moving on
    install_thread.join()

    print("\nVulkan SDK installed successfully.")

    # Cleanup: Delete the installer
    os.remove(installer_path)


def vulkan(force_install):
    print("\n/*-----------------------------------------*/")
    print("/*                   vulkan                */")
    print("/*-----------------------------------------*/\n")
    if not is_vulkan_installed():
        print("Vulkan SDK not found. Installing...")
        install_vulkan(force_install)
    else:
        print("found vulkan SDK")
        vulkan_found_version = get_installed_vulkan_version()
        print(f" > location = {get_installed_vulkan_path()}")
        print(f" > version = {vulkan_found_version}")


def install_vulkan_thread(installer_path):
    subprocess.run([installer_path, "/S"], check=True)


def vulkan(force_install):
    print("\n/*-----------------------------------------*/")
    print("/*                   vulkan                */")
    print("/*-----------------------------------------*/\n")
    if not is_vulkan_installed():
        print("Vulkan SDK not found. Installing...")
        install_vulkan(force_install)
    else:
        print("found vulkan SDK")
        vulkan_found_version = get_installed_vulkan_version()
        print(f" > location = {get_installed_vulkan_path()}")
        print(f" > version = {vulkan_found_version}")
