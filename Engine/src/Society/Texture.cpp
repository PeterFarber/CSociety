
#include <string>
#include <Society/Texture.h>

#include <glad/glad.h>

namespace sc {
	Texture2D::Texture2D()
		: ID(), Width(0), Height(0), Internal_Format(GL_RGB),
		  Image_Format(GL_RGB),
		  Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR),
		  Filter_Max(GL_LINEAR) {
		std::string s;
		glGenTextures(1, &this->ID);
	}

	void Texture2D::Generate(
		const unsigned int width,
		const unsigned int height,
		const unsigned char *data
	) {
		this->Width = width;
		this->Height = height;
		// create Texture
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			static_cast<GLint>(this->Internal_Format),
			static_cast<int>(width),
			static_cast<int>(height),
			0,
			this->Image_Format,
			GL_UNSIGNED_BYTE,
			data
		);

		// set Texture wrap and filter modes
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			static_cast<GLint>(this->Wrap_S)
		);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			static_cast<GLint>(this->Wrap_T)
		);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			static_cast<GLint>(this->Filter_Min)
		);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			static_cast<GLint>(this->Filter_Max)
		);
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::Bind() const {
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}
}
