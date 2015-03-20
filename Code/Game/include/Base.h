#pragma once

#include <cstdint>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <memory>
#include <chrono>
#include <queue>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>
#include <array>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef WIN32
#	include <GL/glew.h>
#elif __APPLE__
#	define GL3_PROTOTYPES 1
#	include <OpenGL/gl3.h>
#else
#	define GL3_PROTOTYPES 1
#	include <GL3/gl3.h>
#endif

#ifdef __APPLE__
#	include <CoreFoundation/CoreFoundation.h>
#endif

#define _ELPP_DEFAULT_LOG_FILE "Game.log"
#include "easylogging++.h"

#include "System.h"