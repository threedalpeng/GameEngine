//*******************************************************************
// Terms of Use and Disclaimer
// Copyright 2011-2040 Sungkil Lee
//
// This software is owned by Sungkil Lee.
// You may use and modify this software for non-commercial and commercial software,
// but re-distribution of this software is strictly prohibited.
//*******************************************************************

#ifndef __CGUT_H__
#define __CGUT_H__

// minimum standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>

#include "cg/cgmath.h"

// platform-dependent configuration
#if defined(_MSC_VER)
	// enforce not to use /MD or /MDd flag
#if defined(_DLL) && !defined(CGUT_DYNAMIC)
#error Use /MT at Configuration > C/C++ > Code Generation > Run-time Library; Otherwise, define CGUT_DYNAMIC to use /MD
#endif
#include <direct.h>
#include <io.h>
#define access _access // suppress warning for deprecated posix access
#elif defined(__GNUC__)
#include <unistd.h>
#endif

#define GLFW_INCLUDE_NONE
#if defined(__has_include) && __has_include("gl/glfw/glfw3.h")
#include "gl/glfw/glfw3.h"		// http://www.glfw.org // using OpenGL/GLSL Core Profile 4.6
#include "gl/glad/glad.h"		// visit http://glad.dav1d.de/ to generate your own glad.h/glad.c
#elif __has_include("gles/glfw/glfw3.h")
#include "gles/glfw/glfw3.h"	// http://www.glfw.org // using OpenGL/GLSL ES Profile 3.0 or higher
#include "gles/glad/glad.h"		// visit http://glad.dav1d.de/ to generate your own glad.h/glad.c
#endif

// explicitly link libraries
#if defined(_MSC_VER)
#if _MSC_VER>=1920 && !defined(_DLL) // static lib for VC2019 or higher
#if defined _M_IX86
#pragma comment( lib, "glfw3.x86.lib" )
#elif defined _M_X64
#pragma comment( lib, "glfw3.x64.lib" )
#endif
#else // dynamic lib for other VC version
#if defined _M_IX86
#pragma comment( lib, "glfw3dll.x86.lib" )		// static lib for VC2019
#elif defined _M_X64
#pragma comment( lib, "glfw3dll.x64.lib" )		// static lib for VC2019
#endif
#endif
#endif

//*************************************
// OpenGL versions
struct gl_version_t
{
	int major = 0;
	int minor = 0;
	struct { int major = 0, minor = 0; } sl;
	static gl_version_t& instance() { static gl_version_t v; return v; }
	int gl() const { return major * 10 + minor; }
	int glsl() const { return sl.major * 10 + sl.minor; }

#ifdef GL_ES_VERSION_2_0
	enum default_t { major_default = 2, minor_default = 0 };	// minimum for modern GL ES
	constexpr bool is_gles() { return true; }
#else
	enum default_t { major_default = 3, minor_default = 3 };	// minimum for modern GL
	constexpr bool is_gles() { return false; }
#endif
};

//*************************************
// module file path
#ifdef _MSC_VER
struct module_path_t
{
	char path[_MAX_PATH], drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	module_path_t() { GetModuleFileNameA(0, path, _MAX_PATH); _splitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT); }
};
#endif

//*************************************
// common structures
struct mem_t
{
	char* ptr = nullptr;
	size_t	size = 0;
};

struct vertex // will be used for all the course examples
{
	vec3 pos;	// position
	vec3 norm;	// normal vector; we will use this for vertex color for this example
	vec2 tex;	// texture coordinate; ignore this for the moment
};

struct image
{
	unsigned char* ptr = nullptr; // image content
	union { struct { int width, height; }; ivec2 size; };
	int		channels = 0;

	image() : width(0), height(0) {}
	~image() { if (ptr) free(ptr); }
	GLint internal_format() { return channels == 1 ? GL_R8 : channels == 2 ? GL_RG8 : channels == 3 ? GL_RGB8 : GL_RGBA8; }
	GLenum format() { return channels == 1 ? GL_RED : channels == 2 ? GL_RG : channels == 3 ? GL_RGB : GL_RGBA; }
};

struct mesh
{
	std::vector<vertex>	vertex_list;
	std::vector<uint>	index_list;
	GLuint	vertex_buffer = 0;
	GLuint	index_buffer = 0;
	GLuint	vertex_array = 0;
	GLuint	texture = 0;

	~mesh()
	{
		if (vertex_buffer) glDeleteBuffers(1, &vertex_buffer);
		if (index_buffer) glDeleteBuffers(1, &index_buffer);
		if (vertex_array) glDeleteVertexArrays(1, &vertex_array);
	}
};

//*************************************
// utility functions
inline mem_t cg_read_binary(const char* file_path)
{
	mem_t m;
	FILE* fp = fopen(file_path, "rb"); if (fp == nullptr) { printf("[error] Unable to open %s\n", file_path); return mem_t(); }
	fseek(fp, 0L, SEEK_END);
	m.size = ftell(fp);
	m.ptr = (char*)malloc(m.size + 1);		// +1 for string
	fseek(fp, 0L, SEEK_SET);				// reset file pointer
	fread(m.ptr, m.size, 1, fp);
	memset(m.ptr + m.size, 0, sizeof(char));	// for string
	fclose(fp);
	return m;
}

inline char* cg_read_shader(const char* file_path)
{
#ifdef _MSC_VER
	if (strlen(file_path) < 2) { printf("%s(): file_path (%s) is too short\n", __FUNCTION__, file_path); return nullptr; }

	// get the full path of shader file
	module_path_t mpath;
	char shader_file_path[_MAX_PATH]; sprintf_s(shader_file_path, "%s%s%s", mpath.drive, mpath.dir, file_path);

	// get the full path of a shader file
	if (_access(shader_file_path, 0) != 0) { printf("%s(): %s not exists\n", __FUNCTION__, shader_file_path); return nullptr; }
	return cg_read_binary(shader_file_path).ptr;
#else
	return cg_read_binary(file_path).ptr;
#endif
}

inline bool cg_validate_shader(GLuint shaderID, const char* shaderName)
{
	const int MAX_LOG_LENGTH = 4096;
	static char msg[MAX_LOG_LENGTH] = { 0 };
	GLint shaderInfoLogLength;

	glGetShaderInfoLog(shaderID, MAX_LOG_LENGTH, &shaderInfoLogLength, msg);
	if (shaderInfoLogLength > 1 && shaderInfoLogLength <= MAX_LOG_LENGTH)
		printf("[Shader: %s]\n%s\n", shaderName, msg);

	GLint shaderCompileStatus; glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompileStatus);
	if (shaderCompileStatus == GL_TRUE) return true;

	glDeleteShader(shaderID);
	return false;
}

inline bool cg_validate_program(GLuint programID, const char* programName)
{
	const int MAX_LOG_LENGTH = 4096;
	static char msg[MAX_LOG_LENGTH] = { 0 };
	GLint programInfoLogLength;

	glGetProgramInfoLog(programID, MAX_LOG_LENGTH, &programInfoLogLength, msg);
	if (programInfoLogLength > 1 && programInfoLogLength <= MAX_LOG_LENGTH)
		printf("[Program: %s]\n%s\n", programName, msg);

	GLint programLinkStatus; glGetProgramiv(programID, GL_LINK_STATUS, &programLinkStatus);
	if (programLinkStatus != GL_TRUE) { glDeleteProgram(programID); return false; }

	glValidateProgram(programID);
	glGetProgramInfoLog(programID, MAX_LOG_LENGTH, &programInfoLogLength, msg);
	if (programInfoLogLength > 1 && programInfoLogLength <= MAX_LOG_LENGTH)
		printf("[Program: %s]\n%s\n", programName, msg);

	GLint programValidateStatus; glGetProgramiv(programID, GL_VALIDATE_STATUS, &programValidateStatus);
	if (programValidateStatus != GL_TRUE) { glDeleteProgram(programID); return false; }

	return true;
}

#ifdef _MSC_VER
inline const char* cg_conf_path()
{
	static module_path_t mpath;
	static char conf_dir[_MAX_PATH] = { 0 }, conf_path[_MAX_PATH] = { 0 };
	if (!conf_path[0])
	{
		char temp[_MAX_PATH]; GetTempPathA(_MAX_PATH - 1, temp);
		if (temp[0]) { size_t s = strlen(temp); if (temp[s - 1] != '\\') { temp[s] = '\\'; temp[s + 1] = '\0'; } }
		if (!conf_dir[0]) sprintf(conf_dir, "%s.cgbase\\", temp);
		if (_access(conf_dir, 0) != 0) _mkdir(conf_dir);
		sprintf(conf_path, "%s%s%s.conf", conf_dir, mpath.fname, mpath.ext);
	}
	return conf_path;
}
#endif

inline bool cg_glfw_init_once()
{
	static bool b = false; if (b) return true;
	return b = (glfwInit() != GLFW_FALSE);
}

inline void cg_glfw_error(int error_code, const char* desc)
{
	printf("[glfw] error(%d): %s\n", error_code, desc);
}

// high dpi-aware window scaling
inline float cg_get_dpi_scale()
{
	cg_glfw_init_once();

	static float scale = 0;
	if (scale > 0) return scale;

	GLFWmonitor* mon = glfwGetPrimaryMonitor(); if (!mon) return 1.0f;
#if defined(_MSC_VER) && GLFW_VERSION_MAJOR>=3 && GLFW_VERSION_MINOR>=3 // Ubuntu GLFW does not support 3.3 yet
	float xscale, yscale; glfwGetMonitorContentScale(mon, &xscale, &yscale);
	scale = xscale > yscale ? xscale : yscale;
	if (scale < 1.01f || scale>16.0f) scale = 1.0f;
#else
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor()); if (!mode) return 1.0f;
	int mon_width = mode->width > mode->height ? mode->width : mode->height; // to detect monitor tilting
	scale = (mon_width >= 2560 && mon_width <= 16384) ? mon_width / 1638.4f : 1.0f;
	if (scale > 2.5f) scale = 2.5f; // prevent excessive scaling in WSL
#endif

	return scale;
}

inline ivec2 cg_scale_by_dpi(int width, int height)
{
	float dpi_scale = cg_get_dpi_scale();
	return ivec2(int(width * dpi_scale), int(height * dpi_scale));
}

inline ivec2 cg_scale_by_dpi(ivec2 size)
{
	return cg_scale_by_dpi(size.x, size.y);
}

inline ivec2 cg_default_window_size(bool b_scale_by_dpi = true)
{
#ifdef GL_ES_VERSION_2_0
	ivec2 size = ivec2(576, 1024);	// initial window size similarly to smartphone
#else
	ivec2 size = ivec2(720, 480);	// initial window size
#endif
	return b_scale_by_dpi ? cg_scale_by_dpi(size) : size;
}

inline GLFWwindow* cg_create_window(const char* name, int& width, int& height, int version_major = gl_version_t::major_default, int version_minor = gl_version_t::minor_default, bool show_window = true)
{
	// register error callback for GLFW
	glfwSetErrorCallback(cg_glfw_error);

	// initialization
	if (!cg_glfw_init_once()) { printf("[error] %s(): failed in glfwInit()\n", __func__); return nullptr; }

	// give GLFW hints for window and OpenGL context
	gl_version_t& v = gl_version_t::instance();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);	// minimum requirement
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);	// minimum requirement
	if (v.is_gles())
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	}
	else
	{
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);				// core profile (>=3.3) allow only forward-compatible profile
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// create core profile; all legacy deprecated
	}

	// create a windowed mode window and its OpenGL context
	GLFWwindow* win = glfwCreateWindow(width, height, name, nullptr, nullptr); if (!win) { printf("Failed to create a GLFW window.\n"); glfwTerminate(); return nullptr; }

	// get the screen size and locate the window in the center
	ivec2 win_pos(-1, -1);
#ifdef _MSC_VER
	FILE* fp = fopen(cg_conf_path(), "r");
	if (fp) { fscanf(fp, "window_pos_x = %d\nwindow_pos_y = %d\n", &win_pos.x, &win_pos.y); fclose(fp); }
#endif

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	ivec2 screen_size = ivec2(mode->width, mode->height);
	if (win_pos.x < 0 || win_pos.x >= screen_size.x || win_pos.y < 0 || win_pos.y >= screen_size.y) win_pos = ivec2((screen_size.x - width) / 2, (screen_size.y - height) / 2);
	glfwSetWindowPos(win, win_pos.x, win_pos.y);
	glfwSetWindowSize(win, width, height); // make sure to update the size again to avoid DPI mismatch problems

	printf("Using window resolution %dx%d at (%d,%d)\n", width, height, win_pos.x, win_pos.y);

	// make context and show window
	glfwMakeContextCurrent(win);
	if (show_window) glfwShowWindow(win);

	return win;
}

inline void cg_destroy_window(GLFWwindow* window)
{
	// save window position
#ifdef _MSC_VER
	if (glfwGetWindowAttrib(window, GLFW_VISIBLE))
	{
		int x, y; glfwGetWindowPos(window, &x, &y);
		FILE* fp = fopen(cg_conf_path(), "w"); if (fp) { fprintf(fp, "window_pos_x = %d\nwindow_pos_y = %d\n", x, y); fclose(fp); }
	}
#endif

	glfwDestroyWindow(window);
	glfwTerminate();
}

inline bool cg_init_extensions(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);	// make sure the current context again

#ifdef GL_ES_VERSION_2_0
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) { printf("init_extensions(): Failed in gladLoadGLES2Loader()\n"); glfwTerminate(); return false; }
#else
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { printf("init_extensions(): Failed in gladLoadGLLoader()\n"); glfwTerminate(); return false; }
#endif

	// print GL/GLSL versions
	gl_version_t& v = gl_version_t::instance();
	printf("Using %s on %s, %s\n", glGetString(GL_VERSION), glGetString(GL_RENDERER), glGetString(GL_VENDOR));
	glGetIntegerv(GL_MAJOR_VERSION, &v.major);
	glGetIntegerv(GL_MINOR_VERSION, &v.minor); while (v.minor > 10) v.minor /= 10;
	const char* strGLSLver = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (v.gl() < 20) printf("Warning: OpenGL version may be too low to run modern-style OpenGL\n");

	// warning for lower version
	printf("Using %s%s\n", v.is_gles() ? "" : "OpenGL GLSL ", strGLSLver);
#ifdef GL_ES_VERSION_2_0
	if (strGLSLver) { sscanf(strGLSLver, "OpenGL ES GLSL ES %d.%d", &v.sl.major, &v.sl.minor); while (v.sl.minor >= 10) v.sl.minor /= 10; }
#else
	if (strGLSLver) { sscanf(strGLSLver, "%d.%d", &v.sl.major, &v.sl.minor); while (v.sl.minor >= 10) v.sl.minor /= 10; }
#endif

	// load and check extensions
	std::set<std::string> ext_set;
#ifdef GL_ES_VERSION_2_0
	size_t ext_len = strlen((const char*)glGetString(GL_EXTENSIONS));
	std::vector<char> ext((char*)glGetString(GL_EXTENSIONS), (char*)glGetString(GL_EXTENSIONS) + ext_len + 2);
	for (char* t = strtok(&ext[0], " \t\n"); t; t = strtok(nullptr, " \t\n")) ext_set.insert(t);
#else
	int next; glGetIntegerv(GL_NUM_EXTENSIONS, &next);
	for (int k = 0; k < next; k++) ext_set.insert((const char*)glGetStringi(GL_EXTENSIONS, k));
#define CHECK_GL_EXT(ext) if(ext_set.find( "GL_ARB_"#ext )==ext_set.end() ) printf( "Warning: GL_ARB_" #ext " not supported.\n" );
	CHECK_GL_EXT(vertex_buffer_object);	// BindBuffers, DeleteBuffers, GenBuffers, IsBuffer, BufferData, BufferSubData, GenBufferSubData, ...
	CHECK_GL_EXT(vertex_array_object);	// BindBuffers, DeleteBuffers, GenBuffers, IsBuffer, BufferData, BufferSubData, GenBufferSubData, ...
	CHECK_GL_EXT(vertex_shader);			// functions related to vertex shaders
	CHECK_GL_EXT(fragment_shader);		// functions related to fragment shaders
	CHECK_GL_EXT(shader_objects);			// functions related to program and shaders
#undef CHECK_GL_EXT
#endif

	return true;
}

inline const char* shader_type_name(GLenum shader_type)
{
	if (shader_type == 0x8B31) return "vertex shader";
	if (shader_type == 0x8B30) return "fragment shader";
	if (shader_type == 0x8DD9) return "geometry shader";
	if (shader_type == 0x8E88) return "tess control shader";
	if (shader_type == 0x8E87) return "tess evaluation shader";
	if (shader_type == 0x91B9) return "compute shader";
	return "unknown shader_type";
}

inline bool strstr(const char* src, std::initializer_list<const char*> substr)
{
	for (auto* s : substr) if (strstr(src, s)) return true;
	return false;
}

inline GLuint cg_create_shader(const char* shader_source, GLenum shader_type, std::string& log)
{
	if (!shader_source) { printf("%s(): shader_source == nullptr\n", __FUNCTION__); return 0; }

	std::string stn = std::string("[") + shader_type_name(shader_type) + "]";
	std::string src = shader_source;
	std::string macro;

	if (!strstr(shader_source, "#version"))
	{
		gl_version_t& v = gl_version_t::instance();
		char sver[1024]; sprintf(sver, "#version %d%s", v.glsl() * 10, v.is_gles() ? " es" : ""); macro += std::string(sver) + "\n";
		printf("%-18s '%s' added automatically.\n", stn.c_str(), sver);
	}

	std::vector<const char*> src_list;
	std::vector<GLint> src_size_list;
	if (!macro.empty()) { src_list.push_back(macro.c_str()); src_size_list.push_back(macro.size()); }
	src_list.push_back(shader_source); src_size_list.push_back(strlen(shader_source));

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, src_list.size(), &src_list[0], &src_size_list[0]);
	glCompileShader(shader);
	if (!cg_validate_shader(shader, shader_type_name(shader_type))) { printf("Unable to compile %s\n", shader_type_name(shader_type)); return 0; }

	return shader;
}

inline GLuint cg_create_program_from_string(const char* vertex_shader_source, const char* fragment_shader_source)
{
	// try to create a program
	GLuint program = glCreateProgram();
	glUseProgram(program);

	// create shaders
	std::string log;
	GLuint vertex_shader = cg_create_shader(vertex_shader_source, GL_VERTEX_SHADER, log);
	GLuint fragment_shader = cg_create_shader(fragment_shader_source, GL_FRAGMENT_SHADER, log);
	if (!log.empty()) printf("%s\n", log.c_str());
	if (!vertex_shader || !fragment_shader) return 0;

	// attach vertex/fragments shaders
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	// try to link program
	glLinkProgram(program);
	if (!cg_validate_program(program, "program")) { printf("Unable to link program\n"); return 0; }

	return program;
}

inline GLuint cg_create_program(const char* vert_path, const char* frag_path)
{
	const char* vertex_shader_source = cg_read_shader(vert_path); if (vertex_shader_source == NULL) return 0;
	const char* fragment_shader_source = cg_read_shader(frag_path); if (fragment_shader_source == NULL) return 0;

	// try to create a program
	GLuint program = cg_create_program_from_string(vertex_shader_source, fragment_shader_source);

	// deallocate string
	free((void*)vertex_shader_source);
	free((void*)fragment_shader_source);
	return program;
}

inline uint cg_create_vertex_array(uint vertex_buffer, uint index_buffer = 0)
{
	if (!vertex_buffer) { printf("%s(): vertex_buffer == 0\n", __func__); return 0; }

	// create and bind a vertex array object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// bind vertex/index buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	if (index_buffer) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

	// bind vertex attributes by interpreting the organization of struct vertex
	static size_t attrib_size[] = { sizeof(vertex().pos), sizeof(vertex().norm), sizeof(vertex().tex) };
	for (size_t k = 0, byte_offset = 0; k < 3; k++, byte_offset += attrib_size[k - 1])
	{
		// We need to indicate the explicit binding of vertex attributes in vertex shader:
		// e.g., layout(location=0) in vec3 position;
		glEnableVertexAttribArray(GLuint(k));
		glVertexAttribPointer(GLuint(k), attrib_size[k] / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)byte_offset);
	}

	// unbind vao and return
	glBindVertexArray(0);
	return vao;
}

inline mesh* cg_load_mesh(const char* vert_binary_path, const char* index_binary_path)
{
	mesh* new_mesh = new mesh();

	// load vertex buffer
	mem_t v = cg_read_binary(vert_binary_path);
	if (v.size % sizeof(vertex)) { printf("%s is not a valid vertex binary file\n", vert_binary_path); return nullptr; }
	new_mesh->vertex_list.resize(v.size / sizeof(vertex));
	memcpy((void*)&new_mesh->vertex_list[0], v.ptr, v.size);

	// load index buffer
	mem_t i = cg_read_binary(index_binary_path);
	if (i.size % sizeof(uint)) { printf("%s is not a valid index binary file\n", index_binary_path); return nullptr; }
	new_mesh->index_list.resize(v.size / sizeof(uint));
	memcpy((void*)&new_mesh->index_list[0], i.ptr, i.size);

	// release memory
	if (v.ptr) free(v.ptr);
	if (i.ptr) free(i.ptr);

	// create a vertex buffer
	glGenBuffers(1, &new_mesh->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * new_mesh->vertex_list.size(), &new_mesh->vertex_list[0], GL_STATIC_DRAW);

	// create a index buffer
	glGenBuffers(1, &new_mesh->index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->index_list.size(), &new_mesh->index_list[0], GL_STATIC_DRAW);

	// generate vertex array object, which is mandatory for OpenGL 3.3 and higher
	new_mesh->vertex_array = cg_create_vertex_array(new_mesh->vertex_buffer, new_mesh->index_buffer);
	if (!new_mesh->vertex_array) { printf("%s(): failed to create vertex aray\n", __func__); return nullptr; }

	return new_mesh;
}

#if defined(__has_include) && __has_include( "stb_image.h" )
#include "stb_image.h"
#endif

// assume stb_image.h included somewhere with STB_IMAGE_IMPLEMENTATION
extern "C" unsigned char* stbi_load(const char*, int*, int*, int*, int);

inline image* cg_load_image(const char* image_path)
{
	if (access(image_path, 0) != 0) { printf("%s(): %s not exists\n", __func__, image_path); return nullptr; }
	if (access(image_path, 4) != 0) { printf("%s(): %s does not have a read access\n", __func__, image_path); return nullptr; }

	int w, h, c; unsigned char* i0 = stbi_load(image_path, &w, &h, &c, 0); // load as is for channels
	if (i0 && c == 1) { free(i0); i0 = stbi_load(image_path, &w, &h, &c, 3); c = 3; } // load again gray-scale image as RGB
	if (!i0 || !w || !h || !c) { printf("%s(): unable to load %s\n", __func__, image_path); return nullptr; }

	image* i = new image;
	i->width = w;
	i->height = h;
	i->channels = c;

	int stride0 = w * c, stride1 = (stride0 + 3) & (~3); // alignment at 4-byte boundary
	i->ptr = (unsigned char*)malloc(sizeof(unsigned char) * stride1 * h);
	for (int y = 0; y < h; y++) memcpy(i->ptr + (h - 1 - y) * stride1, i0 + y * stride0, stride0); // copy row with vertical flip
	free(i0); // release the stbi-loaded image; stbi_image_free==free() in regular cases

	return i;
}

inline GLuint cg_create_texture(image* i, bool mipmap, GLenum wrap = GL_CLAMP_TO_EDGE, GLenum filter = GL_LINEAR)
{
	if (!i) return 0; // return null texture; 0 is reserved as a null texture
	int w = i->width, h = i->height, c = i->channels;

	// induce internal format and format from image
	GLint	internal_format = c == 1 ? GL_R8 : c == 2 ? GL_RG8 : c == 3 ? GL_RGB8 : GL_RGBA8;
	GLenum	format = c == 1 ? GL_RED : c == 2 ? GL_RG : c == 3 ? GL_RGB : GL_RGBA;

	// create a src texture (lena texture)
	GLuint texture;
	glGenTextures(1, &texture); if (texture == 0) { printf("%s(): failed in glGenTextures()\n", __func__); return 0; }
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, i->ptr);
	if (i) { delete i; i = nullptr; } // release image

	// build mipmap
	if (mipmap)
	{
		int mip_levels = 0; for (int k = w > h ? w : h; k; k >>= 1) mip_levels++;
		for (int l = 1; l < mip_levels; l++)
			glTexImage2D(GL_TEXTURE_2D, l, internal_format, (w >> l) == 0 ? 1 : (w >> l), (h >> l) == 0 ? 1 : (h >> l), 0, format, GL_UNSIGNED_BYTE, nullptr);
		if (glGenerateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
	}

	// set up texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, !mipmap ? filter : filter == GL_LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);

	return texture;
}

inline GLuint cg_create_texture(const char* image_path, bool mipmap = true, GLenum wrap = GL_CLAMP_TO_EDGE, GLenum filter = GL_LINEAR)
{
	return cg_create_texture(cg_load_image(image_path), mipmap, wrap, filter);
}

#endif // __CGUT_H__
