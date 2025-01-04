#include <UI/text.h> 
#include <DataTbls/StringIds.h>

#ifdef D2_VERSION_110F

//1.10f: D2Client.0x6FB20430
//1.13c: D2Client.0x6FB6F5F0
Unicode* __fastcall TEXT_ApplyColorCode(Unicode* wszStringArg, uint8_t nColorArg)
{
	Unicode wszBuffer[1024];
	wszBuffer[0] = 0;

	if (*wszStringArg)
	{
		Unicode::strcpy(wszBuffer, D2LANG_GetStringFromTblIndex(STR_IDX_3994_colorcode));
		char szColorCode[3];
		wsprintfA(szColorCode, "%c", (char)(nColorArg + '0'));
		Unicode wszColorCode[3];
		Unicode::win2Unicode(wszColorCode, szColorCode, ARRAY_SIZE(wszColorCode) - 1);
		Unicode::strcat(wszBuffer, wszColorCode);
		Unicode::strcat(wszBuffer, wszStringArg);
		Unicode::strcpy(wszStringArg, wszBuffer);
	}
	return wszStringArg;
}

#endif //D2_VERSION_110F
