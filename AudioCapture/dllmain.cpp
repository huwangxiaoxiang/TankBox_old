// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")

HWAVEIN phwi;
WAVEFORMATEX pwfx;
WAVEINCAPS waveIncaps;
int buffer_length = 10240;
extern "C" __declspec(dllexport)
typedef void(__stdcall* RecordCallBack)(LPSTR ss);

RecordCallBack CallBackfun = NULL;
LPSTR buffers = new CHAR[buffer_length];


WAVEHDR CreateWaveHDR() {
	WAVEHDR pwh;
	CHAR* buffer1 = new CHAR[buffer_length];
	pwh.lpData = buffer1;
	pwh.dwBufferLength = buffer_length;
	pwh.dwUser = 1;
	pwh.dwFlags = 0;
	return pwh;
}

WAVEHDR buffer = CreateWaveHDR();

void WaveInitFormat(LPWAVEFORMATEX m_WaveFormat, WORD nCh, DWORD nSampleRate, WORD BitsPerSample)
{
	m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormat->nChannels = nCh;
	m_WaveFormat->nSamplesPerSec = nSampleRate;
	m_WaveFormat->nAvgBytesPerSec = nSampleRate * nCh * BitsPerSample / 8;
	m_WaveFormat->nBlockAlign = m_WaveFormat->nChannels * BitsPerSample / 8;
	m_WaveFormat->wBitsPerSample = BitsPerSample;
	m_WaveFormat->cbSize = 0;
}

extern "C" __declspec(dllexport)
void CALLBACK MicCallback(HWAVEIN hwavein, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	switch (uMsg)
	{
	case WIM_OPEN:
		OutputDebugString(L"设备打开成功\n");
		break;

	case WIM_DATA:
		OutputDebugString(L"缓冲区满\n");
		if (CallBackfun != NULL) {
			buffers[0] = '\0';
			lstrcpyA(buffers, ((LPWAVEHDR)dwParam1)->lpData);
			CallBackfun(buffers);
		}
		waveInPrepareHeader(hwavein, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));//4
		waveInAddBuffer(hwavein, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));
		break;

	case WIM_CLOSE:
		OutputDebugString(L"设备已关闭\n");
		break;
	default:
		break;
	}
	
}

BOOL OpenDevice() {
	MMRESULT mmResult = waveInGetDevCaps(0, &waveIncaps, sizeof(WAVEINCAPS));
	if (MMSYSERR_NOERROR == mmResult) {
		WaveInitFormat(&pwfx, 1, 8000, 16);
		mmResult = waveInOpen(&phwi, WAVE_MAPPER, &pwfx, (DWORD_PTR)(MicCallback), NULL, CALLBACK_FUNCTION);//3
		
		if (MMSYSERR_NOERROR == mmResult) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

extern "C" _declspec(dllexport)
void StartRecord(RecordCallBack func) {
	/**/CallBackfun = func;
	if (!OpenDevice()) {
		OutputDebugString(L"打开录音设备出错!\n");
		return;
	}
	waveInPrepareHeader(phwi, &buffer, sizeof(WAVEHDR));//4
	waveInAddBuffer(phwi, &buffer, sizeof(WAVEHDR));
	MMRESULT m = waveInStart(phwi);
	if (MMSYSERR_NOERROR == m)
		OutputDebugString(L"已开始录音!\n");
	else
		OutputDebugString(L"开始录音出错!\n");

}

extern "C" _declspec(dllexport)
void StopRecord() {
	waveInStop(phwi);
	OutputDebugString(L"已停止录音!\n");
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(L"缓冲区已初始化\n");
		break;
	case DLL_THREAD_ATTACH:
		OutputDebugString(L"缓冲区已初始化\n");
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


