import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: "../../.."

    ofApp {
        name: { return FileInfo.baseName(sourceDirectory) }

        files: [
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
            'src/filter/Blur.h',
            'src/filter/Blur.cpp',
            'src/filter/Smooth.h',
            'src/filter/Smooth.cpp',
            'src/kinect/Kinect2.h',
            'src/kinect/Kinect2.cpp',
            'src/scenario/Scenario.h',
            'src/scenario/Scenario.cpp',
            'src/tools/Config.h',
            'src/tools/Config.cpp',
            'src/tools/Globals.h',
            'src/tools/Macro.h',
            'src/tools/Window.h',
            'src/tools/Window.cpp',

            'src/views/Detection.h',
            'src/views/Detection.cpp',
            'src/views/Interface.h',
            'src/views/Interface.cpp',
            'src/views/Mapping.h',
            'src/views/Mapping.cpp',
            'src/views/Parameter.h',
            'src/views/Parameter.cpp',
            'src/views/Preview.h',
            'src/views/Preview.cpp',
            'src/views/Projection.h',
            'src/views/Projection.cpp',
        ]

        of.addons: [
            'ofxKinect',
            'ofxOsc',
            'ofxXmlSettings',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project
        of.staticLibraries: []  // static libraries
        of.dynamicLibraries: [] // dynamic libraries

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    property bool makeOF: true  // use makfiles to compile the OF library
                                // will compile OF only once for all your projects
                                // otherwise compiled per project with qbs
    

    property bool precompileOfMain: false  // precompile ofMain.h
                                           // faster to recompile when including ofMain.h 
                                           // but might use a lot of space per project

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
