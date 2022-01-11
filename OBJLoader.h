#pragma once

static std::pair<std::vector<Vertex>, std::vector<Vertex>> loadOBJ(const char* file_name)
{
	// Vertex portions
	std::vector<glm::fvec3> vertex_positions;
	std::vector<glm::fvec2> vertex_texcoords;
	std::vector<glm::fvec3> vertex_normals;

	// Face vectors
	// Quads
	std::vector<int> vertex_position_indicies;
	std::vector<int> vertex_texcoord_indicies;
	std::vector<int> vertex_normal_indicies;

	// Triangles
	std::vector<int> vertex_position_indicies_t;
	std::vector<int> vertex_texcoord_indicies_t;
	std::vector<int> vertex_normal_indicies_t;

	// Vertex array
	std::vector<Vertex> vertices; // Quad
	std::vector<Vertex> vertices_t; // Triangles

	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	// File open error check
	if (!in_file.is_open())
	{
		throw "ERROR::OBJLOADER::Could not open file.\n";
	}

	// Read one line at a time
	while (std::getline(in_file, line))
	{
		// Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}
		else if (prefix == "o")
		{

		}
		else if (prefix == "s")
		{

		}
		else if (prefix == "use_mtl")
		{

		}
		else if (prefix == "v") // Vertex position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{
			int counter = 0;
			std::vector<int> aux_normals;
			std::vector<int> aux_textures;
			std::vector<int> aux_positions;
			int counter2 = 0;
			while (ss >> temp_glint)
			{
				// Pushing indices into correct arrays
				if (counter == 0)
					aux_positions.push_back(temp_glint);
				else if (counter == 1)
					aux_textures.push_back(temp_glint);
				else if (counter == 2)
					aux_normals.push_back(temp_glint);

				// Handling characters
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					counter2++;
					++counter;
					ss.ignore(1, ' ');
				}

				// Reset the counter
				if (counter > 2)
					counter = 0;
			}
			if (counter2 == 3)
			{
				for (auto i : aux_positions)
				{
					vertex_position_indicies.push_back(i);
				}
				for (auto i : aux_textures)
				{
					vertex_texcoord_indicies.push_back(i);
				}
				for (auto i : aux_normals)
				{
					vertex_normal_indicies.push_back(i);
				}
			}
			else
			{
				for (auto i : aux_positions)
				{
					vertex_position_indicies_t.push_back(i);
				}
				for (auto i : aux_textures)
				{
					vertex_texcoord_indicies_t.push_back(i);
				}
				for (auto i : aux_normals)
				{
					vertex_normal_indicies_t.push_back(i);
				}
			}
		}
		else
		{

		}
	}

	// Build final vertex array (mesh)
	vertices.resize(vertex_position_indicies.size(), Vertex());
	vertices_t.resize(vertex_position_indicies_t.size(), Vertex());

	// Load in all indices
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].position = vertex_positions[vertex_position_indicies[i] - 1];
		vertices[i].texture = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
		vertices[i].normal = vertex_normals[vertex_normal_indicies[i] - 1];
		vertices[i].color = glm::vec3(1.f, 1.f, 1.f);
	}

	// Load in all indices for Triangles
	for (size_t i = 0; i < vertices_t.size(); ++i)
	{
		vertices_t[i].position = vertex_positions[vertex_position_indicies_t[i] - 1];
		vertices_t[i].texture = vertex_texcoords[vertex_texcoord_indicies_t[i] - 1];
		vertices_t[i].normal = vertex_normals[vertex_normal_indicies_t[i] - 1];
		vertices_t[i].color = glm::vec3(1.f, 1.f, 1.f);
	}

	// DEBUG
	std::cout << line << "\n";
	std::cout << "Nr of vertices(QUAD): " << vertices.size() << "\n";
	std::cout << "Nr of vertices(TRIANGLES): " << vertices_t.size() << "\n";

	// Loaded success
	std::cout << "OBJ file loaded!\n";
	return { vertices, vertices_t };
}