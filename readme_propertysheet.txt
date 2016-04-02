PropertySheet Organization

Global_Base:
    Set include path for solution projects 
	Set include path for third party libs 

Global_Debug :
Global_Release:
    Set lib path for solution projects (debug/release)
	Set lib path for third party libs(debug/release)

Memo:
    #1 Include and Lib paths should be set globally. This makes sure all projects have a equal scope
    of available headers/libs. It's also new-project-friendly.

    #2 Do NOT set lib names globally. Let each project choose what she wants. All dependencies finally
    pass down to the executable, where compiler will figure them out at link time.

    #2.5 In fact, I don't need a per-project propertysheet.

    #3 Do not build fat libs. Fat is evil.

----------------------------Sample Dependencies-------------------------------

[Include Path] (Global_Base)
Solution Projects Include Path
    $(SolutionDir)/

ThirdParty Include Path
    $(SolutionDir)/ThirdParty/boost/include/
    $(SolutionDir)/ThirdParty/fbx_sdk_Jan_2015/include

[Lib Path] (Glbal_Debug/Glbal_Release)
Solution Projects Lib Path
    debug: 
        $(SolutionDir)/$(Configuration)
    release:
        $(SolutionDir)/$(Configuration)/

ThirdParty Lib Path
    debug:
        $(SolutionDir)/ThirdParty/boost/lib/
        $(SolutionDir)/ThirdParty/fbx_sdk_Jan_2015/lib/vs2013/x86/debug/
    release:
        $(SolutionDir)/ThirdParty/boost/lib/
        $(SolutionDir)/ThirdParty/fbx_sdk_Jan_2015/lib/vs2013/x86/release/

[Lib Names]
    debug:
        core.lib
        engine.lib
        renerer.lib
        libboost_serialization-vc120-mt-gd-1_59.lib
        libfbxsdk-md.lib

    release:
        core.lib
        engine.lib
        renerer.lib
        libboost_serialization-vc120-mt-1_59.lib
        libfbxsdk-md.lib

Project Dependencies:

Core
    libfbxsdk-md.lib
    d3dcompiler.lib
    dxguid.lib
    libboost_serialization-vc120-mt-1_59.lib | libboost_serialization-vc120-mt-gd-1_59.lib(why not there?)

Engine
    dwmapi.lib
    setupapi.lib

Renderer
    d3d11.lib

DummyDemo(exe)
    Renderer.lib
    Core.lib
    Engine.lib

