#include <fstream>
#include <iostream>
#include "menger.h"

namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger()
{
	// Add additional initialization if you like
}

Menger::~Menger()
{
}

void
Menger::set_nesting_level(int level)
{
	nesting_level_ = level;
	dirty_ = true;
}

bool
Menger::is_dirty() const
{
	return dirty_;
}

void
Menger::set_clean()
{
	dirty_ = false;
}

// FIXME generate Menger sponge geometry
void
Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
			  									std::vector<glm::vec4>& vtx_normals,
                          std::vector<glm::uvec3>& obj_faces) const
{
	obj_vertices.clear();
	vtx_normals.clear();
	obj_faces.clear();
	float minx, miny, minz, maxx, box_length;
	minx = -0.5f;
	miny = -0.5f;
	minz = -0.5f;
	maxx = 0.5f;
	box_length = maxx - minx;
	if (nesting_level_ == 0) {
		drawCube(obj_vertices, vtx_normals, obj_faces, minx, miny, minz, 
			minx + box_length, miny + box_length, minz + box_length, 0);
	}
	else {
		recurse_helper(obj_vertices, vtx_normals, obj_faces, minx, miny, minz,
			box_length /= 3.0f, nesting_level_, 0);
	}
}

int
Menger::recurse_helper(std::vector<glm::vec4>& obj_vertices,
			  							 std::vector<glm::vec4>& vtx_normals,
                       std::vector<glm::uvec3>& obj_faces,
                       float minx, float miny, float minz, float box_length, 
                       int current_depth, int start) const {
	float x, y, z;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (i % 2 + j % 2 + k % 2 < 2) {
					x = minx + (float)i * box_length;
					y = miny + (float)j * box_length;
					z = minz + (float)k * box_length;
					if (current_depth > 1) {
						//Recurse into new smaller cube at new min location, with new box length
						float new_length = box_length / 3.0f;
						start = recurse_helper(obj_vertices, vtx_normals, obj_faces,
                       x, y, z, new_length, current_depth - 1, start);
					}
					else {
						drawCube(obj_vertices, vtx_normals, obj_faces, 
							x, y, z, x + box_length, y + box_length, z + box_length, start);
						start += 36;
					}
				}
			}
		}
	}
	return start;
}

//Draws a single cube given six vertices
void
Menger::drawCube(std::vector<glm::vec4>& obj_vertices,
				 std::vector<glm::vec4>& vtx_normals,
				 std::vector<glm::uvec3>& obj_faces,
				 float minx, float miny, float minz, float maxx, float maxy, float maxz, int start) const {
	//Triangle 1, front facing, bottom right
	obj_vertices.push_back(glm::vec4(minx, miny, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, miny, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start, start + 1, start + 2));

	//Triangle 2, front facing, top left
	obj_vertices.push_back(glm::vec4(minx, miny, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 3, start + 4, start + 5));

	//Triangle 3, left facing, bottom right
	obj_vertices.push_back(glm::vec4(minx, miny, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 6, start + 7, start + 8));

	//Triangle 4, left facing, top left
	obj_vertices.push_back(glm::vec4(minx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 9, start + 10, start + 11));

	//Triangle 5, back facing, bottom right
	obj_vertices.push_back(glm::vec4(minx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 12, start + 13, start + 14));

	//Triangle 6, back facing, top left
	obj_vertices.push_back(glm::vec4(minx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 15, start + 16, start + 17));

	//Triangle 7, right facing, bottom right
	obj_vertices.push_back(glm::vec4(maxx, miny, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 18, start + 19, start + 20));

	//Triangle 8, right facing, top left
	obj_vertices.push_back(glm::vec4(maxx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 21, start + 22, start + 23));

	//Triangle 9, top facing, bottom right
	obj_vertices.push_back(glm::vec4(minx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 24, start + 25, start + 26));

	//Triangle 10, top facing, top left
	obj_vertices.push_back(glm::vec4(minx, maxy, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, maxy, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 27, start + 28, start + 29));

	//Triangle 11, bottom facing, bottom right
	obj_vertices.push_back(glm::vec4(minx, miny, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, miny, maxz, 1.0f));//
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 30, start + 31, start + 32));

	//Triangle 12, bottom facing, top left
	obj_vertices.push_back(glm::vec4(minx, miny, maxz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(minx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_vertices.push_back(glm::vec4(maxx, miny, minz, 1.0f));
	vtx_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	obj_faces.push_back(glm::uvec3(start + 33, start + 34, start + 35));
}