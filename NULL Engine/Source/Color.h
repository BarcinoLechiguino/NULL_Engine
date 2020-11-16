#ifndef __COLOR_H__
#define __COLOR_H__

struct Color
{
	float r, g, b, a;
	
	Color();
	Color(float r, float g, float b, float a = 1.0f);

	// --- GET/SET
	Color Get();														// Will Return the Color vector in Range: 0.0f ~ 1.0f. Will be checked and mod. to be within the limits before return.
	Color GetRGB255();													// Will Return the Color vector in Range: 0.0f ~ 255.0f. Will be checked & mod. to be within the limits before return.

	void Set(float r, float g, float b, float a = 1.0f);				// Will set the Color vector with the given RGBA values. Will be checked and mod. to be within limits. 
	void Set(const Color& color);										// Both Set() methods expec the RGBA values to be in Range: 0.0f ~ 1.0f.
	void SetFromRGB255(const Color& color);								// Will set the Color vector with the given RGBA values in Range: 0.0f ~ 255.0f in the passed Color vector.
	
	void SetWithinLimits();												// Will modify this vector to set the RGBA values within Range: 0.0f ~ 1.0f. Used in Gets and Sets.

	// --- OPERATORS
	float* operator &();

	Color operator +(const Color& color);								// It is expected that the passed parameters are within the range of 0.0f to 1.0f.
	Color operator -(const Color& color);								// It is expected that the passed parameters are within the range of 0.0f to 1.0f.
	Color operator *(const float& scalar);								// 
	Color operator /(const float& scalar);								// 

	Color operator +=(const Color& color);								// It is expected that the passed parameters are within the range of 0.0f to 1.0f.
	Color operator -=(const Color& color);								// It is expected that the passed parameters are within the range of 0.0f to 1.0f.
	Color operator *=(const float& scalar);								// 
	Color operator /=(const float& scalar);								// 
};

extern Color Red;
extern Color Green;
extern Color Blue;
extern Color Black;
extern Color White;

#endif // !__COLOR_H__