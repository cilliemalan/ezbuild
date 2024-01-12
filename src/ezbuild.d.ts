
/// <reference no-default-lib="true"/>
/// <reference lib="es2015" />

interface Target {
    type: "executable" | "module" | "staticLibrary";
    name: string;
    sources: string[];
    private_include_directories: string[];
    public_include_directories: string[];
    private_link_directories: string[];
    public_link_directories: string[];
    private_compile_definitions: string[];
    public_compile_definitions: string[];
    private_compile_options: string[];
    public_compile_options: string[];
    dependencies: string[];

    addPublicIncludeDir(...items: string[]): Target;
    addPrivateIncludeDir(...items: string[]): Target;
    addPublicLinkDir(...items: string[]): Target;
    addPrivateLinkDir(...items: string[]): Target;
    addPublicCompileDefinition(...items: string[]): Target;
    addPrivateCompileDefinition(...items: string[]): Target;
    addPublicCompileOptions(...items: string[]): Target;
    addPrivateCompileOptions(...items: string[]): Target;
    link(...items: (string | Target)[]): Target;
    addSource(...files: (string | File)[]);
}

declare function addModule(name: string, files: (string | File)[]): Target;
declare function addExecutable(name: string, files: (string | File)[]): Target;
declare function addStaticLibrary(name: string, files: (string | File)[]): Target;

interface FileSystemObject {
    name: string;
    localName: string;
}

interface File extends FileSystemObject {
    size: number;
    extension: string;
}

interface Directory extends FileSystemObject {
    subDirectory(name: string): Directory;
    subDirectories(): Directory[];
    subDirectoriesRecursive(): Directory[];
    files(): File[];
    filesRecursive(): File[];
    filesWithExtension(...extensions: string[]): File[];
    filesWithExtensionRecursive(...extensions: string[]): File[];
}

interface ToolOutput {
    arguments: Record<string, string | string[]>;
    output: string | string[];
    input: string | string[];
}

interface Tool {
    name: string;
    description: string;
    family: "gcc" | "clang" | "msvc" | "other";
    type: "compiler" | "linker";
    inputExtensions: string[];
    outputExtension: string;
    command: string;
    depfile: string;
    restat: string;

    process(input: string | string[], target: Target): ToolOutput;
}

interface Toolchain {
    name: string;
    tools: Tool[];
}

declare var toolchains: Toolchain[];
declare var toolchain: Toolchain;

declare function setToolchain(toolchain: Toolchain | string);
declare function addToolchain(toolchain: Toolchain);
