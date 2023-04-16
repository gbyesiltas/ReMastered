/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== Makefile.am ==================
static const unsigned char temp_binary_data_0[] =
"## Process this file with automake to create Makefile.in\n"
"##\n"
"## This file is part of SoundTouch, an audio processing library for pitch/time adjustments\n"
"## \n"
"## SoundTouch is free software; you can redistribute it and/or modify it under the\n"
"## terms of the GNU General Public License as published by the Free Software\n"
"## Foundation; either version 2 of the License, or (at your option) any later\n"
"## version.\n"
"## \n"
"## SoundTouch is distributed in the hope that it will be useful, but WITHOUT ANY\n"
"## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR\n"
"## A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n"
"## \n"
"## You should have received a copy of the GNU General Public License along with\n"
"## this program; if not, write to the Free Software Foundation, Inc., 59 Temple\n"
"## Place - Suite 330, Boston, MA  02111-1307, USA\n"
"\n"
"\n"
"include $(top_srcdir)/config/am_include.mk\n"
"\n"
"\n"
"# set to something if you want other stuff to be included in the distribution tarball\n"
"EXTRA_DIST=SoundTouch.sln SoundTouch.vcxproj\n"
"\n"
"noinst_HEADERS=AAFilter.h cpu_detect.h cpu_detect_x86.cpp FIRFilter.h RateTransposer.h TDStretch.h PeakFinder.h \\\n"
"    InterpolateCubic.h InterpolateLinear.h InterpolateShannon.h\n"
"\n"
"lib_LTLIBRARIES=libSoundTouch.la\n"
"#\n"
"libSoundTouch_la_SOURCES=AAFilter.cpp FIRFilter.cpp FIFOSampleBuffer.cpp    \\\n"
"    RateTransposer.cpp SoundTouch.cpp TDStretch.cpp cpu_detect_x86.cpp      \\\n"
"    BPMDetect.cpp PeakFinder.cpp InterpolateLinear.cpp InterpolateCubic.cpp \\\n"
"    InterpolateShannon.cpp\n"
"\n"
"# Compiler flags\n"
"AM_CXXFLAGS+=-O3\n"
"\n"
"# Compile the files that need MMX and SSE individually.\n"
"libSoundTouch_la_LIBADD=libSoundTouchMMX.la libSoundTouchSSE.la\n"
"noinst_LTLIBRARIES=libSoundTouchMMX.la libSoundTouchSSE.la\n"
"libSoundTouchMMX_la_SOURCES=mmx_optimized.cpp\n"
"libSoundTouchSSE_la_SOURCES=sse_optimized.cpp\n"
"\n"
"# We enable optimizations by default.\n"
"# If MMX is supported compile with -mmmx.\n"
"# Do not assume -msse is also supported.\n"
"if HAVE_MMX\n"
"libSoundTouchMMX_la_CXXFLAGS = -mmmx $(AM_CXXFLAGS)\n"
"else\n"
"libSoundTouchMMX_la_CXXFLAGS = $(AM_CXXFLAGS)\n"
"endif\n"
"\n"
"# We enable optimizations by default.\n"
"# If SSE is supported compile with -msse.\n"
"if HAVE_SSE\n"
"libSoundTouchSSE_la_CXXFLAGS = -msse $(AM_CXXFLAGS)\n"
"else\n"
"libSoundTouchSSE_la_CXXFLAGS = $(AM_CXXFLAGS)\n"
"endif\n"
"\n"
"# Let the user disable optimizations if he wishes to.\n"
"if !X86_OPTIMIZATIONS\n"
"libSoundTouchMMX_la_CXXFLAGS = $(AM_CXXFLAGS)\n"
"libSoundTouchSSE_la_CXXFLAGS = $(AM_CXXFLAGS)\n"
"endif\n"
"\n"
"# Modify the default 0.0.0 to LIB_SONAME.0.0\n"
"libSoundTouch_la_LDFLAGS=-version-info @LIB_SONAME@\n"
"\n"
"# other linking flags to add\n"
"# noinst_LTLIBRARIES = libSoundTouchOpt.la\n"
"# libSoundTouch_la_LIBADD = libSoundTouchOpt.la\n"
"# libSoundTouchOpt_la_SOURCES = mmx_optimized.cpp sse_optimized.cpp \n"
"# libSoundTouchOpt_la_CXXFLAGS = -O3 -msse -fcheck-new -I../../include\n";

const char* Makefile_am = (const char*) temp_binary_data_0;

//================== SoundTouch.sln ==================
static const unsigned char temp_binary_data_1[] =
"Microsoft Visual Studio Solution File, Format Version 12.00\n"
"# Visual Studio 14\n"
"VisualStudioVersion = 14.0.23107.0\n"
"MinimumVisualStudioVersion = 10.0.40219.1\n"
"Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"SoundTouch\", \"SoundTouch.vcxproj\", \"{68A5DD20-7057-448B-8FE0-B6AC8D205509}\"\n"
"EndProject\n"
"Global\n"
"\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n"
"\t\tDebug|Win32 = Debug|Win32\n"
"\t\tDebug|x64 = Debug|x64\n"
"\t\tRelease|Win32 = Release|Win32\n"
"\t\tRelease|x64 = Release|x64\n"
"\t\tReleaseX64|Win32 = ReleaseX64|Win32\n"
"\t\tReleaseX64|x64 = ReleaseX64|x64\n"
"\tEndGlobalSection\n"
"\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.Debug|Win32.ActiveCfg = Debug|Win32\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.Debug|x64.ActiveCfg = Debug|x64\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.Release|Win32.ActiveCfg = Release|Win32\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.Release|x64.ActiveCfg = Release|x64\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.ReleaseX64|Win32.ActiveCfg = ReleaseX64|Win32\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.ReleaseX64|Win32.Build.0 = ReleaseX64|Win32\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.ReleaseX64|x64.ActiveCfg = ReleaseX64|x64\n"
"\t\t{68A5DD20-7057-448B-8FE0-B6AC8D205509}.ReleaseX64|x64.Build.0 = ReleaseX64|x64\n"
"\tEndGlobalSection\n"
"\tGlobalSection(SolutionProperties) = preSolution\n"
"\t\tHideSolutionNode = FALSE\n"
"\tEndGlobalSection\n"
"EndGlobal\n";

const char* SoundTouch_sln = (const char*) temp_binary_data_1;

//================== SoundTouch.vcxproj ==================
static const unsigned char temp_binary_data_2[] =
"\xef\xbb\xbf<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<Project DefaultTargets=\"Build\" ToolsVersion=\"14.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n"
"  <ItemGroup Label=\"ProjectConfigurations\">\n"
"    <ProjectConfiguration Include=\"Debug|Win32\">\n"
"      <Configuration>Debug</Configuration>\n"
"      <Platform>Win32</Platform>\n"
"    </ProjectConfiguration>\n"
"    <ProjectConfiguration Include=\"Debug|x64\">\n"
"      <Configuration>Debug</Configuration>\n"
"      <Platform>x64</Platform>\n"
"    </ProjectConfiguration>\n"
"    <ProjectConfiguration Include=\"Release|Win32\">\n"
"      <Configuration>Release</Configuration>\n"
"      <Platform>Win32</Platform>\n"
"    </ProjectConfiguration>\n"
"    <ProjectConfiguration Include=\"Release|x64\">\n"
"      <Configuration>Release</Configuration>\n"
"      <Platform>x64</Platform>\n"
"    </ProjectConfiguration>\n"
"  </ItemGroup>\n"
"  <PropertyGroup Label=\"Globals\">\n"
"    <ProjectGuid>{68A5DD20-7057-448B-8FE0-B6AC8D205509}</ProjectGuid>\n"
"    <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>\n"
"  </PropertyGroup>\n"
"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">\n"
"    <ConfigurationType>StaticLibrary</ConfigurationType>\n"
"    <PlatformToolset>v142</PlatformToolset>\n"
"    <UseOfMfc>false</UseOfMfc>\n"
"    <CharacterSet>MultiByte</CharacterSet>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">\n"
"    <ConfigurationType>StaticLibrary</ConfigurationType>\n"
"    <PlatformToolset>v142</PlatformToolset>\n"
"    <UseOfMfc>false</UseOfMfc>\n"
"    <CharacterSet>MultiByte</CharacterSet>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">\n"
"    <ConfigurationType>StaticLibrary</ConfigurationType>\n"
"    <PlatformToolset>v142</PlatformToolset>\n"
"    <UseOfMfc>false</UseOfMfc>\n"
"    <CharacterSet>MultiByte</CharacterSet>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">\n"
"    <ConfigurationType>StaticLibrary</ConfigurationType>\n"
"    <PlatformToolset>v142</PlatformToolset>\n"
"    <UseOfMfc>false</UseOfMfc>\n"
"    <CharacterSet>MultiByte</CharacterSet>\n"
"  </PropertyGroup>\n"
"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n"
"  <ImportGroup Label=\"ExtensionSettings\">\n"
"  </ImportGroup>\n"
"  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"PropertySheets\">\n"
"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n"
"    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\n"
"  </ImportGroup>\n"
"  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"PropertySheets\">\n"
"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n"
"    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\n"
"  </ImportGroup>\n"
"  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"PropertySheets\">\n"
"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n"
"    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\n"
"  </ImportGroup>\n"
"  <ImportGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"PropertySheets\">\n"
"    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n"
"    <Import Project=\"$(VCTargetsPath)Microsoft.CPP.UpgradeFromVC71.props\" />\n"
"  </ImportGroup>\n"
"  <PropertyGroup Label=\"UserMacros\" />\n"
"  <PropertyGroup>\n"
"    <_ProjectFileVersion>14.0.23107.0</_ProjectFileVersion>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\n"
"    <OutDir>$(Platform)\\$(Configuration)\\</OutDir>\n"
"    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\n"
"    <OutDir>$(Platform)\\$(Configuration)\\</OutDir>\n"
"    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>\n"
"    <TargetName>$(ProjectName)_x64</TargetName>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\n"
"    <OutDir>$(Platform)\\$(Configuration)\\</OutDir>\n"
"    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>\n"
"    <TargetName>$(ProjectName)D</TargetName>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\n"
"    <OutDir>$(Platform)\\$(Configuration)\\</OutDir>\n"
"    <IntDir>$(Platform)\\$(Configuration)\\</IntDir>\n"
"    <TargetName>$(ProjectName)D_x64</TargetName>\n"
"  </PropertyGroup>\n"
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\n"
"    <ClCompile>\n"
"      <Optimization>Full</Optimization>\n"
"      <InlineFunctionExpansion>AnySuitable</InlineFunctionExpansion>\n"
"      <IntrinsicFunctions>true</IntrinsicFunctions>\n"
"      <AdditionalIncludeDirectories>..\\..\\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n"
"      <PreprocessorDefinitions>WIN32;NDEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <StringPooling>true</StringPooling>\n"
"      <RuntimeLibrary>MultiThreaded</RuntimeLibrary>\n"
"      <FunctionLevelLinking>true</FunctionLevelLinking>\n"
"      <FloatingPointModel>Fast</FloatingPointModel>\n"
"      <PrecompiledHeader />\n"
"      <PrecompiledHeaderOutputFile>$(OutDir)$(TargetName).pch</PrecompiledHeaderOutputFile>\n"
"      <AssemblerListingLocation>$(OutDir)</AssemblerListingLocation>\n"
"      <ObjectFileName>$(OutDir)</ObjectFileName>\n"
"      <ProgramDataBaseFileName>$(OutDir)</ProgramDataBaseFileName>\n"
"      <WarningLevel>Level3</WarningLevel>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"      <DebugInformationFormat />\n"
"      <CompileAs>Default</CompileAs>\n"
"      <EnableEnhancedInstructionSet>StreamingSIMDExtensions2</EnableEnhancedInstructionSet>\n"
"      <XMLDocumentationFileName>$(IntDir)</XMLDocumentationFileName>\n"
"      <BrowseInformationFile>$(IntDir)</BrowseInformationFile>\n"
"      <MultiProcessorCompilation>true</MultiProcessorCompilation>\n"
"    </ClCompile>\n"
"    <ResourceCompile>\n"
"      <PreprocessorDefinitions>NDEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <Culture>0x040b</Culture>\n"
"    </ResourceCompile>\n"
"    <Lib>\n"
"      <OutputFile>$(OutDir)$(TargetName)$(TargetExt)</OutputFile>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"    </Lib>\n"
"    <PostBuildEvent>\n"
"      <Command>if not exist ..\\..\\lib mkdir ..\\..\\lib\n"
"copy $(OutDir)$(TargetName)$(TargetExt) ..\\..\\lib</Command>\n"
"    </PostBuildEvent>\n"
"  </ItemDefinitionGroup>\n"
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\n"
"    <Midl>\n"
"      <TargetEnvironment>X64</TargetEnvironment>\n"
"    </Midl>\n"
"    <ClCompile>\n"
"      <Optimization>Full</Optimization>\n"
"      <InlineFunctionExpansion>AnySuitable</InlineFunctionExpansion>\n"
"      <IntrinsicFunctions>true</IntrinsicFunctions>\n"
"      <AdditionalIncludeDirectories>..\\..\\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n"
"      <PreprocessorDefinitions>WIN32;NDEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <StringPooling>true</StringPooling>\n"
"      <RuntimeLibrary>MultiThreaded</RuntimeLibrary>\n"
"      <FunctionLevelLinking>true</FunctionLevelLinking>\n"
"      <FloatingPointModel>Fast</FloatingPointModel>\n"
"      <PrecompiledHeader />\n"
"      <PrecompiledHeaderOutputFile>$(OutDir)$(TargetName).pch</PrecompiledHeaderOutputFile>\n"
"      <AssemblerListingLocation>$(OutDir)</AssemblerListingLocation>\n"
"      <ObjectFileName>$(OutDir)</ObjectFileName>\n"
"      <ProgramDataBaseFileName>$(OutDir)</ProgramDataBaseFileName>\n"
"      <WarningLevel>Level3</WarningLevel>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"      <DebugInformationFormat />\n"
"      <CompileAs>Default</CompileAs>\n"
"      <EnableEnhancedInstructionSet>\n"
"      </EnableEnhancedInstructionSet>\n"
"      <XMLDocumentationFileName>$(IntDir)</XMLDocumentationFileName>\n"
"      <BrowseInformationFile>$(IntDir)</BrowseInformationFile>\n"
"      <MultiProcessorCompilation>true</MultiProcessorCompilation>\n"
"    </ClCompile>\n"
"    <ResourceCompile>\n"
"      <PreprocessorDefinitions>NDEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <Culture>0x040b</Culture>\n"
"    </ResourceCompile>\n"
"    <Lib>\n"
"      <OutputFile>$(OutDir)$(TargetName)$(TargetExt)</OutputFile>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"    </Lib>\n"
"    <PostBuildEvent>\n"
"      <Command>if not exist ..\\..\\lib mkdir ..\\..\\lib\n"
"copy $(OutDir)$(TargetName)$(TargetExt) ..\\..\\lib</Command>\n"
"    </PostBuildEvent>\n"
"  </ItemDefinitionGroup>\n"
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\n"
"    <ClCompile>\n"
"      <Optimization>Disabled</Optimization>\n"
"      <AdditionalIncludeDirectories>..\\..\\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n"
"      <PreprocessorDefinitions>WIN32;_DEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <BasicRuntimeChecks>EnableFastChecks</BasicRuntimeChecks>\n"
"      <RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>\n"
"      <FloatingPointModel>Fast</FloatingPointModel>\n"
"      <PrecompiledHeader />\n"
"      <PrecompiledHeaderOutputFile>$(OutDir)$(TargetName).pch</PrecompiledHeaderOutputFile>\n"
"      <AssemblerListingLocation>$(OutDir)</AssemblerListingLocation>\n"
"      <ObjectFileName>$(OutDir)</ObjectFileName>\n"
"      <ProgramDataBaseFileName>$(OutDir)</ProgramDataBaseFileName>\n"
"      <BrowseInformation>true</BrowseInformation>\n"
"      <WarningLevel>Level3</WarningLevel>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"      <DebugInformationFormat>EditAndContinue</DebugInformationFormat>\n"
"      <CompileAs>Default</CompileAs>\n"
"      <EnableEnhancedInstructionSet>StreamingSIMDExtensions2</EnableEnhancedInstructionSet>\n"
"      <XMLDocumentationFileName>$(IntDir)</XMLDocumentationFileName>\n"
"      <BrowseInformationFile>$(IntDir)</BrowseInformationFile>\n"
"      <MultiProcessorCompilation>true</MultiProcessorCompilation>\n"
"    </ClCompile>\n"
"    <ResourceCompile>\n"
"      <PreprocessorDefinitions>_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <Culture>0x040b</Culture>\n"
"    </ResourceCompile>\n"
"    <Lib>\n"
"      <OutputFile>$(OutDir)$(TargetName)$(TargetExt)</OutputFile>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"    </Lib>\n"
"    <PostBuildEvent>\n"
"      <Command>if not exist ..\\..\\lib mkdir ..\\..\\lib\n"
"copy $(OutDir)$(TargetName)$(TargetExt) ..\\..\\lib</Command>\n"
"    </PostBuildEvent>\n"
"  </ItemDefinitionGroup>\n"
"  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\n"
"    <Midl>\n"
"      <TargetEnvironment>X64</TargetEnvironment>\n"
"    </Midl>\n"
"    <ClCompile>\n"
"      <Optimization>Disabled</Optimization>\n"
"      <AdditionalIncludeDirectories>..\\..\\include;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>\n"
"      <PreprocessorDefinitions>WIN32;_DEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <BasicRuntimeChecks>EnableFastChecks</BasicRuntimeChecks>\n"
"      <RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>\n"
"      <FloatingPointModel>Fast</FloatingPointModel>\n"
"      <PrecompiledHeader />\n"
"      <PrecompiledHeaderOutputFile>$(OutDir)$(TargetName).pch</PrecompiledHeaderOutputFile>\n"
"      <AssemblerListingLocation>$(OutDir)</AssemblerListingLocation>\n"
"      <ObjectFileName>$(OutDir)</ObjectFileName>\n"
"      <ProgramDataBaseFileName>$(OutDir)</ProgramDataBaseFileName>\n"
"      <BrowseInformation>true</BrowseInformation>\n"
"      <WarningLevel>Level3</WarningLevel>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>\n"
"      <CompileAs>Default</CompileAs>\n"
"      <EnableEnhancedInstructionSet>\n"
"      </EnableEnhancedInstructionSet>\n"
"      <XMLDocumentationFileName>$(IntDir)</XMLDocumentationFileName>\n"
"      <BrowseInformationFile>$(IntDir)</BrowseInformationFile>\n"
"      <MultiProcessorCompilation>true</MultiProcessorCompilation>\n"
"    </ClCompile>\n"
"    <ResourceCompile>\n"
"      <PreprocessorDefinitions>_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n"
"      <Culture>0x040b</Culture>\n"
"    </ResourceCompile>\n"
"    <Lib>\n"
"      <OutputFile>$(OutDir)$(TargetName)$(TargetExt)</OutputFile>\n"
"      <SuppressStartupBanner>true</SuppressStartupBanner>\n"
"    </Lib>\n"
"    <PostBuildEvent>\n"
"      <Command>if not exist ..\\..\\lib mkdir ..\\..\\lib\n"
"copy $(OutDir)$(TargetName)$(TargetExt) ..\\..\\lib</Command>\n"
"    </PostBuildEvent>\n"
"  </ItemDefinitionGroup>\n"
"  <ItemGroup>\n"
"    <ClCompile Include=\"AAFilter.cpp\">\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">MaxSpeed</Optimization>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">MaxSpeed</Optimization>\n"
"    </ClCompile>\n"
"    <ClCompile Include=\"BPMDetect.cpp\">\n"
"      <DisableSpecificWarnings Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">4996</DisableSpecificWarnings>\n"
"      <DisableSpecificWarnings Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">4996</DisableSpecificWarnings>\n"
"      <DisableSpecificWarnings Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">4996</DisableSpecificWarnings>\n"
"      <DisableSpecificWarnings Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">4996</DisableSpecificWarnings>\n"
"    </ClCompile>\n"
"    <ClCompile Include=\"cpu_detect_x86.cpp\" />\n"
"    <ClCompile Include=\"FIFOSampleBuffer.cpp\">\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">MaxSpeed</Optimization>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">MaxSpeed</Optimization>\n"
"    </ClCompile>\n"
"    <ClCompile Include=\"FIRFilter.cpp\">\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">MaxSpeed</Optimization>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">MaxSpeed</Optimization>\n"
"    </ClCompile>\n"
"    <ClCompile Include=\"InterpolateCubic.cpp\" />\n"
"    <ClCompile Include=\"InterpolateLinear.cpp\" />\n"
"    <ClCompile Include=\"InterpolateShannon.cpp\" />\n"
"    <ClCompile Include=\"mmx_optimized.cpp\" />\n"
"    <ClCompile Include=\"PeakFinder.cpp\" />\n"
"    <ClCompile Include=\"RateTransposer.cpp\">\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">MaxSpeed</Optimization>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">MaxSpeed</Optimization>\n"
"    </ClCompile>\n"
"    <ClCompile Include=\"SoundTouch.cpp\">\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">MaxSpeed</Optimization>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">MaxSpeed</Optimization>\n"
"    </ClCompile>\n"
"    <ClCompile Include=\"sse_optimized.cpp\" />\n"
"    <ClCompile Include=\"TDStretch.cpp\">\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">Disabled</Optimization>\n"
"      <BasicRuntimeChecks Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">EnableFastChecks</BasicRuntimeChecks>\n"
"      <BrowseInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">true</BrowseInformation>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">MaxSpeed</Optimization>\n"
"      <Optimization Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">MaxSpeed</Optimization>\n"
"    </ClCompile>\n"
"  </ItemGroup>\n"
"  <ItemGroup>\n"
"    <ClInclude Include=\"..\\..\\include\\BPMDetect.h\" />\n"
"    <ClInclude Include=\"..\\..\\include\\FIFOSampleBuffer.h\" />\n"
"    <ClInclude Include=\"..\\..\\include\\FIFOSamplePipe.h\" />\n"
"    <ClInclude Include=\"..\\..\\include\\SoundTouch.h\" />\n"
"    <ClInclude Include=\"..\\..\\include\\STTypes.h\" />\n"
"    <ClInclude Include=\"AAFilter.h\" />\n"
"    <ClInclude Include=\"cpu_detect.h\" />\n"
"    <ClInclude Include=\"FIRFilter.h\" />\n"
"    <ClInclude Include=\"InterpolateCubic.h\" />\n"
"    <ClInclude Include=\"InterpolateLinear.h\" />\n"
"    <ClInclude Include=\"InterpolateShannon.h\" />\n"
"    <ClInclude Include=\"PeakFinder.h\" />\n"
"    <ClInclude Include=\"RateTransposer.h\" />\n"
"    <ClInclude Include=\"TDStretch.h\" />\n"
"  </ItemGroup>\n"
"  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n"
"  <ImportGroup Label=\"ExtensionTargets\">\n"
"  </ImportGroup>\n"
"</Project>";

const char* SoundTouch_vcxproj = (const char*) temp_binary_data_2;

//================== soundtouch_config.h.in ==================
static const unsigned char temp_binary_data_3[] =
"/* Use Float as Sample type */\n"
"#undef SOUNDTOUCH_FLOAT_SAMPLES\n"
"\n"
"/* Use Integer as Sample type */\n"
"#undef SOUNDTOUCH_INTEGER_SAMPLES\n";

const char* soundtouch_config_h_in = (const char*) temp_binary_data_3;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x1a187401:  numBytes = 2804; return Makefile_am;
        case 0x1e03f806:  numBytes = 1393; return SoundTouch_sln;
        case 0x9bb52799:  numBytes = 20000; return SoundTouch_vcxproj;
        case 0x6fff950a:  numBytes = 131; return soundtouch_config_h_in;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "Makefile_am",
    "SoundTouch_sln",
    "SoundTouch_vcxproj",
    "soundtouch_config_h_in"
};

const char* originalFilenames[] =
{
    "Makefile.am",
    "SoundTouch.sln",
    "SoundTouch.vcxproj",
    "soundtouch_config.h.in"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
