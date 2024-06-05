# Friendly LwM2M Client
Friendly LwM2M Client  is Eclipse Wakaama-based integrated LWM2M solution

For download repository with submodules:  `git clone --recurse-submodules git@github.com:Friendly-Technologies/Friendly-LwM2M-Client.git`

For running auto setup environment:  `sh ./wpp_env_setup.sh`

For open projects with VS Code:
1) Start VS Code and go to `File→Open Workspace` from File.
2) In the open window, specify the path to the file `vs-code-wakaamaplus.code-workspace` located in the `2305-WakaamaPlus` repository.
3) After the workspace has loaded, go to `Extensions` (Ctrl+Shift+x), set `Recommended` (Text field: @recommended) in the extensions filter, and then install all recommended extensions.
4) Let’s move on to the `CMake Tool` extension, the open one contains the entire main interface.
5) Before the first build of the projects, it is necessary to configure the `Kit` in both projects, for this you need to choose `Wpp Linux Kit` (but in general the compiler depends on the target platform) as a compiler for both projects. After the  kit is installed, you can start the first building.
