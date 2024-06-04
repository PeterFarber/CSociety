#ifndef SC_RESOURCE_MANAGER_H
#define SC_RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <Society/Shader.h>
#include <Society/Core/Base.h>

#include <Society/Texture.h>

namespace sc {
	// A static singleton ResourceManager class that hosts several
	// functions to load m_textures and m_shaders. Each loaded texture
	// and/or shader is also stored for future reference by string
	// handles. All functions and resources are static and no 
	// public constructor is defined.
	class DECLSPEC ResourceManager {
		private:
			// resource storage
			std::map<std::string, Shader *> m_shaders;
			std::map<std::string, Texture2D *> m_textures;

		public:
			ResourceManager();

			// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
			Shader *LoadShader(
				const char *vShaderFile,
				const char *fShaderFile,
				const char *gShaderFile,
				const std::string &name
			);

			// retrieves a stored sader
			Shader *GetShader(const std::string &name);

			// loads (and generates) a texture from file
			Texture2D *LoadTexture(
				const char *file,
				bool alpha,
				const std::string &name
			);

			// retrieves a stored texture
			Texture2D *GetTexture(const std::string &name);

			// properly de-allocates all loaded resources
			void Clear();

		private:
			// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).

			// loads and generates a shader from file
			static Shader *loadShaderFromFile(
				const char *vShaderFile,
				const char *fShaderFile,
				const char *gShaderFile = nullptr
			);

			// loads a single texture from file
			static Texture2D *loadTextureFromFile(const char *file, bool alpha);
	};
}
#endif
