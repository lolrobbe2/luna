import os
import threading
import time
import subprocess
import urllib.request
import urllib.error

vulkan_major_version = "1.3."
vulkan_install_version = "1.3.268.0"
vulkanSDKDownloadUrl = f"https://sdk.lunarg.com/sdk/download/{vulkan_install_version}/windows/VulkanSDK-{vulkan_install_version}-Installer.exe"


def download_file(url, destination, callback=None):
    try:
        req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
        with urllib.request.urlopen(req) as response, open(
            destination, "wb"
        ) as out_file:
            total_size = int(response.headers.get("Content-Length", 0))
            downloaded_size = 0
            while True:
                data = response.read(1024)
                if not data:
                    break
                out_file.write(data)
                downloaded_size += len(data)
                if callback:
                    callback(downloaded_size, total_size)
    except urllib.error.HTTPError as e:
        print(f"HTTP Error {e.code}: {e.reason}. Failed to download {url}")
    except Exception as e:
        print(f"An error occurred: {e}")


def download_progress(downloaded_size, total_size):
    percentage = (downloaded_size / total_size) * 100
    bar_length = 50
    completed_length = int(bar_length * (percentage / 100))
    remaining_length = bar_length - completed_length
    progress_bar = "[" + "=" * completed_length + ">" + " " * remaining_length + "]"
    print(f"Downloading... {progress_bar} {percentage:.2f}%\r", end="", flush=True)


def install_vulkan_thread(installer_path):
    subprocess.run(
        [
            installer_path,
            "--accept-licenses",
            "--default-answer",
            "--confirm-command",
            "install",
            "com.lunarg.vulkan.debug",
        ],
        check=True,
    )


def install_vulkan(force_install=False):
    if not force_install:
        permission_granted = False
        while not permission_granted:
            reply = (
                input(
                    f"Would you like to install Vulkan SDK {vulkan_install_version}? [Y/N]: "
                )
                .strip()
                .lower()
            )
            if reply == "n":
                return
            permission_granted = reply == "y"
    else:
        print(" > Force installing Vulkan!")

    # Download the Vulkan SDK installer
    installer_path = "vulkan_sdk_installer.exe"
    download_thread = threading.Thread(
        target=download_file,
        args=(vulkanSDKDownloadUrl, installer_path, download_progress),
    )
    download_thread.start()

    # Show dots while waiting for the download to complete
    #while download_thread.is_alive():
    #    for _ in range(3):
    #         print(".", end="", flush=True)
    #        time.sleep(0.1)
    #    print("\b\b\b   \b\b\b", end="", flush=True)
    #    time.sleep(0.5)

    # Wait for the download to complete
    download_thread.join()

    # Run the installer
    print("\nDownload complete. Starting installation...")
    install_thread = threading.Thread(
        target=install_vulkan_thread, args=(installer_path,)
    )
    install_thread.start()

    # Show dots while waiting for the installation to complete
    

    # Wait for the installation to complete
    install_thread.join()
    print("\nVulkan SDK installation complete.")


def vulkan(force_install):
    print("\n/*-----------------------------------------*/")
    print("/*                   vulkan                */")
    print("/*-----------------------------------------*/\n")
    if not is_vulkan_installed():
        print("Vulkan SDK not found. Installing...")
        install_vulkan(force_install)
    else:
        print("Found Vulkan SDK")
        vulkan_found_version = get_installed_vulkan_version()
        print(f" > Location: {get_installed_vulkan_path()}")
        print(f" > Version: {vulkan_found_version}")


def is_vulkan_installed():
    return (
        get_installed_vulkan_version() is not None
        and vulkan_major_version in get_installed_vulkan_version()
    )


def get_installed_vulkan_path():
    return os.environ.get("VULKAN_SDK")


def get_installed_vulkan_version():
    vulkan_version = os.environ.get("VULKAN_SDK_VERSION")
    if vulkan_version is None:
        vulkan_sdk_path = get_installed_vulkan_path()
        if vulkan_sdk_path is None:
            return None
        else:
            return vulkan_sdk_path.split("\\")[-1]
    else:
        return vulkan_version


if __name__ == "__main__":
    vulkan(force_install=True)
