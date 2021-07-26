/*  by Alun Evans 2016 LaSalle (aevanss@salleurl.edu) */

#include <string>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "imageloader.h"

//include some standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

//include OpenGL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//include some custom code files
#include "glfunctions.h" //include all OpenGL stuff
#include "Shader.h" // class to compile shaders

//include maths library
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//include some custom code files
#include "glfunctions.h" //include all OpenGL stuff
#include "Shader.h" // class to compile shaders


std::string basepath = "assets/";
std::string inputfile = basepath + "sphere.obj";
std::string inputfile1 = basepath + "sphere.obj";
std::vector < tinyobj::shape_t > shapes;
std::vector  < tinyobj::material_t > materials;
std::string err;

bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str(), basepath.c_str());
bool ret1 = tinyobj::LoadObj(shapes, materials, err, inputfile1.c_str(), basepath.c_str());

using namespace std;
using namespace glm;

//global variables to help us do things
int g_ViewportWidth = 960; int g_ViewportHeight = 960; // Default window size, in pixels
double eix_x, eix_y;  //variables storing mouse position
const vec3 g_backgroundColor(0.2f, 0.2f, 0.2f); // background colour - a GLM 3-component vector
vec3 g_light_dir(110, 110, 100);


//Camera vectors 
glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 c_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 eye = glm::vec3(0.0f, 0.0f, 6.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 side = glm::normalize(glm::cross(up, c_front));
glm::vec3 top = glm::cross(c_front, side);


float x = 0, y = 0, z = -2;
float x1 = -0.5, z1 = -1;


float x_mercury_planet = -4, y_mercury_planet = 0;
float x_venus_planet = -4, y_venus_planet = 0;
float x_earth = -4, y_earth = 0;
float x_mart = -4, y_mart = 0;
float x_jupiter_planet = -4, y_jupiter_planet = 0;
float x_saturn_planet = -4, y_saturn_planet = 0;
float x_uranus_planet = -4, y_uranus_planet = 0;
float x_neptune_planet = -4, y_neptune_planet = 0;
float x_pluton_planet = -4, y_pluton_planet = 0;
float ang = 0;
double time;
bool press;

//Rotation camera 
bool firstMouse = true;
float rotation_x = -90.0f;
float rotation_y = 0.0f;
float final_x = 512.0f / 2.0;
float final_y = 512.0f / 2.0;
float camp_visio = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


GLuint texture_id_earth;
GLuint texture_id_universe;
GLuint texture_id_sun;
GLuint texture_id_jupiter_planet;
GLuint texture_id_mars_planet;
GLuint texture_id_mercury_planet;
GLuint texture_id_neptune_planet;
GLuint texture_id_saturn_planet;
GLuint texture_id_uranus_planet;
GLuint texture_id_venus_planet;
GLuint texture_id_pluton_planet;

GLuint g_simpleShader = 0; //shader identifier
GLuint g_simpleShaderUniverse = 0;
GLuint g_Vao = 0; //vao
GLuint g_Vao1 = 0; //vao
GLuint g_NumTriangles = 0; //  Numbre of triangles we are painting.

glm::mat4 projection_matrix = glm::perspective(
	45.0f, //Field of view
	1.0f, //Aspect ratio
	0.1f, //near plane (distance from camera)
	50.0f //Far plane (distance from camera)
);

						   // ------------------------------------------------------------------------------------------
						   // This function manually creates a square geometry (defined in the array vertices[])
						   // ------------------------------------------------------------------------------------------
void load()
{
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	std::cout << "# of shapes : " << shapes.size() << std::endl;

	//**********************
	// CODE TO SET GEOMETRY
	//**********************

	const GLfloat vertices[] = {

		//Vertices
		0.5f, -0.5f, 1.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,


		//Tejado
		-0.8f,0.5f,0.0f, //Abajo izq
		0.0f,1.0f,0.0f, //Arriba medio
		0.8f,0.5f,0.0f, //Abajo derech


		//Puerta

		-0.25f,-0.5f,0.0f, //abajo izq
		-0.25f,0.0f,0.0f, //arriba izq
		0.25f,0.0f,0.0f, //arriba derch
		0.25f,-0.5f,0.0f, //abajo derch


		 //Suelo
		-1.0f,-0.5f,0.0f,//arriba izq
		-1.0f,-1.0f,0.0f,//abajo izq
		1.0f,-0.5f,0.0f, //arriba derecha
		1.0f,-1.0f,0.0f, //abajo derecha


		//Sol
		-1.0f,0.5,0.0f,
		-1.0f,1.0f,0.0f,
		-0.5f,1.0f,0.0f,


		//ventana
		0.2f,0.25f,0.0f, //Abajo izq
		0.2f,0.45f,0.0f, //arriba iz
		0.4f,0.45f,0.0f,	//Arriba derch
		0.4f, 0.25f,0.0f


	};


	//we assign a colour to each corner (each colour is RGB)

	const GLfloat colors[] = {

		1.0f, 1.0f, 1.0f,
		1.f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,


		1.0f,0.0f,0.0f, //Abajo izq
		1.0f,0.0f,0.0f, //Arriba medio
		1.0f,0.0f,0.0f, //Abajo derech


		0.54f,0.27f,0.07f,
		0.54f,0.27f,0.07f,
		0.54f,0.27f,0.07f,
		0.54f,0.27f,0.07f,


		0.0f, 1.0f, 0.49f,
		0.0f, 1.0f, 0.49f,
		0.0f, 1.0f, 0.49f,
		0.0f, 1.0f, 0.49f,


		1.0f,1.0f,0.2f,
		1.0f,1.0f,0.2f,
		1.0f,1.0f,0.2f,


		0.5f,0.8f,0.9f,
		0.5f,0.8f,0.9f,
		0.5f,0.8f,0.9f,
		0.5f,0.8f,0.9f


	};


	// The index buffer references the vertices we paint, in order

	//here we have two triangles

	const GLuint indices[] = {//Imprimos los vertices

		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		7, 8, 9,
		7,9,10,
		11,12,13,
		12,13,14,
		15,16,17,
		18,19,20,
		18,20,21

	};


	//**********************
	// CODE TO LOAD EVERYTHING INTO MEMORY
	//**********************

	//load the shader
	Shader simpleShader("src/shader.vert", "src/shader.frag");
	g_simpleShader = simpleShader.program;
	

	// Create the VAO where we store all geometry (stored in g_Vao)
	g_Vao = gl_createAndBindVAO();
	//create vertex buffer for positions, colors, and indices, and bind them to shader
	gl_createAndBindAttribute(&(shapes[0].mesh.positions[0]), shapes[0].mesh.positions.size() * sizeof(float), g_simpleShader, "a_vertex", 3);
	//gl_createAndBindAttribute(colors, sizeof(colors), g_simpleShader, "a_color", 3);
	gl_createIndexBuffer(&(shapes[0].mesh.indices[0]), shapes[0].mesh.indices.size() * sizeof(unsigned int));

	//load the shaderuniverse
	Shader simpleShaderUniverse("src/shader.vert", "src/shader_universe.frag");
	g_simpleShaderUniverse = simpleShaderUniverse.program;

	GLfloat* a_normal = &(shapes[0].mesh.normals[0]);
	gl_createAndBindAttribute(&(shapes[0].mesh.normals[0]), shapes[0].mesh.normals.size() * sizeof(float), g_simpleShader, "a_normal", 3);
	GLfloat* uvs = &(shapes[0].mesh.texcoords[0]);
	GLuint uvs_size = shapes[0].mesh.texcoords.size() * sizeof(GLfloat);

	gl_createAndBindAttribute(uvs, uvs_size, g_simpleShader, "a_uv", 2);

	//unbind everything
	gl_unbindVAO();

	//store number of triangles (use in draw())
	g_NumTriangles = shapes[0].mesh.indices.size() / 3;

	//load textures planets

	//******************
	//MILKYWAY
	//******************

	Image* milkyway = loadBMP("assets/milkyway.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_universe);
	glBindTexture(GL_TEXTURE_2D, texture_id_universe);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		milkyway->width,	//width
		milkyway->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		milkyway->pixels);	//pointer to start of data

	//******************
	//SUN
	//******************

	Image* sun = loadBMP("assets/sunmap.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_sun);
	glBindTexture(GL_TEXTURE_2D, texture_id_sun);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		sun->width,	//width
		sun->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		sun->pixels);	//pointer to start of data

	//******************
	//MERCURY
	//******************

	Image* mercury_planet = loadBMP("assets/mercurymap.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_mercury_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_mercury_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		mercury_planet->width,	//width
		mercury_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		mercury_planet->pixels);	//pointer to start of data

	//******************
	//venus_planet
	//******************

	Image* venus_planet = loadBMP("assets/venusmap.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_venus_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_venus_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		venus_planet->width,	//width
		venus_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		venus_planet->pixels);	//pointer to start of data

	//******************
	//EARTH
	//******************

	Image* earth_planet = loadBMP("assets/earthmap1k.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_earth);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		earth_planet->width,	//width
		earth_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		earth_planet->pixels);	//pointer to start of data

	//******************
	//MARS
	//******************

	Image* mars_planet = loadBMP("assets/mars_1k_color.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_mars_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_mars_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		mars_planet->width,	//width
		mars_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		mars_planet->pixels);	//pointer to start of data

	//******************
	//JUPITER
	//******************

	Image* jupiter_planet = loadBMP("assets/jupitermap.bmp"); //Load the texture

	glGenTextures(1, &texture_id_jupiter_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_jupiter_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		jupiter_planet->width,	//width
		jupiter_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		jupiter_planet->pixels);	//pointer to start of data

	//******************
	//SATURN
	//******************

	Image* saturn_planet = loadBMP("assets/saturnmap.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_saturn_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_saturn_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		saturn_planet->width,	//width
		saturn_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		saturn_planet->pixels);	//pointer to start of data

	//******************
	//URANUS
	//******************

	Image* uranus_planet = loadBMP("assets/uranusmap.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_uranus_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_uranus_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		uranus_planet->width,	//width
		uranus_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		uranus_planet->pixels);	//pointer to start of data

	//******************
	//NEPTUNE
	//******************

	Image* neptune_planet = loadBMP("assets/neptunemap.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_neptune_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_neptune_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		neptune_planet->width,	//width
		neptune_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		neptune_planet->pixels);	//pointer to start of data

	

	//******************
	//PLUTON
	//******************

	Image* pluton_planet = loadBMP("assets/plutomapthumb.bmp"); //Load the "texture"

	glGenTextures(1, &texture_id_pluton_planet);
	glBindTexture(GL_TEXTURE_2D, texture_id_pluton_planet);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, //target
		0,	//level, = 0 base, no mipmap
		GL_RGB,	//the wasy opengl stores the data
		pluton_planet->width,	//width
		pluton_planet->height,	//height
		0,	//border, must be 0
		GL_RGB,	//the format of the original data
		GL_UNSIGNED_BYTE,	//type of data
		pluton_planet->pixels);	//pointer to start of data

}
double time1 = glfwGetTime();
// ------------------------------------------------------------------------------------------
// This function actually draws to screen and called non-stop, in a loop
// ------------------------------------------------------------------------------------------
void draw()
{
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	//projection matrix
	mat4 projection_matrix = perspective(
		45.0f, // Field of view
		1.0f, // Aspect ratio
		0.1f, // near plane (distance from camera)
		50.0f // Far plane (distance from camera)
	);

	//view matrix
	glm::mat4 view_matrix = glm::lookAt(
		eye,
		eye + c_front,
		up
	);


	//***************
	//UNIVERSE
	//***************

	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	// activate shader
	glUseProgram(g_simpleShaderUniverse);

	mat4 model = translate(mat4(1.0f), eye); //translate identity in -0.5x

	GLuint view_loc = glGetUniformLocation(g_simpleShaderUniverse, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	GLuint projection_loc = glGetUniformLocation(g_simpleShaderUniverse, "u_projection"); //create projection matrix and pass to shader
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	GLuint model_loc = glGetUniformLocation(g_simpleShaderUniverse, "u_model"); //find “u_model1” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader
															   
	GLuint colorLoc = glGetUniformLocation(g_simpleShaderUniverse, "u_color");
	glUniform3f(colorLoc, 0.0, 0.0, 1.0);

	//find sampler uniform in shader
	GLuint u_texture = glGetUniformLocation(g_simpleShaderUniverse, "u_texture");

	//bind the sampler to teture unit 0
	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_universe);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//*************
	//SUN
	//*************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShaderUniverse);

	mat4 model_e = translate(mat4(1.0f), vec3(0, 0, 0)); //translate identity in -0.5x
	model = scale(model_e, vec3(1, 1, 1));

	view_loc = glGetUniformLocation(g_simpleShaderUniverse, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShaderUniverse, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	model_loc = glGetUniformLocation(g_simpleShaderUniverse, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader
	
	GLuint colorLoc1 = glGetUniformLocation(g_simpleShaderUniverse, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	GLuint u_light_dir = glGetUniformLocation(g_simpleShaderUniverse, "u_light_dir");
	glUniform3f(u_light_dir, 10, 10, 10);

	GLuint u_light_color = glGetUniformLocation(g_simpleShaderUniverse, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 1.0);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_sun);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);


	//******************
	//mercury_planet
	//******************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_mercury_planet, 0, x_mercury_planet)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.015, 0.015, 0.015));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_mercury_planet, 0, -x_mercury_planet);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_mercury_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//*********************
	//VENUS
	//*********************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_venus_planet, 0, x_venus_planet)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.04, 0.04, 0.04));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_venus_planet, 0, -x_venus_planet);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_venus_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);



	//**************************
	//EARTH NEW
	//**************************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_earth,0, x_earth)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.03, 0.03, 0.03));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_earth, 0, -x_earth);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_earth);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//**************************
	//MART
	//**************************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_mart, 0, x_mart)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.02, 0.02, 0.02));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_mart, 0, -x_mart);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_mars_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//**************************
	//jupiter_planet
	//**************************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_jupiter_planet, 0, x_jupiter_planet)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.5, 0.5, 0.5));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_jupiter_planet, 0, -x_jupiter_planet);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);


	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_jupiter_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//***************************
	//SATURN
	//***************************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);


	model_e = translate(mat4(1.0f), vec3(y_saturn_planet, 0, x_saturn_planet)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.45, 0.45, 0.45));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_saturn_planet, 0, -x_saturn_planet);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_saturn_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//**************************
	//URANUS
	//**************************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_uranus_planet, 0, x_uranus_planet)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.1, 0.1, 0.1));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_uranus_planet, 0, -x_uranus_planet);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_uranus_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//**********************
	//NEPTUN
	//**********************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_neptune_planet, 0, x_neptune_planet)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.1, 0.1, 0.1));

	view_loc = glGetUniformLocation(g_simpleShader, "u_view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)); //upload the data to the shader

	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_neptune_planet, 0, -x_neptune_planet);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_neptune_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);

	//**********************
	//PLUTON
	//**********************

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glUseProgram(g_simpleShader);

	model_e = translate(mat4(1.0f), vec3(y_pluton_planet, 0, x_pluton_planet)); //translate identity in -0.5x
	model = scale(model_e, vec3(0.1, 0.1, 0.1));


	model_loc = glGetUniformLocation(g_simpleShader, "u_model"); //find “u_model” in shader
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model)); //upload the data to the shader

	projection_loc = glGetUniformLocation(g_simpleShader, "u_projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

	colorLoc1 = glGetUniformLocation(g_simpleShader, "u_color");
	glUniform3f(colorLoc1, 0.0, 1.0, 1.0);

	u_light_dir = glGetUniformLocation(g_simpleShader, "u_light_dir");
	glUniform3f(u_light_dir, -y_pluton_planet, 0, -x_pluton_planet);

	u_light_color = glGetUniformLocation(g_simpleShader, "u_light_color");
	glUniform3f(u_light_color, 1.0, 1.0, 0.8);

	glUniform1i(u_texture, 0);

	//activate texture unit 0 and bind our texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_pluton_planet);

	//bind the geometry
	gl_bindVAO(g_Vao);

	// Draw to screen
	glDrawElements(GL_TRIANGLES, g_NumTriangles * 3, GL_UNSIGNED_INT, 0);


	time = glfwGetTime();
	if ((time - time1) > 0.1) {

		//**********************
		//MERCURY
		//**********************

		x_mercury_planet = cos(ang * 3) * (1.1); 
		y_mercury_planet = sin(ang * 3) * (1.1);

		//**********************
		//VENUS
		//**********************

		x_venus_planet = cos(ang* 2) * (1.7); 
		y_venus_planet = sin(ang* 2) * (1.7);

		//**********************
		//EARTH
		//**********************

		x_earth = cos(ang*1.5) * (1.8);
		y_earth = sin(ang*1.5) * (1.8);

		//**********************
		//MART
		//**********************

		x_mart = cos(ang) * (2.1); 
		y_mart = sin(ang) * (2.1);

		//**********************
		//JUPITER
		//**********************

		x_jupiter_planet = cos(ang*0.6) * (5.5); 
		y_jupiter_planet = sin(ang*0.6) * (5.5);

		//**********************
		//SATURN
		//**********************

		x_saturn_planet = cos(ang*0.4) * (6.7); 
		y_saturn_planet = sin(ang*0.4) * (6.7);


		//**********************
		//URANUS
		//**********************

		x_uranus_planet = cos(ang*0.3) * (7.9); 
		y_uranus_planet = sin(ang*0.3) * (7.9);

		//**********************
		//NEPTUNE
		//**********************

		x_neptune_planet = cos(ang*0.2) * (8.1); 
		y_neptune_planet = sin(ang*0.2) * (8.1);


		//**********************
		//PLUTON
		//**********************

		x_pluton_planet = cos(ang*0.1) * (9); 
		y_pluton_planet = sin(ang*0.1) * (9);


		time1 = glfwGetTime();
		ang = ang + 0.02;
	}

}

// -----------------------------------------------------------------------------------------------
// This function is called every time you press a key--Those are the controls for moving around 
// -----------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Camera speed
	float camara = 0.09f;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, 1);

	if (key == GLFW_KEY_R && action & (GLFW_PRESS || GLFW_REPEAT)) load();

	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {

		eye += (camara * eye) + c_front; //Move to the front	with the W key

	}

	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))

		eye -= glm::normalize(glm::cross(c_front, up)) * camara; //Move to the left with the A key
	
	if (key == GLFW_KEY_D && action == GLFW_PRESS || action == GLFW_REPEAT)

		eye += glm::normalize(glm::cross( c_front, up)) * camara; //Move to the right with the D key

	if (key == GLFW_KEY_S && action == GLFW_PRESS || action == GLFW_REPEAT)

		eye -=( camara * eye) + c_front;  //Move back the left with the S key



}


// ------------------------------------------------------------------------------------------
// This function is called every time you click the mouse
// ------------------------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double eix_x, double eix_y) {
	float xoffset = eix_x - final_x;
	float yoffset = final_y - eix_y;
	std::cout << xoffset << std::endl;
	float sensitivity = 0.08f; //Ajust the efect of rotation
	final_x = eix_x;
	final_y = eix_y;

	if (firstMouse)
	{
		final_x = eix_x;
		final_y = eix_y;
		firstMouse = false;
	}
	if (rotation_y < -89.0f)
		rotation_y = -89.0f;
	if (rotation_y > 89.0f)
		rotation_y = 89.0f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	rotation_x += xoffset;
	rotation_y += yoffset;


	//vector to look around.
	glm::vec3 front;
	front.x = cos(glm::radians(rotation_x)) * cos(glm::radians(rotation_y));
	front.y = sin(glm::radians(rotation_y));
	front.z = sin(glm::radians(rotation_x)) * cos(glm::radians(rotation_y));
	c_front = glm::normalize(front);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		cout << "Left mouse down at" << eix_x << ", " << eix_y << endl;
	}
}

int main(void)
{
	//setup window and boring stuff, defined in glfunctions.cpp
	GLFWwindow* window;
	if (!glfwInit())return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(g_ViewportWidth, g_ViewportHeight, "Hello OpenGL!", NULL, NULL);
	if (!window) { glfwTerminate();	return -1; }
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	//input callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	
	glfwSetCursorPosCallback(window, mouse_callback); //we add the mouse_callback procedure in order to move the camera with the mouse
	//load all the resources
	load();

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{

		//The velocity of the camera will be balanced out.
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		GLuint u_texture = glGetUniformLocation(g_simpleShader, "u_texture");

		draw();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

		//mouse position must be tracked constantly (callbacks do not give accurate delta)
		glfwGetCursorPos(window, &eix_x, &eix_y);
	}

	//terminate glfw and exit
	glfwTerminate();
	return 0;
}
