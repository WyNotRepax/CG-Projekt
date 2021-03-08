#pragma once

#include "GL/glew.h"
#include <string>
#include "../math/Vector.h"
#include "../math/Matrix.h"

class Shader
{
	//Static
	static GLuint sCurrentShader;
protected:
	static void setUniformVec(GLint location, const float& x, const float& y, const float& z);;
	static void setUniformVec(GLint location, const Vector& v);

	static void setUniformMatrix(GLint location, const float* m);
	static void setUniformMatrix(GLint location, const Matrix& matrix);

	static void setUniformInt(GLint location, const GLuint& id);

	static void setUniformFloat(GLint location, const float& f);

	//Member
protected:
	GLuint mId;
	bool mLoaded;
protected:
	Shader();
	Shader(const std::string& vspath, const std::string& fspath);
	bool load(const std::string& vspath, const std::string& fspath);
public:
	virtual ~Shader();
protected:
	GLint getUniformLocation(const std::string& uniformName);
public:
	virtual void activate();
};

