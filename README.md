# ezbuild
A very simple build system that utilizes ninja.

## Concept
Whenever ezbuild is executed, it will build the _project_ found in
the current directory. A _project_ is identified by a file called
`ezbuild.proj`. If there is no project in the current directory
the parent directories will be searched until one is found and
that built accordingly.

Within subdirectories, configuration files may be found
called `.ezbuild`. These are implicitly included in the build.

Both of these files are javascript files that are interpreted
by ezbuild. The task of these files are to register _targets_.
A _target_ is the output of the build process. There are three
types of targets, _executables_, _libraries_ and _modules_. An _executable_
is an executable produced by building all inputs and linking them
together. A _library_ is a static library (which is a bunch of object
files archived together). A _module_ is a virtual target that acts
the same as a library, but doesn't actually archive the files.

> [!NOTE]
> only use _library_ targets if you want to produce a static
> library file as build output. For internal reusable components, use
> a _module_.

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
the project is simply buily using ninja. If any of the ezbuild files have
been modified, or if the build is fresh, the ezbuild files are processed
to produce a ninja build file after which ninja is invoked to build the project.




