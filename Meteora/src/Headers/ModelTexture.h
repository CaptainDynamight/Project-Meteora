#pragma once

class ModelTexture
{
	private:
		int textureID;
		float shineDamper = 1;
		float reflectivity = 0;
		bool hasTransparency = false;
		bool useFakeLighting = false;

	public:
		ModelTexture()
		{

		}

		ModelTexture(int id)
		{
			this->textureID = id;
		}

		void setHasTransparecny(bool hasTransparency)
		{
			this->hasTransparency = hasTransparency;
		}

		bool getHasTransparency()
		{
			return hasTransparency;
		}

		void setUseFakeLighting(bool useFakeLighting)
		{
			this->useFakeLighting = useFakeLighting;
		}

		bool getUseFakeLighting()
		{
			return useFakeLighting;
		}

		unsigned int getID()
		{
			return this->textureID;
		}

		float getShineDamper()
		{
			return shineDamper;
		}

		float getReflectivity()
		{
			return reflectivity;
		}

		void setShineDamper(float shineDamper)
		{
			this->shineDamper = shineDamper;
		}

		void setReflectivity(float reflectivity)
		{
			this->reflectivity = reflectivity;
		}
};