import os
import shutil
import sys


def copy_folder_contents(source_folder, destination_folder):
    try:
        # Check if the source folder exists
        if not os.path.exists(source_folder):
            print(f"Source folder '{source_folder}' does not exist.")
            return

        # Check if the destination folder exists, create it if not
        if not os.path.exists(destination_folder):
            print("creating folder")
            os.makedirs(destination_folder)

        # Copy the contents of the source folder to the destination folder
        for item in os.listdir(source_folder):
            source_item = os.path.join(source_folder, item)
            destination_item = os.path.join(destination_folder, item)

            # Use shutil.copy2() to copy files, preserving metadata
            if os.path.isfile(source_item):
                shutil.copy2(source_item, destination_item)

        print(f"Contents of '{source_folder}' copied to '{destination_folder}'.")
    except Exception as e:
        print(f"An error occurred: {e}")


def main():
    script_directory = os.path.dirname(os.path.abspath(__file__))

    # Set the current working directory to the script directory
    os.chdir(script_directory)
    print(os.getcwd())
    outputdir = "debug-windows-x86_64"
    copy_folder_contents(f"bin/{outputdir}/x64/luna/", f"bin/{outputdir}/x64/sandbox/")
    copy_folder_contents(f"bin/{outputdir}/x64/luna/", f"bin/{outputdir}/x64/apollo/")

    outputdir = "release-windows-x86_64"
    copy_folder_contents(f"bin/{outputdir}/x64/luna/", f"bin/{outputdir}/x64/sandbox/")
    copy_folder_contents(f"bin/{outputdir}/x64/luna/", f"bin/{outputdir}/x64/apollo/")

    outputdir = "distribution-windows-x86_64"
    copy_folder_contents(f"bin/{outputdir}/x64/luna", f"bin/{outputdir}/x64/sandbox/")
    copy_folder_contents(f"bin/{outputdir}/x64/luna/", f"bin/{outputdir}/x64/apollo/")



if __name__ == "__main__":
    try:
        main()
    # Your script code here
    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)

sys.exit(0)
