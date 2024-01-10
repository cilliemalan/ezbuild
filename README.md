# Ezbuild
A very simple build system that utilizes ninja.

## Example Build
Imagine a repository. It has a `src` directory with some source files
in it. In the root of the repo is an `ezbuild.proj` file.
```js

const files = [
    "src/main.c",
    "src/some.c",
    "src/more.c",
    "src/files.c"
]

addExecutable("myapp", files)
```

Now imagine we have some modules we want to group together
for some reason.

```js
const protos = addModule("protos", [
    "src/protos/a.c",
    "src/protos/b.c",
    "src/protos/c.c"
])
protos.publicIncludeDir("src/protos")
protos.privateCompileDefinitions("PROTO_DEF")
protos.privateCompileOptions("-Os")

const maths = addModule("maths", [
    "src/maths/my.c",
    "src/maths/maths.c"
])
maths.publicIncludeDir("src/maths")

const myapp = addExecutable("myapp", [
    "src/main.c",
    "src/some.c",
    "src/more.c",
    "src/files.c"
])

myapp.link(protos)
myapp.link(maths)

```

Things can be chained together.

```js
const protos = addModule("protos", [
        "src/protos/a.c",
        "src/protos/b.c",
        "src/protos/c.c"
    ])
    .publicIncludeDir("src/protos")
    .privateCompileDefinitions("PROTO_DEF")
    .privateCompileOptions("-Os")
```

Dependencies can be referenced by object or by name.

```js
myapp.link("protos")
myapp.link("maths")
```

You can also have multiple files. If the repo root has an `ezbuild.proj`
file, any `.ezbuild` file found in a subdirectory will also be processed.

### Globbing
Globbing (using something like `"src/*.c"`) is generally frowned upon for
build systems. Ezbuild does not support wildcards in arguments, but does support
an alternative that can accomplish the same thing.

```js
// register a module for each subdirectory inside
// the "lib" directory.
const modules = directory("lib").subDirectories()
    .map(dir => addModule(dir.name, dir.filesWithExtension("c")
                    .publicIncludeDir(dir.name)
                    .privateCompileDefinition(`MODULE_${dir.name.toUpperCase())))

// add an executable using each file inside the src directory
// include all subdirectories.
const app = addExecutable(
    "myapp",
    directory("src").filesWithExtensionRecursive("c"))

// link the executable with all modules.
app.link(...modules)
```

### Multiple configurations
If you read the description of the concept below you'll see that there
can be multiple layers of configuration. These are the target environment,
build configuration, and debug level. There can also be different toolchains
which have a specific compiler and linker. The target environments and
build configurations must be set in the `ezbuild.proj` file before any
other commands are called.
```js
setTargetEnvironments("linux", "mac", "windows")
setBuildConfigurations("full", "demo")
```
> [!IMPORTANT]
> These must appear before any other commands.

When multiple configurations are in play, all the configuration files
will be processed multiple times, one for each applicable configuration.
The current target environment, build configuration, or debug level
may be discovered by checking the `targetEnvironment`, `buildConfiguration`,
and `debugLevel` globals.

# Concept
Whenever ezbuild is executed, it will build the _project_ found in
the current directory. A _project_ is identified by a file called
`ezbuild.proj`. If there is no project file in the current directory,
it will be considered as if a blank project file exists.

Within subdirectories, configuration files may be found
called `.ezbuild`. These are implicitly included in the build.
If an `ezbuild.proj` file is found in a subdirectory, that directory
will not implicitly be included in the build (but may be explicitly
included). The `ezbuild.proj` file and `.ezbuild` files are not
interpreted differently and they are interpreted one after the
other (order is undefined).

> [!NOTE]
> Both of these file are javascript files that are interpreted
> when ezbuild is run.

The ezbuild files construct a database of what to build and how.
This information is used to construct a ninja build file in the
build directory (`build` by default) which is then interpreted
by ninja to build whatever needs to be built.

> [!NOTE]
> There is no seperate build and configure step. Everything is
> configure and built at once.

## Configurations, Options, Targets
One of the main reasons we have a plethora of tools involved in
the build process (e.g. make, automake, autoconf, kconfig, etc.)
is to enable the build process to be configurable. This usually
to accomplish one of the following:
- Support multiple platforms (i.e. the project has one specific way
  that it is built, but needs to be able to build on many different
  platforms).
- Support multiple target operating systems (i.e. the project needs to be
  built multiple times, producing the same output, but targetting
  different operating systems).
- Support multiple target hardware platforms (e.g. building firmware for
  multiple revisions of the target hardware).
- Optional features that can be toggled on and off (e.g. a crypto library
  allowing certain features to be excluded to make the build small).
- Turning debug features on and off.
- Support different steps in the software lifecycle, like configure, build,
  and install. Ezbuild does not support this.
- Optional features that are toggled on and off automatically based
  on whether a specific dependency is available. Ezbuild does not 
  support this.
- Generating certain files based on the environment. Ezbuild 
  does not support this.
Regardless of what the ultimate goal is, the build process is usually
the same: to turn source files into object files and link
them together. The main goal of the configuration process (in traditional
tools) is to decide which compiler and linker options to use.

In some build suites (e.g. Makefile based) the goal is to configure the 
source tree to build a specific configuration. If the configuration changes
the whole build changes. Some tools (e.g. Makefile, CMake, KConfig) build out-of-tree,
but still has only one build configuration per build directory.
CMake has the added complication that different generators have a different feature
set regarding this (MSVC can have debug and release configurations, but not others).

### Target Environment
Ezbuild supports configuration of the build process on multiple layers. The topmost layer
is *target environemt*. The idea is to build everything inside the project for each
target environment. A common use is cross-compilation where each target environment
uses its own compiler and toolchain. This is the top layer, and is akin to a kit or toolchain
in CMake. Which compiler and linker, and certain compile options are specified on this level.

### Build Configuration
The layer beneath the target environment is the *build configuration*. Each build configuration
is a different "flavor" of the same output. For example, building the same project
for multiple revisions of hardware, or building the same project in multiple offerings
(e.g. paid version, free version, demo version).

### Debug Level
Sitting on the side of the target environment and build configuration is the *debug level*.
There would typically be only two different options (debug and release) and each would simply add
a few compile options or precompiler defs.
> [!NOTE]
> The default debug levels available are set by the toolchain.

### Layers
The three layers each comprise a dimension in the *build matrix*. They also each have a default
value. The default target environment is *host*, the default build configuration is *default*, and
the default debug level is *debug*.

For each of the layers a subdirectory to the build directory will be created. If there is only one
variant of a specific layer (which is often the case) a subdirectory will not be created.

## Compilers, Targets, Options

### Compilers and Linkers
There are compilers and linkers. A compiler takes an input file and produces an output file. A linker
takes multiple input files and produces one output file. What compiler and linker to use is decided
based on the file extension. A set of compilers and linkers is defined as a *toolchain*.

### Arguments
During the compile or link step, the compiler and linker have certain options. These are things such
as which include directories to use, the target architecture, the optimization level, the input files,
the output files, etc. These are all considered arguments for the compiler or linker and come in
a few different categories:
- Input file(s).
- Output file.
- Dependency metadata output file (compiler).
- Include directories (compiler).
- Linker directories (linker).
- Linked libraries (linker).
- Options.
- definitions (compiler).
When a toolchain is defined, it needs to define how to turn each of these into command line arguments.

### Targets
A *target* is something produced by the build process. There are three types of targets: executable,
library, and module. Executable is an executable file. Library is a static library (which is itself
merely a collection of object files archived together). Module is a kind of "virtual library", which
is to say it can be thought of as a static library but it doesn't actually produce a static library
file as build output.
> [!NOTE] ezbuild does not currently support dynamic libraries.
A target is comprised of a collection of source files.

## Functional Build
The whole build can be thought of as a function:
```
(ezbuild_files, target_environment, build_configuration, debug_level, args) => ninja_file
```
The ezbuild files' job is to produce this function, which will be called for each required
variant of the target environment, build configuration, or debug level requested to be built.

## The Build Process
The `ezbuild.proj` file and in the `.ezbuild` files contain commands
that specify the targets to be built and what files are considered their
inputs. The goal is that these files contain minimal commands, even though
javascript provides considerable capabilities. These commands are a few
javascript functions that allow configuration of the build process. The
build configuration process produces a ninja build file that is used to
build the targets in the project.

The build process begins by scanning all the ezbuild files. If none of them
have been modified since the previous build, the files are ignored and
the project is simply built using ninja. If any of the ezbuild files have
been modified, or if the build is fresh, the ezbuild files are processed
to produce a ninja build file after which ninja is invoked to build the project.
