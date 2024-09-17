# Windows 10; PowerShell 7
# Copy the source files to the destination

# 'pi' is HOST to which the files are sent
scp `
    source/makefile `
    source/st7735s.h `
    source/st7735s.c `
    source/main_example_wpi.c `
    pi:/home/pi/workspace/st7735s/
