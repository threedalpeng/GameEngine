#include "cg/cgmath.h" // slee's simple math library
#include "cg/cgut.h"   // slee's OpenGL utility

//*************************************
// global constants
static const char* window_name = "cgbase - transform - Stanford dragon colored by normals";
static const char* vert_shader_path = "shaders/transform.vert";
static const char* frag_shader_path = "shaders/transform.frag";
static const char* mesh_vertex_path = "mesh/dragon.vertex.bin";
static const char* mesh_index_path = "mesh/dragon.index.bin";
static const uint MIN_INSTANCE = 1; // minimum instances
static const uint MAX_INSTANCE = 3; // maximum instances
uint NUM_INSTANCE = 1;				// initial instances

//*************************************
// common structures
struct camera
{
	vec3 eye = vec3(0, 30, 300);
	vec3 at = vec3(0, 0, 0);
	vec3 up = vec3(0, 1, 0);
	mat4 view_matrix = mat4::look_at(eye, at, up);

	float fovy = PI / 4.0f; // must be in radian
	float aspect_ratio;
	float dNear = 1.0f;
	float dFar = 1000.0f;
	mat4 projection_matrix;
};

//*************************************
// window objects
GLFWwindow* window = nullptr;
ivec2 window_size = cg_default_window_size(); // initial window size

//*************************************
// OpenGL objects
GLuint program = 0; // ID holder for GPU program

//*************************************
// global variables
int frame = 0; // index of rendering frames

//*************************************
// scene objects
mesh* pMesh = nullptr;
camera cam;

//*************************************
void update()
{
	// update projection matrix
	cam.aspect_ratio = window_size.x / float(window_size.y);
	cam.projection_matrix = mat4::perspective(cam.fovy, cam.aspect_ratio, cam.dNear, cam.dFar);

	// update uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation(program, "view_matrix");
	if (uloc > -1)
		glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix); // update the view matrix (covered later in viewing lecture)
	uloc = glGetUniformLocation(program, "projection_matrix");
	if (uloc > -1)
		glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix); // update the projection matrix (covered later in viewing lecture)
}

void render()
{
	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// notify GL that we use our own program
	glUseProgram(program);

	// bind vertex array object
	if (pMesh && pMesh->vertex_array)
		glBindVertexArray(pMesh->vertex_array);

	// render vertices: trigger shader programs to process vertex data
	for (int k = 0, kn = int(NUM_INSTANCE); k < kn; k++)
	{
		// configure transformation parameters
		float t = float(glfwGetTime());
		float theta = t * ((k % 2) - 0.5f) * float(k + 1) * 0.5f;
		float move = ((k % 2) - 0.5f) * 200.0f * float((k + 1) / 2);

		// build the model matrix
		mat4 model_matrix = mat4::translate(move, 0.0f, -abs(move)) *
			mat4::translate(cam.at) *
			mat4::rotate(vec3(0, 1, 0), theta) *
			mat4::translate(-cam.at);

		// update the uniform model matrix and render
		glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_TRUE, model_matrix);
		glDrawElements(GL_TRIANGLES, pMesh->index_list.size(), GL_UNSIGNED_INT, nullptr);
	}

	// swap front and back buffers, and display to screen
	glfwSwapBuffers(window);
}

void reshape(GLFWwindow* window, int width, int height)
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering
	window_size = ivec2(width, height);
	glViewport(0, 0, width, height);
}

void print_help()
{
	printf("[help]\n");
	printf("- press ESC or 'q' to terminate the program\n");
	printf("- press F1 or 'h' to see help\n");
	printf("- press '+/-' to increase/decrease the number of instances (min=%d, max=%d)\n", MIN_INSTANCE, MAX_INSTANCE);
	printf("\n");
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
			glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key == GLFW_KEY_H || key == GLFW_KEY_F1)
			print_help();
		else if (key == GLFW_KEY_KP_ADD || (key == GLFW_KEY_EQUAL && (mods & GLFW_MOD_SHIFT)) /* + */)
		{
			if (NUM_INSTANCE >= MAX_INSTANCE)
				return;
			printf("> NUM_INSTANCE = % -4d\r", ++NUM_INSTANCE);
		}
		else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS)
		{
			if (NUM_INSTANCE <= MIN_INSTANCE)
				return;
			printf("> NUM_INSTANCE = % -4d\r", --NUM_INSTANCE);
		}
	}
}

void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		dvec2 pos;
		glfwGetCursorPos(window, &pos.x, &pos.y);
		printf("> Left mouse button pressed at (%d, %d)\n", int(pos.x), int(pos.y));
	}
}

void motion(GLFWwindow* window, double x, double y)
{
}

bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f); // set clear color
	glEnable(GL_CULL_FACE);									   // turn on backface culling
	glEnable(GL_DEPTH_TEST);								   // turn on depth tests

	// load the mesh
	pMesh = cg_load_mesh(mesh_vertex_path, mesh_index_path);
	if (pMesh == nullptr)
	{
		printf("Unable to load mesh\n");
		return false;
	}

	return true;
}

void user_finalize()
{
}

int main(int argc, char* argv[])
{
	// create window and initialize OpenGL extensions
	if (!(window = cg_create_window(window_name, window_size.x, window_size.y)))
	{
		glfwTerminate();
		return 1;
	}
	if (!cg_init_extensions(window))
	{
		glfwTerminate();
		return 1;
	} // version and extensions

	// initializations and validations
	if (!(program = cg_create_program(vert_shader_path, frag_shader_path)))
	{
		glfwTerminate();
		return 1;
	} // create and compile shaders/program
	if (!user_init())
	{
		printf("Failed to user_init()\n");
		glfwTerminate();
		return 1;
	} // user initialization

	// register event callbacks
	glfwSetWindowSizeCallback(window, reshape); // callback for window resizing events
	glfwSetKeyCallback(window, keyboard);		// callback for keyboard events
	glfwSetMouseButtonCallback(window, mouse);	// callback for mouse click inputs
	glfwSetCursorPosCallback(window, motion);	// callback for mouse movement

	// enters rendering/event loop
	for (frame = 0; !glfwWindowShouldClose(window); frame++)
	{
		glfwPollEvents(); // polling and processing of events
		update();		  // per-frame update
		render();		  // per-frame render
	}

	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}