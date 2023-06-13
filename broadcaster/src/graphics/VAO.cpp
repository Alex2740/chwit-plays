#include "VAO.h"


// Constructor that generates a VAO ID
Graphics::VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}


// Links a VBO Attribute such as a position or color to the VAO
void Graphics::VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}


// Binds the VAO
void Graphics::VAO::Bind()
{
	glBindVertexArray(ID);
}


// Unbinds the VAO
void Graphics::VAO::Unbind()
{
	glBindVertexArray(0);
}


// Deletes the VAO
void Graphics::VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
