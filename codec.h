#ifndef CODEC_H
#define CODEC_H

#include <string>
#include <vector>

extern "C" {
#include <stdint.h>
}

struct AVCodecContext;
struct AVCodec;
class Atom;

class Codec {
public:
	std::string     name;
	AVCodecContext *context;
	AVCodec        *codec;

	Codec();

	bool parse(Atom *trak, std::vector<int> &offsets, Atom *mdat);
	void clear();

	bool matchSample(const unsigned char *start, int maxlength);
	bool isKeyframe (const unsigned char *start, int maxlength);
	int  getLength  (      unsigned char *start, int maxlength, int &duration);

private:
	// Used by mp4a.
	int mask1;
	int mask0;
};


// Read an unaligned, big-endian value.
// A compiler will optimize this (at -O2) to a single instruction if possible.
template<class T>
static inline T readBE(const uint8_t *p, size_t i = 0) {
	return (i >= sizeof(T)) ? T(0) :
			(T(*p) << ((sizeof(T) - 1 - i) * 8)) | readBE<T>(p + 1, i + 1);
};

template<class T>
static inline void readBE(T &result, const uint8_t *p) { result = readBE<T>(p); };

// Write an unaligned, big-endian value.
template<class T>
static inline void writeBE(uint8_t *p, T value, size_t i = 0) {
	(i >= sizeof(T)) ? void(0) :
		(*p = ((value >> ((sizeof(T) - 1 - i) * 8)) & 0xFF) , writeBE(p + 1, value, i + 1));
};


#endif // CODEC_H
