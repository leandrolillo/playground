#ifndef AUDIORESOURCE_H_
#define AUDIORESOURCE_H_

#include "../../Resource.h"

enum class AudioFormat {
	MONO8 = 8,
	MONO16,
	STEREO8,
	STEREO16
};

class AudioResource : public Resource
{
	private:
		AudioFormat format;
		uint32_t frequency;
		std::vector<char> data;

	public:
		AudioResource() : Resource(0, MimeTypes::AUDIO) {
			frequency = 0;
			format = AudioFormat::MONO8;
		}
		std::vector<char> getData() const {
			return data;
		}

		void setData(const std::vector<char> &data) {
			this->data = data;
		}

		AudioFormat getFormat() const {
			return format;
		}

		void setFormat(AudioFormat format) {
			this->format = format;
		}

		uint32_t getFrequency() const {
			return frequency;
		}

		void setFrequency(uint32_t freq) {
			this->frequency = freq;
		}

};
#endif
