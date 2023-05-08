workspace "PuzzleGame"
	  architecture "x64"
	  configurations{"Debug", "Release"}

	project "PuzzleGame"
	location "build"
	kind "WindowedApp"
	language "C++"

	files{
		"src/**.h", "src/**.cpp"
	}

	includedirs{
		"libs/math",
		"libs/ImGUI",
		"libs"
    }

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines {"SYS_WIN"}
		links {
		"d3d11.lib",
		"dxgi.lib",
		"dxguid.lib",
		"d3dcompiler.lib"
	  }

	filter "configurations:Debug"
	       defines {"DEBUG"}
	       symbols "On"
		   
	filter "configurations:Release"
	       defines {"RELEASE"}
	       optimize "On"
