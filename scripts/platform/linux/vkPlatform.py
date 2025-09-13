import tarfile
from pathlib import Path
import os
from tarfile import TarFile


def installVulkanSDK(sdk_file_path: Path, vulkan_sdk_download_dir: Path, ae_vulkan_version: str):
    print(f"Extracting {sdk_file_path}...")
    extract_to_path = vulkan_sdk_download_dir
    extracted_sdk_path = extract_to_path / ae_vulkan_version

    try:
        with tarfile.open(sdk_file_path, 'r:xz') as tar:
            tar.extractall(path=extract_to_path)
        print(f"Successfully extracted Vulkan SDK to {extract_to_path}")

        if not extracted_sdk_path.is_dir():
            print(f"Error: Expected SDK directory '{extracted_sdk_path}' not found after extraction.")
            print(f"Please check the contents of '{extract_to_path}'. The archive may have an unexpected structure.")
            print("You may need to set the VULKAN_SDK environment variable manually after locating the SDK.")
            try:
                print(f"Contents of '{extract_to_path}':")
                for item in extract_to_path.iterdir():
                    print(f"  - {item.name}")
            except Exception as list_e:
                print(f"    Could not list contents of '{extract_to_path}': {list_e}")
            return

    except tarfile.ReadError as e:
        print(f"Error: Failed to read {sdk_file_path}. It might be corrupted or not a valid tar.gz file.")
        print(f"Exception details: {e}")
    except FileNotFoundError:
        print(f"Error: Downloaded file {sdk_file_path} not found for extraction.")
    except Exception as e:
        print(f"An error occurred during extraction: {e}")

    # execute vulkansdk to install the SDK
   #   os.execve(f"{extracted_sdk_path}/vulkansdk", [f"{extracted_sdk_path}/vulkansdk", "--maxjobs"], os.environ)

# def areSDKDebugLibsInstalled(sdk_path: Path) -> bool:
#     lib_dir = sdk_path / "lib"
#
#     if not lib_dir.is_dir():
#         print(f"Vulkan SDK library directory '{lib_dir}' not found.")
#         print(f"Ensure VULKAN_SDK ('{sdk_path}') is set correctly by sourcing 'setup-env.sh' from the SDK root.")
#         return False
#
#     lib_vulkan_so = lib_dir / "libvulkan.so.1"
#     if not lib_vulkan_so.exists():
#          lib_vulkan_so = lib_dir / "libvulkan.so"
#
#     if lib_vulkan_so.exists():
#         print(f"Vulkan SDK libraries (e.g., {lib_vulkan_so.name}) found in {lib_dir}.")
#         print("Debug versions/symbols are typically included or managed by the system/build on Linux.")
#         return True
#     else:
#         print(f"Could not find representative Vulkan library (libvulkan.so or libvulkan.so.1) in {lib_dir}.")
#         print("The Vulkan SDK may not be installed correctly, or VULKAN_SDK is not pointing to the correct location.")
#         return False

