import os
import subprocess
import pkg_resources
from pathlib import Path


def install(package):
    print(f"Installing {package} module...")
    subprocess.check_call(['python', '-m', 'pip', 'install', package])

def ValidatePackage(package):
    required = { package }
    installed = {pkg.key for pkg in pkg_resources.working_set}
    missing = required - installed
    if missing:
        install(package)

def ValidatePackages():
    ValidatePackage('requests')
    ValidatePackage('fake-useragent')

def FormatPremakeFile(filename, replacmentMapping):
    with open(filename, 'r') as file:
        filedata = file.read()

    for key, value in replacmentMapping.items():
        filedata = filedata.replace(key, value.replace("\\", "/"))

    with open(filename.strip("template"), 'w+') as file:
        file.write(filedata)


def main():
    ValidatePackages()
    
    if Path.cwd().name == 'scripts':
        os.chdir('../')
        
    import Vulkan

    if not Vulkan.isSDKInstalled():
        print("Vulkan SDK not installed.")
        return

    if not Vulkan.areSDKDebugLibsInstalled():
        print("Vulkan SDK debug libs not found.")
        return

    vkIncludeDir = Vulkan.getIncludeDir()
    vkLibDir = Vulkan.getLibDir()
    vkDebugLibDir = Vulkan.getDebugLibDir()


    FormatPremakeFile("Dependencies.lua.template", {"{{vkIncludeDir}}": vkIncludeDir, "{{vkLibDir}}": vkLibDir, "{{vkDebugLibDir}}": vkDebugLibDir})

    print("Running premake...")
    subprocess.call(["vendor/premake/premake5", "vs2022"])


if __name__ == "__main__":
    main()
