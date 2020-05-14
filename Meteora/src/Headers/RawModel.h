#pragma once
class RawModel
{
	private:
		unsigned int vaoID;
		int vertexCount;
	
	public:
		RawModel() = default;
		RawModel(int vaoID, int vertexCount)
		{
			this->vaoID = vaoID;
			this->vertexCount = vertexCount;
		}

		unsigned int getVaoID()
		{
			return vaoID;
		}

		int getVertexCount()
		{
			return vertexCount;
		}
};