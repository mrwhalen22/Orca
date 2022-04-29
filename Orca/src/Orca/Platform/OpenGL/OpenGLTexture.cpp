#include "oapch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

#include <glad/glad.h>

namespace Orca
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) 
		:m_Width(width), m_Height(height)
	{
		OA_PROFILE_FUNCTION();
		GLenum internalFormat = GL_RGBA8, dataFormat = GL_RGBA;
		m_InternalFormat = internalFormat;
		m_Format = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		OA_PROFILE_FUNCTION();
		int channels, width, height;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			OA_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D(const std::string&) - Texture Load");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		OA_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width; m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if(channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if(channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_Format = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		OA_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		OA_PROFILE_FUNCTION();
		uint32_t bpp = m_Format == GL_RGBA ? 4 : 3;
		OA_CORE_ASSERT(size == m_Width * m_Height * bpp, "Texture size is incorrect!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		OA_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}


}
