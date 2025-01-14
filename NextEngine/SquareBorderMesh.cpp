#include "SquareBorderMesh.h"

string const SquareBorderMesh::MESH_NAME = "square border";

SquareBorderMesh::SquareBorderMesh() {
	//SquareMeshVbo::loadData();
}

void SquareBorderMesh::render() {
	if (this->posAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->posVboId);
		glVertexAttribPointer(this->posAttribId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	}
	if (this->texAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->texVboId);
		glVertexAttribPointer(this->texAttribId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	}
	glDrawArrays(GL_LINE_LOOP, 0, 4);
}