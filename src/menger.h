#ifndef MENGER_H
#define MENGER_H

#include <glm/glm.hpp>
#include <vector>

class Menger {
public:
	Menger();
	~Menger();
	void set_nesting_level(int);
	bool is_dirty() const;
	void set_clean();
	void generate_geometry(std::vector<glm::vec4>& obj_vertices,
			       						 std::vector<glm::vec4>& vtx_normals,
	                       std::vector<glm::uvec3>& obj_faces) const;
private:
	int recurse_helper(std::vector<glm::vec4>& obj_vertices,
		  							 std::vector<glm::vec4>& vtx_normals,
                     std::vector<glm::uvec3>& obj_faces,
                     float minx, float miny, float minz, float box_length, 
                     int current_depth, int start) const;
	void drawCube(std::vector<glm::vec4>& obj_vertices,
			       						 std::vector<glm::vec4>& vtx_normals,
	                       std::vector<glm::uvec3>& obj_faces,
	                       float minx, float miny, float minz, 
	                       float maxx, float maxy, float maxz, int start) const;
	int nesting_level_ = 0;
	bool dirty_ = false;
};

#endif
