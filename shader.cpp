#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//#include "GL/glew.h" - using QOpenGLFunctions instead

#include "logger.h" //For our utility Logger class

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    //Get the instance of the Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Open files and check for errors
    vShaderFile.open( vertexPath );
    if(!vShaderFile)
        mLogger->logText("ERROR SHADER FILE " + std::string(vertexPath) + " NOT SUCCESFULLY READ", LogType::REALERROR);
    fShaderFile.open( fragmentPath );
    if(!fShaderFile)
        mLogger->logText("ERROR SHADER FILE " + std::string(fragmentPath) + " NOT SUCCESFULLY READ", LogType::REALERROR);
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf( );
    fShaderStream << fShaderFile.rdbuf( );
    // close file handlers
    vShaderFile.close( );
    fShaderFile.close( );
    // Convert stream into string
    vertexCode = vShaderStream.str( );
    fragmentCode = fShaderStream.str( );

    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );

    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        mLogger->logText("ERROR SHADER VERTEX " + std::string(vertexPath) +
                         " COMPILATION_FAILED\n" + infoLog, LogType::REALERROR);
    }
    // Fragment shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        mLogger->logText("ERROR SHADER VERTEX " + std::string(fragmentPath) +
                         " COMPILATION_FAILED\n" + infoLog, LogType::REALERROR);
    }

    // Shader Program linking
    this->mProgram = glCreateProgram();
    glAttachShader( this->mProgram, vertex );
    glAttachShader( this->mProgram, fragment );
    glLinkProgram( this->mProgram );
    // Print linking errors if any
    glGetProgramiv( this->mProgram, GL_LINK_STATUS, &success );

    //making nice output name for printing:
    std::string shadername{vertexPath};
    shadername.resize(shadername.size()-5); //deleting ".vert"

    if (!success)
    {
        glGetProgramInfoLog( this->mProgram, 512, nullptr, infoLog );
        mLogger->logText("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" +
                          shadername +  "\n   " + infoLog, LogType::REALERROR);
    }
    else
    {
       mLogger->logText("GLSL shader " + shadername + " was successfully compiled");
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    // The shader program is now on the GPU and we reference it by using the mProgram variable
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    //Get the instance of the Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    // Open files and check for errors
    vShaderFile.open( vertexPath );
    if(!vShaderFile)
        mLogger->logText("ERROR SHADER FILE " + std::string(vertexPath) + " NOT SUCCESFULLY READ", LogType::REALERROR);
    fShaderFile.open( fragmentPath );
    if(!fShaderFile)
        mLogger->logText("ERROR SHADER FILE " + std::string(fragmentPath) + " NOT SUCCESFULLY READ", LogType::REALERROR);
    gShaderFile.open( geometryPath );
//    if(!gShaderFile)
//        mLogger->logText("ERROR SHADER FILE " + std::string(geometryPath) + " NOT SUCCESFULLY READ", LogType::REALERROR);
    std::stringstream vShaderStream, fShaderStream, gShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf( );
    fShaderStream << fShaderFile.rdbuf( );
//    gShaderStream << gShaderFile.rdbuf( );
    // close file handlers
    vShaderFile.close( );
    fShaderFile.close( );
//    gShaderFile.close( );
    // Convert stream into string
    vertexCode = vShaderStream.str( );
    fragmentCode = fShaderStream.str( );
//    geometryCode = gShaderStream.str( );

    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );
//    const GLchar * gShaderCode = geometryCode.c_str( );

    // 2. Compile shaders
    GLuint vertex, fragment, geometry;
    GLint success;
    GLchar infoLog[512];

    // Vertex shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        mLogger->logText("ERROR SHADER VERTEX " + std::string(vertexPath) +
                         " COMPILATION_FAILED\n" + infoLog, LogType::REALERROR);
    }

    // Fragment shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        mLogger->logText("ERROR SHADER VERTEX " + std::string(fragmentPath) +
                         " COMPILATION_FAILED\n" + infoLog, LogType::REALERROR);
    }

//    // Geometry shader
//    geometry = glCreateShader( GL_GEOMETRY_SHADER );
//    glShaderSource( geometry, 1, &gShaderCode, nullptr);
//    glCompileShader( geometry );
//    // Print compile errors if any
//    glGetShaderiv( geometry, GL_COMPILE_STATUS, &success );
//    if ( !success )
//    {
//        glGetShaderInfoLog( geometry, 512, nullptr, infoLog );
//        mLogger->logText("ERROR SHADER GEOMETRY " + std::string(geometryPath) +
//                         " COMPILATION_FAILED\n" + infoLog, LogType::REALERROR);
//    }

    // Shader Program linking
    this->mProgram = glCreateProgram();
    glAttachShader( this->mProgram, vertex );
    glAttachShader( this->mProgram, fragment );
//    glAttachShader( this->mProgram, geometry );
    glLinkProgram( this->mProgram );
    // Print linking errors if any
    glGetProgramiv( this->mProgram, GL_LINK_STATUS, &success );

    //making nice output name for printing:
    std::string shadername{vertexPath};
    shadername.resize(shadername.size()-5); //deleting ".vert"

    if (!success)
    {
        glGetProgramInfoLog( this->mProgram, 512, nullptr, infoLog );
        mLogger->logText("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" +
                          shadername +  "\n   " + infoLog, LogType::REALERROR);
    }
    else
    {
       mLogger->logText("GLSL shader " + shadername + " was successfully compiled");
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    // The shader program is now on the GPU and we reference it by using the mProgram variable
    glDeleteShader( vertex );
    glDeleteShader( fragment );
//    glDeleteShader( geometry );
}

void Shader::use()
{
    glUseProgram( this->mProgram );
}

GLuint Shader::getProgram() const
{
    return mProgram;
}
