
#include<vector>

struct VertexBufferSource
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		}
	}
};

class VertexBufferLayout
{
public:
	std::vector<VertexBufferSource> m_Elements;
	unsigned int m_Stride;

	VertexBufferLayout() :m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * sizeof(float);
	}

	inline unsigned int GetStride() { return m_Stride; }
	inline const std::vector<VertexBufferSource>& GetElements() { return m_Elements; }
};