import argparse
import os
import subprocess
import platform
import urllib.request
import threading
import time

# Define the version number separately
vulkan_major_version = "1.3."
vulkan_install_version = "1.3.216.0"

# Construct the URL with the version number
vulkanSDKDownloadUrl = f"https://sdk.lunarg.com/sdk/download/{vulkan_install_version}/windows/VulkanSDK-{vulkan_install_version}-Installer.exe"

premake_download_url = "https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip"


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


def is_premake_installed():
    if platform.system() == "Windows":
        return os.path.exists("vendor\\premake5.exe")
    else:
        False


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
        permissionGranted = False
        while not permissionGranted:
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
            permissionGranted = reply == "y"
    else:
        print(" > force installing vulkan!")
    # Download the Vulkan SDK installer
    installer_path = "vulkan_sdk_installer.exe"
    urllib.request.urlretrieve(download_url, installer_path)

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
        for i in dots_printed:
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


def run_premake_vs2022():
    subprocess.run(["vendor\\premake5", "vs2019"], check=True)

def install_premake(force_install=False):
    #ask for permission to install premake
    if not force_install:
        permissionGranted = False
        while not permissionGranted:
            reply = (
                str(
                    input(
                        "Would you like to install premake {0:s}? [Y/N]: ".format(
                            vulkan_install_version
                        )
                    )
                )
                .lower()
                .strip()[:1]
            )
            if reply == "n":
                return
            permissionGranted = reply == "y"

def premake():
    print("\n/*------------------------------------------*/")
    print("/*                   premake                */")
    print("/*------------------------------------------*/\n")
    if not is_premake_installed():
        print("premake not installed")
    else:
        run_premake_vs2022()
        print("Premake5 executed successfully.")


def main():
    #parse options
    parser = argparse.ArgumentParser(description="Install Vulkan SDK and Premake.")
    parser.add_argument(
        "--force-vulkan", action="store_true", help="Install Vulkan SDK without asking for permission."
    )
    parser.add_argument(
        "--force-premake", action="store_true", help="Install Vulkan SDK without asking for permission."
    )
    args = parser.parse_args()

    vulkan(args.force_vulkan)
    premake()


if __name__ == "__main__":
    main()
