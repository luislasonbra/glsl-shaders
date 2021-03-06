
#include <iostream>
#include <math.h>
#include <stdio.h>     // Standard C/C++ IO  

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <GL/glut.h>   // The GL Utility Toolkit (GLUT) Header

#include <png.h>
#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace std;

GLubyte* png_texture_load(const char * file_name,
                          int * width, int * height);

string readFile(const char *filePath);

GLuint   program_object;  // a handler to the GLSL program used to update
GLuint   vertex_shader;   // a handler to vertex shader. This is used internally 
GLuint   fragment_shader; // a handler to fragment shader. This is used internally too

/// Print out the information log for a shader object 
/// @arg obj handle for a program object
static void printProgramInfoLog(GLuint obj)
{
	GLint infologLength = 0, charsWritten = 0;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 2) {
		GLchar* infoLog = new GLchar [infologLength];
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		std::cerr << infoLog << std::endl;
		delete infoLog;
	}
}

// Our GL Specific Initializations
bool init(void){
   glClearColor(0.7f, 0.7f, 0.7f, 0.5f);	// Black Background

    // Read shaders
    std::string vertShaderStr = readFile("css_box_shadow.vert");
    std::string fragShaderStr = readFile("css_box_shadow.frag");
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();


   program_object  = glCreateProgram();    // creating a program object
   vertex_shader   = glCreateShader(GL_VERTEX_SHADER);   // creating a vertex shader object
   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); // creating a fragment shader object
   printProgramInfoLog(program_object);

   glShaderSource(vertex_shader, 1, &vertShaderSrc, NULL); // assigning the vertex source
   glShaderSource(fragment_shader, 1, &fragShaderSrc, NULL); // assigning the fragment source
   printProgramInfoLog(program_object);   // verifies if all this is ok so far

   // compiling and attaching the vertex shader onto program
   glCompileShader(vertex_shader);
   glAttachShader(program_object, vertex_shader);
   printProgramInfoLog(program_object);   // verifies if all this is ok so far

   // compiling and attaching the fragment shader onto program
   glCompileShader(fragment_shader);

   int status;
   glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);

   if(status != GL_TRUE){
     std::cout << "The shader could not be compiled\n" << std::endl;
     char errorlog[500] = {'\0'};
     glGetShaderInfoLog(fragment_shader, 500, 0, errorlog);
     std::cout << "Error: " << errorlog << std::endl;
   }

   glAttachShader(program_object, fragment_shader);
   printProgramInfoLog(program_object);   // verifies if all this is ok so far

   // Link the shaders into a complete GLSL program.
   glLinkProgram(program_object);
   printProgramInfoLog(program_object);   // verifies if all this is ok so far

   // some extra code for checking if all this initialization is ok
   GLint prog_link_success;
   glGetObjectParameterivARB(program_object, GL_OBJECT_LINK_STATUS_ARB, &prog_link_success);
   if (!prog_link_success) {
      fprintf(stderr, "The shaders could not be linked\n");
      exit(1);
   }

	return true;
}

// Our rendering is done here
void render(void)  {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); //I've seen this on most tutorials
  glDisable(GL_DEPTH_TEST);

  int img_s_x, img_s_y;
  img_s_x = 500;
  img_s_y = 487;

  // load texture
  GLubyte* texture=png_texture_load("rect.png",
                                    &img_s_x, &img_s_y);

  if (!texture) {
    cout << "error loading texture" << endl;
  }

  GLuint textureID[1];
  glGenTextures(1, textureID);

  glBindTexture(GL_TEXTURE_2D, textureID[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, img_s_x, img_s_y, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)texture );

  // set texture uniform
  GLint baseImageLoc = glGetUniformLocation(program_object, "texture");

  glUniform1i(baseImageLoc, 0);

	// bind the GLSL program	
	// this means that from here the GLSL program attends all OpenGL operations
//	glUseProgram(program_object);

  glColor4f(1.0,0.0,0.0, 0.5);


  // painting a quad
	glBegin(GL_QUADS);
     glVertex3f(-0.5, -0.5, 0.0);
	   glVertex3f(0.5, -0.5, 0.0);
	   glVertex3f(0.5, 0.5, 0.0);
	   glVertex3f(-0.5, 0.5, 0.0);
	glEnd();

  //glUseProgram(0);


  // unbind the GLSL program
	// this means that from here the OpenGL fixed functionality is used
	glUseProgram(program_object);

  glColor4f(1.0,1.0,1.0, 0.5f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // bind texture
  glActiveTexture(GL_TEXTURE0 + 0);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID[0]);
  glBegin(GL_QUADS);
    glTexCoord2d( 0.0f, 0.0f);
    glVertex3f( -1.0f, -1.0f, 0.0f );
    glTexCoord2d( 0.0f, 1.0f);
    glVertex3f( -1.0f, 1.0f, 0.0f );
    glTexCoord2d( 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, 0.0f );
    glTexCoord2d( 1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, 0.0f );
  glEnd();
  glDisable(GL_TEXTURE_2D);

  glUseProgram(0);
   // Swap The Buffers To Become Our Rendering Visible
   glutSwapBuffers( );
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	// Calculate The Aspect Ratio And Set The Clipping Volume
	if (h == 0) h = 1;
	gluPerspective(45, (float)w/(float)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
	glLoadIdentity();                // Reset The Modelview Matrix
}

// Our keyboard handler
void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case 27:       // When escape is pressed...
			exit(0);    // Exit The Program
		   break;
		default:
		break;
	}
}



// Main Function For Bringing It All Together.
int main(int argc, char** argv){
	glutInit(&argc, argv);                           // GLUT Initializtion
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);     // Display Mode (Rgb And Double Buffered)	
	glutCreateWindow("GLSL Hello World!");          // Window Title 

  init();                                          // Our Initialization
	glutDisplayFunc(render);                         // Register The Display Function
	glutReshapeFunc(reshape);                        // Register The Reshape Handler
	glutKeyboardFunc(keyboard);                      // Register The Keyboard Handler	
//	glutIdleFunc(render);                            // Do Rendering In Idle Time
	glutMainLoop();                                  // Go To GLUT Main Loop
	return 0;
}


GLubyte* png_texture_load(const char * file_name,
                          int * width, int * height) {
    png_byte header[8];

    FILE *fp = fopen(file_name, "rb");
    if (fp == 0)
    {
        perror(file_name);
        return 0;
    }

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: %s is not a PNG.\n", file_name);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = (png_byte*)malloc(rowbytes * temp_height * sizeof(png_byte)+15);
    if (image_data == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = (png_byte**)malloc(temp_height * sizeof(png_bytep));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        free(image_data);
        fclose(fp);
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    return (GLubyte*)image_data;
}

string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

