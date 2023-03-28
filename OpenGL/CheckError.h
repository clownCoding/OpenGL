#pragma once
#ifndef CHECKERROR_H_
#define CHECKERROR_H_
#include <GL/glew.h>
#include <iostream>


#ifndef glCheckError()
GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__);
#endif

#endif /* CHECKERROR_H_ */
