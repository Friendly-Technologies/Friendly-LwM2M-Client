# Friendly LWM2M client
Wakaama++  is Eclipse Wakaama-based integrated LWM2M solution

For download repository with submodules:  `git clone --recurse-submodules git@github.com:<repo>`  

For running auto setup environment:  `sh ./wpp_env_setup.sh`

For open project wit VS Code:
	1) Start VS Code and go to `File→Open Workspace` from File.
	2) In the open window, specify the path to the file `vs-code-wpp.code-workspace` located in the root of the repository.
	3) After the workspace has loaded, go to `Extensions` (Ctrl+Shift+x), set `Recommended` (Text field: @recommended) in the extensions filter, and then install all recommended extensions.
	4) Let’s move on to the `CMake Tool` extension, the open one contains the entire main interface.
	5) Before the first build of the projects, it is necessary to configure the `Kit` in both projects, for this you need to choose `Wpp Linux Kit` (but in general the compiler depends on the target platform) as compiler for bothe projects. After the  kit is installed, you can start the first building.
