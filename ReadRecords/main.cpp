#include <iostream>
#include <fstream>
#include "bass.h"
#include "bassasio.h"

#define FILE_PATH "record"
#define BUFFER_LEN 2048

using namespace std;

void main() {
	//Init libs
	if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
		cout << "BASS ERROR: " << BASS_ErrorGetCode() << endl;
		system("pause");
		return;
	}
	if (!BASS_ASIO_Init(0, BASS_ASIO_THREAD)) {
		cout << "BASSASIO ERROR: " << BASS_ASIO_ErrorGetCode() << endl;
		system("pause");
		return;
	}

	
/*	float payback = 0;
	float pre_payback = 0;
	if (fin.is_open()) {
		while (!fin.eof()) {
			fin.read((char*)&payback, sizeof(payback));
			cout << payback;
			pre_payback = payback;
		}
	}*/

	float* buffer = new float[BUFFER_LEN];

	ifstream fin( FILE_PATH, ios::binary );
	if (fin.is_open()) {
		float payback;		
		HSTREAM stream = BASS_StreamCreate(BASS_ASIO_GetRate(), 1, BASS_STREAM_DECODE, STREAMPROC_PUSH, 0);
		bool empty = false;
		int payback_index = 0;

		BASS_ASIO_ChannelEnableBASS(FALSE, 0, stream, TRUE);
		BASS_ASIO_Start(0, 0);

		int bass_err = BASS_ErrorGetCode();
		int bass_asio_err = BASS_ASIO_ErrorGetCode();

		while (!empty) {
			while (!fin.eof() && payback_index < BUFFER_LEN / 4) {
				fin.read((char*)&payback, sizeof(payback));
				buffer[payback_index++] = payback;
				if (fin.eof()) {
					empty = true;
					cout << "End!!!" << endl;
				}
			}
			cout << "Sample has just been read" << endl;
			BASS_StreamPutData(stream, buffer, BUFFER_LEN);
			payback_index = 0;
		}		
	}
	else {
		cout << "Error: File is not opened!" << endl;
	}
	
	system("pause");	
	delete buffer;
	fin.close();
	BASS_ASIO_Stop();
	BASS_ASIO_Free();
	return;
}