#include "Mesh.h"

Mesh::Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices) {
	myIndexCount = numIndices;
	myVertexCount = numVerts;


	// Create and bind our vertex array
	glCreateVertexArrays(1, &myVao);
	glBindVertexArray(myVao);

	// Create 2 buffers, 1 for vertices and the other for indices
	glCreateBuffers(2, myBuffers);

	// Bind and buffer our vertex data
	glBindBuffer(GL_ARRAY_BUFFER, myBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Bind and buffer our index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	// Get a null vertex to get member offsets from
	Vertex* vert = nullptr;
	
	// Enable vertex attribute 0
	glEnableVertexAttribArray(0);
	// Our first attribute is 3 floats, the distance between 
	// them is the size of our vertex, and they will map to the position in our vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Position)); 

	// Enable vertex attribute 1
	glEnableVertexAttribArray(1);
	// Our second attribute is 4 floats, the distance between 
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &(vert->Color));
	
	// Unbind our VAO
	glBindVertexArray(0);
}

Mesh::~Mesh() {
	// Clean up our buffers
	glDeleteBuffers(2, myBuffers);
	// Clean up our VAO
	glDeleteVertexArrays(1, &myVao);
}

void Mesh::Draw() {
	// Bind the mesh
	glBindVertexArray(myVao);
	// Draw all of our vertices as triangles, our indexes are unsigned ints (uint32_t)
	glDrawElements(GL_TRIANGLES, myIndexCount, GL_UNSIGNED_INT, nullptr);
}
