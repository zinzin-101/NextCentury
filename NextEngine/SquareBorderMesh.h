#pragma once

#include "SquareMeshVbo.h"

class SquareBorderMesh : public SquareMeshVbo {
	public:
		SquareBorderMesh();
		static string const MESH_NAME;
		virtual void render();
};