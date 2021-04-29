#include <random>

#include "engine/application.h"
#include "circle.h"

class MovingCircleApplication : public Application {
public:
	MovingCircleApplication(const char* title, ivec2 window_size);

private:
	// OpenGL objects
	GLuint	program = 0;

	const char* vert_shader_path = "shaders/circ.vert";
	const char* frag_shader_path = "shaders/circ.frag";

	struct {
		// circ.frag
		GLint b_solid_color = -1;
		GLint solid_color = -1;
		// circ.vert
		GLint model_matrix = -1;
		GLint aspect_matrix = -1;
	} uloc;

	const uint MIN_CIRCLES = 3;
	const uint MAX_CIRCLES = 256;
	uint NUM_CIRCLES = 100;

	const float MIN_RADIUS = 0.1f;
	const float MAX_RADIUS = 1.f;

	float window_aspect = 1.f;
	float VERTICAL_WALL = 1.f;
	float HORIZONTAL_WALL = 1.f;

	float t = 0.0f;

	bool b_solid_color = false;
#ifndef GL_ES_VERSION_2_0
	bool	b_wireframe = false;
#endif
	struct {
		bool add = false, sub = false;
		operator bool() const { return add || sub; }
	} b;

	void init() final;
	void update() final;
	void render() final;

	std::vector<Circle> circles;

	void print_help();

	void create_circles();
	void update_num_circles();

	void reshape(GLFWwindow* window, int width, int height) final;
	void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) final;
	void mouse(GLFWwindow* window, int button, int action, int mods) final;
};

inline bool circle_collided(Circle c1, Circle c2);
inline bool circle_collided(Circle c, vec2 center, float radius);
inline bool circle_collided(vec2 center1, float radius1, vec2 center2, float radius2);
inline vec2 get_next_velocity(float m1, float v1, float m2, float v2);

MovingCircleApplication::MovingCircleApplication(const char* title, ivec2 window_size)
	: Application(title, window_size) {}

void MovingCircleApplication::init() {
	Application::init();

	if (!(program = cg_create_program(vert_shader_path, frag_shader_path))) {
		glfwTerminate(); exit(1);
	}

	window_aspect = window_size.x / float(window_size.y);
	VERTICAL_WALL = 1.f;
	HORIZONTAL_WALL = 1.f * window_aspect;

	// log hotkeys
	print_help();

	// init GL states
	glLineWidth(1.0f);
	glClearColor(39 / 255.0f, 40 / 255.0f, 34 / 255.0f, 1.0f);	// set clear color
	glEnable(GL_CULL_FACE);										// turn on backface culling
	glEnable(GL_DEPTH_TEST);									// turn on depth tests

	Circle::update_vertex_buffer();

	create_circles();

	uloc.b_solid_color = glGetUniformLocation(program, "b_solid_color");
	uloc.solid_color = glGetUniformLocation(program, "solid_color");
	uloc.model_matrix = glGetUniformLocation(program, "model_matrix");
	uloc.aspect_matrix = glGetUniformLocation(program, "aspect_matrix");
}

void MovingCircleApplication::update()
{
	static float prev_t = 0.f;

	// update global simulation parameter
	t = float(glfwGetTime()) * 0.4f;
	float duration = t - prev_t;
	prev_t = t;

	// tricky aspect correction matrix for non-square window
	mat4 aspect_matrix =
	{
		std::min(1 / window_aspect, 1.0f), 0, 0, 0,
		0, std::min(window_aspect, 1.0f), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	// update common uniform variables in vertex/fragment shaders
	if (uloc.b_solid_color > -1) glUniform1i(uloc.b_solid_color, b_solid_color);
	if (uloc.aspect_matrix > -1) glUniformMatrix4fv(uloc.aspect_matrix, 1, GL_TRUE, aspect_matrix);

	// update vertex buffer by the pressed keys
	if (b) update_num_circles();

	for (uint i = 0; i < NUM_CIRCLES; i++) {
		Circle& sc = circles[i];
		vec2 scc = sc.center + sc.velocity * duration, tcc;

		for (uint j = 0; j < NUM_CIRCLES; j++) {
			if (i == j) continue;
			Circle& tc = circles[j];
			tcc = tc.center + tc.velocity * duration;
			if (circle_collided(scc, sc.radius, tcc, tc.radius)) {
				vec2 new_vx = get_next_velocity(sc.mass, sc.velocity.x, tc.mass, tc.velocity.x);
				vec2 new_vy = get_next_velocity(sc.mass, sc.velocity.y, tc.mass, tc.velocity.y);
				sc.velocity = vec2(new_vx.x, new_vy.x);
				tc.velocity = vec2(new_vx.y, new_vy.y);
			}
		}
	}

	for (auto& c : circles)
	{
		if (c.center.x - c.radius < -HORIZONTAL_WALL || +HORIZONTAL_WALL < c.center.x + c.radius) {
			c.velocity.x = -c.velocity.x;
		}
		if (c.center.y - c.radius < -VERTICAL_WALL || +VERTICAL_WALL < c.center.y + c.radius) {
			c.velocity.y = -c.velocity.y;
		}

		// per-circle update
		c.update(duration);
	}
}

void MovingCircleApplication::render()
{
	// notify GL that we use our own program
	glUseProgram(program);

	// bind vertex array object
	glBindVertexArray(Circle::vertex_array);

	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render two circles: trigger shader program to process vertex data
	for (auto& c : circles)
	{
		// update per-circle uniforms
		if (uloc.solid_color > -1) glUniform4fv(uloc.solid_color, 1, c.color);	// pointer version
		if (uloc.model_matrix > -1) glUniformMatrix4fv(uloc.model_matrix, 1, GL_TRUE, c.model_matrix);

		c.render();
	}

	// swap front and back buffers, and display to screen
	glfwSwapBuffers(window);
}

void MovingCircleApplication::print_help()
{
	printf("[help]\n");
	printf("- press ESC or 'q' to terminate the program\n");
	printf("- press F1 or 'h' to see help\n");
	printf("- press 'd' to toggle between solid color and texture coordinates\n");
	printf("- press '+/-' to increase/decrease the number of circles (min=%d, max=%d)\n", MIN_CIRCLES, MAX_CIRCLES);
	printf("- press 'i' to toggle between index buffering and simple vertex buffering\n");
#ifndef GL_ES_VERSION_2_0
	printf("- press 'w' to toggle wireframe\n");
#endif
	printf("- press 'r' to toggle wireframe\n");
	printf("\n");
}

void MovingCircleApplication::create_circles()
{
	circles.clear();
	float rNumCircle = sqrtf((float)NUM_CIRCLES);
	float rmin = MIN_RADIUS / rNumCircle;
	float rmax = MAX_RADIUS / rNumCircle;
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<float> center_x_dstr(-1.f * window_aspect + rmax, 1.f * window_aspect - rmax);
	std::uniform_real_distribution<float> center_y_dstr(-1.f + rmax, 1.f - rmax);
	std::uniform_real_distribution<float> direction_dstr(-1.f * PI, 1.f * PI);
	std::uniform_real_distribution<float> radius_dstr(rmin, rmax);
	std::uniform_real_distribution<float> color_dstr(0.f, 1.f);

	float r, cr, cg, cb, drt;
	vec2 ct;
	bool is_conflicted;
	for (uint i = 0; i < NUM_CIRCLES; i++)
	{
		do {
			is_conflicted = false;
			ct = vec2(center_x_dstr(eng), center_y_dstr(eng));
			r = radius_dstr(eng);
			cr = color_dstr(eng);
			cg = color_dstr(eng);
			cb = color_dstr(eng);
			drt = direction_dstr(eng);
			for (auto& c : circles) {
				if (circle_collided(c, ct, r)) {
					is_conflicted = true;
					break;
				}
			}
		} while (is_conflicted);
		circles.emplace_back(ct, r, 0.f, vec4(cr, cg, cb, 1.f), vec2(cos(drt), sin(drt)));
	}
}

void MovingCircleApplication::update_num_circles()
{
	uint n = NUM_CIRCLES; if (b.add) n++; if (b.sub) n--;
	if (n == NUM_CIRCLES || n < MIN_CIRCLES || n > MAX_CIRCLES) return;
	NUM_CIRCLES = n;

	// create NUM_CIRCLES circles here
	create_circles();

	printf("> NUM_CIRCLES = % -4d\r", NUM_CIRCLES);
}

void MovingCircleApplication::reshape(GLFWwindow* window, int width, int height)
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering
	window_size = ivec2(width, height);
	glViewport(0, 0, width, height);
}

void MovingCircleApplication::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)	glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key == GLFW_KEY_H || key == GLFW_KEY_F1)	print_help();
		else if (key == GLFW_KEY_KP_ADD || (key == GLFW_KEY_EQUAL && (mods & GLFW_MOD_SHIFT)))	b.add = true;
		else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS) b.sub = true;
		else if (key == GLFW_KEY_I)
		{
			Circle::b_index_buffer = !Circle::b_index_buffer;
			Circle::update_vertex_buffer();
			printf("> using %s buffering\n", Circle::b_index_buffer ? "index" : "vertex");
		}
		else if (key == GLFW_KEY_D)
		{
			b_solid_color = !b_solid_color;
			printf("> using %s\n", b_solid_color ? "solid color" : "texture coordinates as color");
		}
		else if (key == GLFW_KEY_R)
		{
			create_circles();
		}
#ifndef GL_ES_VERSION_2_0
		else if (key == GLFW_KEY_W)
		{
			b_wireframe = !b_wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, b_wireframe ? GL_LINE : GL_FILL);
			printf("> using %s mode\n", b_wireframe ? "wireframe" : "solid");
		}
#endif
	}
	else if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_KP_ADD || (key == GLFW_KEY_EQUAL && (mods & GLFW_MOD_SHIFT)))	b.add = false;
		else if (key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_MINUS) b.sub = false;
	}
}

void MovingCircleApplication::mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		dvec2 pos; glfwGetCursorPos(window, &pos.x, &pos.y);
		printf("> Left mouse button pressed at (%d, %d)\n", int(pos.x), int(pos.y));
	}
}

inline bool circle_collided(Circle c1, Circle c2)
{
	return ((c1.center - c2.center).length2() <= (c1.radius + c2.radius) * (c2.radius + c2.radius));
}

inline bool circle_collided(Circle c, vec2 center, float radius)
{
	return ((c.center - center).length2() <= (c.radius + radius) * (c.radius + radius));
}

inline bool circle_collided(vec2 center1, float radius1, vec2 center2, float radius2)
{
	return ((center1 - center2).length2() <= (radius1 + radius2) * (radius1 + radius2));
}

inline vec2 get_next_velocity(float m1, float v1, float m2, float v2)
{
	return vec2(((m1 - m2) * v1 + 2 * m2 * v2) / (m1 + m2), (2 * m1 * v1 + (m2 - m1) * v2) / (m1 + m2));
}

int main(int argc, char* argv[]) {
	const char* window_name = "moving-circles";
	MovingCircleApplication* app = new MovingCircleApplication(window_name, cg_default_window_size());
	app->run();
}