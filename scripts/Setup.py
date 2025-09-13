import os
import sys
import subprocess
import pkg_resources
from pathlib import Path

# Import platform-specific helpers for setup tasks
if sys.platform.startswith('win'):
    from platform.windows import setupHelpers
elif sys.platform.startswith('linux'):
    from platform.linux import setupHelpers
else:
    setupHelpers = None # Or a default module with no-op or generic functions

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

def FormatPremakeFile(filename, replacementMapping):
    filedata = None
    with open(filename, 'r') as file:
        filedata = file.read()

    for key, value in replacementMapping.items():
        filedata = filedata.replace(key, value.replace("\\", "/"))

    with open(filename.strip(".template"), 'w+') as file:
        file.write(filedata)

def CreateDependencyLink(linkDir: Path, linkName: str, targetDir: Path):
    if not targetDir.exists():
        targetDir.mkdir()

    if not setupHelpers:
        print(f"Setup helpers not available for platform {sys.platform}. Cannot create dependency link.")
        return ""

    if not linkDir.exists():
        linkDir.mkdir(parents=True, exist_ok=True)

    linkPath = linkDir / linkName

    setupHelpers.removeDependencyLink(linkPath)
    print(f"Creating link at {linkPath} pointing to {targetDir}")
    resolvedLinkPath = setupHelpers.createDependencyLink(linkPath, targetDir)

    if not resolvedLinkPath:
        print(f"Failed to create link for {linkName} at {linkDir}")
        return ""

    return str(Path(resolvedLinkPath).resolve()) # Ensure we return a resolved absolute path string

def main():
    ValidatePackages()
    
    # the root directory (.../AnEngine/scripts/Setup.py -> .../AnEngine/)
    projectRootDir = os.path.dirname(os.path.dirname(os.path.abspath(sys.argv[0])))
    os.chdir(projectRootDir)
        
    import Vulkan

    if not Vulkan.isSDKInstalled():
        print("Vulkan SDK not installed.")
        return

#    if not Vulkan.areSDKDebugLibsInstalled():
#        print("Vulkan SDK debug libs not found.")
#        return

    vkIncludeDir = Vulkan.getIncludeDir()
  #  vkGLMIncludeDir = CreateDependencyLink(Path("linkedIncludes/glm/"), "glm", Path(Vulkan.getIncludeDir("glm")))
    vkLibDir = Vulkan.getLibDir()
    vkDebugLibDir = Vulkan.getDebugLibDir()


    FormatPremakeFile("Dependencies.lua.template", 
        {
            "{{vkIncludeDir}}": vkIncludeDir,
         #   "{{vkGLMIncludeDir}}": vkGLMIncludeDir,
            "{{vkLibDir}}": vkLibDir,
            "{{vkDebugLibDir}}": vkDebugLibDir
        }
    )

    print("Running premake...")
    subprocess.call(["vendor/premake/premake5", "vs2022"])


if __name__ == "__main__":
    main()
