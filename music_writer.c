#include<stdio.h>
#include<stdint.h>
#include<math.h>

int main(int argc, char **argv){

	//God save me i hate coding this is such dogshit fucking kill me
	//i have to do the mono sterro thing today

	uint16_t no_channels = 1;
	uint32_t sample_rate = 22050;
	uint16_t bits_per_sample = 16;
	uint32_t bytes_per_second = (sample_rate * bits_per_sample * no_channels)/8;
	uint16_t bit_channel_product = (bits_per_sample * no_channels)/8;

	uint32_t total_time_sec = 60;
	uint32_t size_of_data = bytes_per_second * total_time_sec;
	uint32_t no_of_samples = sample_rate * total_time_sec;

	uint8_t header[44]={
		'R','I','F','F',      // RIFF

		(uint8_t)((size_of_data + 36) & 0xFF),
		(uint8_t)(((size_of_data + 36) >> 8) & 0xFF),
		(uint8_t)(((size_of_data + 36) >> 16) & 0xFF),
		(uint8_t)(((size_of_data + 36) >> 24) & 0xFF), // Size of file - 8

		'W','A','V','E',      // Wave
		'f','m','t',0x20,     // fmt with a null character
		0x10,0,0,0,           // size of formatted data (16)
		1,0,                  // Type of format (1 for PCM)

		(uint8_t)(no_channels & 0xFF),
		(uint8_t)((no_channels >> 8) & 0xFF),  // Number of channels

		(uint8_t)(sample_rate & 0xFF),
		(uint8_t)((sample_rate >> 8) & 0xFF),
		(uint8_t)((sample_rate >> 16) & 0xFF),
		(uint8_t)((sample_rate >> 24) & 0xFF), // Sample Rate 

		(uint8_t)(bytes_per_second & 0xFF),
		(uint8_t)((bytes_per_second >> 8) & 0xFF),
		(uint8_t)((bytes_per_second >> 16) & 0xFF),
		(uint8_t)((bytes_per_second >> 24) & 0xFF), // Bytes per second

		(uint8_t)(bit_channel_product & 0xFF),
		(uint8_t)((bit_channel_product >> 8) & 0xFF),  // Bytes per second/Sample rate 
		
		(uint8_t)(bits_per_sample & 0xFF),
		(uint8_t)((bits_per_sample >> 8) & 0xFF),  // Bits per sample

		'd','a','t','a',      // data

		(uint8_t)(size_of_data & 0xFF),
		(uint8_t)((size_of_data >> 8) & 0xFF),
		(uint8_t)((size_of_data >> 16) & 0xFF),
		(uint8_t)((size_of_data >> 24) & 0xFF) // Size of file - 44
	};


	uint8_t audio_data[size_of_data];
	int16_t sample;
	float frequency1 = 440;
	float frequency2 = 480;
	//uint32_t samples_per_wave = sample_rate/frequency;
	for(uint32_t i=0; i<no_of_samples;i++){

		sample = ((1<<(bits_per_sample-1))-1)*(0.5*(sin(2*i*M_PI*frequency1/sample_rate)) + 0.5*(sin(2*i*M_PI*frequency2/sample_rate)));

		for(int j = 0; j<bit_channel_product; j++){
			audio_data[bit_channel_product*i + j] = (0xFF & (sample>>(8*j)));
		}

	}

	FILE* audiofile;
	char *output_path;

	if(argc>1){
		output_path = argv[1];
	} else {
		output_path = "output.wav";
	}

	audiofile = fopen(output_path,"wb");
	// Just writing the header
	fwrite(header,sizeof(uint8_t),44,audiofile); 
	// Writing the actual data
	fwrite(audio_data,sizeof(uint8_t),size_of_data,audiofile); 
	fclose(audiofile);
	return 0;
}
