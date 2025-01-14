#pragma once

#include "MeshVbo.h"

class TriangleMeshVbo : public MeshVbo {
protected:

public:
	static string const MESH_NAME;
	TriangleMeshVbo();
	virtual string getMeshName();
	virtual void render();
	virtual void loadData();
};