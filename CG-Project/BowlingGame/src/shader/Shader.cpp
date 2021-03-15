#include "Shader.h"
#include <fstream>
#include <assert.h>
#include "../Logging.h"

GLuint Shader::sCurrentShader = 0;

// Static helper functions
static GLuint CompileShader(GLenum type, const std::string& source) {
	GLuint id;
	LOG_CALL(id = glCreateShader, type);
	const char* pSource = source.c_str();
	LOG_CALL(glShaderSource, id, 1, &pSource, nullptr);
	LOG_CALL(glCompileShader, id);

	GLint result;
	LOG_CALL(glGetShaderiv, id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint length;
		LOG_CALL(glGetShaderiv, id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		LOG_CALL(glGetShaderInfoLog, id, length, &length, message);
		LOG("Shader Compilation Failed: %s\n", message);
		delete[] message;
		LOG_CALL(glDeleteShader, id);
		return 0;
	}

	return id;
}

static GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
	GLuint programID;
	LOG_CALL(programID = glCreateProgram);
	GLuint temporaryVertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint temporaryFragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	LOG_CALL(glAttachShader, programID, temporaryVertexShaderID);
	LOG_CALL(glAttachShader, programID, temporaryFragmentShaderID);
	LOG_CALL(glLinkProgram, programID);

	LOG_CALL(glDeleteShader, temporaryVertexShaderID);
	LOG_CALL(glDeleteShader, temporaryFragmentShaderID);

	return programID;
}

static std::string LoadSource(const std::string& path) {
	std::ifstream input(path);
	std::string source;
	if (!input) {
		fprintf(stderr, "Could not open file %s\n", path.c_str());
		return std::string();
	}
	input.seekg(0, std::ios::end);
	source.resize(input.tellg());
	input.seekg(0);
	input.read((char*)source.data(), source.size());
	return source;
}

// Member implementations

Shader::Shader() :
	mId(0), mLoaded(false)
{

}

Shader::Shader(const std::string& vspath, const std::string& fspath) :
	Shader()
{
	load(vspath, fspath);
}

bool Shader::load(const std::string& vspath, const std::string& fspath)
{
	std::string vssource = LoadSource(vspath);
	std::string fssource = LoadSource(fspath);
	if (vssource.length() == 0) {
		fprintf(stderr, "Could not load the Vertex shader source from %s\n", vspath.c_str());
		return false;
	}
	if (fssource.length() == 0) {
		fprintf(stderr, "Could not load the Fragment shader source from %s\n", fspath.c_str());
		return false;
	}
	mId = CreateShaderProgram(vssource, fssource);
	return true;
}

Shader::~Shader()
{
	LOG_CALL(glDeleteProgram, mId);
	mId = 0;
}

void Shader::activate()
{
	if (sCurrentShader == mId) {
		return;
	}
	LOG_CALL(glUseProgram, mId);
}

GLint Shader::getUniformLocation(const std::string& uniformName)
{
	GLint loc;
	LOG_CALL(loc = glGetUniformLocation, mId, uniformName.c_str());
	if (loc == -1) {
		LOG("Program ID: %d uniformName: %s, location: %d\n", mId, uniformName.c_str(), loc);
	}
	//
	//assert(loc != -1);
	return loc;
}

void Shader::setUniformVec(GLint location, const float& x, const float& y, const float& z)
{
	LOG_CALL(glUniform3f, location, x, y, z);
}

void Shader::setUniformVec(GLint location, const Vector& v)
{
	Shader::setUniformVec(location, v.X, v.Y, v.Z);
}

void Shader::setUniformMatrix(GLint location, const float* m)
{
	LOG_CALL(glUniformMatrix4fv, location, 1, false, m);
}

void Shader::setUniformMatrix(GLint location, const Matrix& m)
{
	Shader::setUniformMatrix(location, m.m);
}

void Shader::setUniformInt(GLint location, const GLuint& i)
{
	LOG_CALL(glUniform1i, location, i);
}


void Shader::setUniformFloat(GLint location, const float& f) {
	LOG_CALL(glUniform1f, location, f);
}