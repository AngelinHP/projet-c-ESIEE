///////////////////////////////////////////////////////////
//
//    deprecated ??
//
///////////////////        PNG_READER     /////////////////

#include "GX.h"
#include "_lodepng.h"
#include "GlutImport.h"

using byte = unsigned char;



//int  CORECreateTextureFromRGBA(byte* data, int width, int height);  // create texture from RGBA data (G2D.cpp)
//void ExtendRGBtoRGBA(vector<byte>& data);							// convert RGB data to RGBA data
void VsymetryRGBAImage(vector<byte>& data, int Width, int Height);  // Vertical-symetry
void SetRGBATransparentPixels(vector<byte>& data, byte RA, byte GA, byte BA); // any pixel with given RGB color value becomes transparent
void LoadPNG(const std::string& filepath, std::vector<byte>& image, unsigned int& width, unsigned int& height); // call lodepng


int GX::ExtractTextureFromPNG(const std::string& filepath, Transparency T)
{
	std::vector<byte> image; //the raw pixels
	unsigned int width, height;

	LoadPNG(filepath, image, width, height);

	if (T != Transparency::None)
	{
		int index = 0; // Transparency::BottomLeft
		if (T == Transparency::BottomRight)  index = 4 * (width - 1);
		if (T == Transparency::UpperLeft)    index = 4 * width * (height - 1);
		if (T == Transparency::UpperRight)   index = 4 * width * height - 4;

		SetRGBATransparentPixels(image, image[index], image[index + 1], image[index + 2]);
	}

	int id = 0; // CORECreateTextureFromRGBA(image.data(), width, height);
	return id;
}

int GX::ExtractTextureFromPNG(const std::string& filepath, int RT, int GT, int BT)
{
	std::vector<byte> image; //the raw pixels
	unsigned int width, height;

	LoadPNG(filepath, image, width, height);

	SetRGBATransparentPixels(image, RT, GT, BT);

	int id = 0;// CORECreateTextureFromRGBA(image.data(), width, height);
	return id;
}



// 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
void LoadPNG(const std::string& filepath, std::vector<byte>& image, unsigned int& width, unsigned int& height)
{
	//decode
	unsigned error = lodepng::decode(image, width, height, filepath.c_str());

	//if there's an error, display it
	if (error)
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		cout.flush();
	}

	VsymetryRGBAImage(image, width, height);

}




// Vertical-symetry
void VsymetryRGBAImage(vector<byte>& data, int Width, int Height)
{
	for (int y = 0; y < Width / 2; y++)
	{
		int width = Width * 4;
		int pY = y * width;
		int sY = (Height - y - 1) * width;

		for (int x = 0; x < width; x++)
		{
			unsigned char t = data[pY + x];
			data[pY + x] = data[sY + x];
			data[sY + x] = t;
		}
	}
}

// any pixel with given RGB color value becomes transparent
void SetRGBATransparentPixels(vector<byte>& data, byte RA, byte GA, byte BA)
{

	int nbPixels = data.size() / 4;

	int p = 0;
	for (int i = 0; i < nbPixels; i++)
	{
		if (data[p] == RA && data[p + 1] == GA && data[p + 2] == BA)
			data[p + 3] = 0;
		p += 4;
	}
}