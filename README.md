# Crystal
Crystal Engine

Install Video: https://youtu.be/zVJSuwHtL0Q?si=Cxt2zz4wpusGpQX4

Written install instructions:

1. Download zip or git clone.
2. Try to open the .sln file if you have VS
3. Otherwise go to the scripts folder and run the Generate Project script for your platform
4. Open the project file of your choice and try to build it.

-- COMMON ERRORS --
Some sort of mismatch error with linking. Solution: Go to project settings and change all of the runtime libraries for the projects
to /MD (release & dist) or /MDd (debug)
shaderc dll of some sort not found. Go in your VulkanSDK and copy the dll files in lib into the executable location