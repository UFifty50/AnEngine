import os
if os.name == 'nt':
    os.system('..\vendor\premake\premake5.exe vs2022')
elif os.name == 'posix':
    os.system('../vendor/premake/premake5.exe gmake2')
else:
    print('Unsupported OS')

input("Press any key to continue...")
