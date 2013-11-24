#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

#pragma link "winmm.lib"	//! sndPlaySound

int ctomorse(char chr, char *pattern){
	switch(toupper(chr)){
	case 'A': strcpy(pattern, ". ___");			break;
	case 'B': strcpy(pattern, "___ . . .");		break;
	case 'C': strcpy(pattern, "___ . ___ .");	break;
	case 'D': strcpy(pattern, "___ . .");		break;
	case 'E': strcpy(pattern, ".");				break;
	case 'F': strcpy(pattern, ". . ___ .");		break;
	case 'G': strcpy(pattern, "___ ___ .");		break;
	case 'H': strcpy(pattern, ". . . .");		break;
	case 'I': strcpy(pattern, ". .");			break;
	case 'J': strcpy(pattern, ". ___ ___ ___");	break;
	case 'K': strcpy(pattern, "___ . ___");		break;
	case 'L': strcpy(pattern, ". ___ . .");		break;
	case 'M': strcpy(pattern, "___ ___");		break;
	case 'N': strcpy(pattern, "___ .");			break;
	case 'O': strcpy(pattern, "___ ___ ___");	break;
	case 'P': strcpy(pattern, ". ___ ___ .");	break;
	case 'Q': strcpy(pattern, "___ ___ . ___");	break;
	case 'R': strcpy(pattern, ". ___ .");		break;
	case 'S': strcpy(pattern, ". . .");			break;
	case 'T': strcpy(pattern, "___");			break;
	case 'U': strcpy(pattern, ". . ___");		break;
	case 'V': strcpy(pattern, ". . . ___");		break;
	case 'W': strcpy(pattern, ". ___ ___");		break;
	case 'X': strcpy(pattern, "___ . . ___");	break;
	case 'Y': strcpy(pattern, "___ . ___ ___");	break;
	case 'Z': strcpy(pattern, "___ ___ . .");	break;

	case '1': strcpy(pattern, ". ___ ___ ___ ___");		break;
	case '2': strcpy(pattern, ". . ___ ___ ___");		break;
	case '3': strcpy(pattern, ". . . ___ ___");			break;
	case '4': strcpy(pattern, ". . . . ___");			break;
	case '5': strcpy(pattern, ". . . . .");				break;
	case '6': strcpy(pattern, "___ . . . .");			break;
	case '7': strcpy(pattern, "___ ___ . . .");			break;
	case '8': strcpy(pattern, "___ ___ ___ . .");		break;
	case '9': strcpy(pattern, "___ ___ ___ ___ .");		break;
	case '0': strcpy(pattern, "___ ___ ___ ___ ___");	break;

	default	: *pattern = 0;
	}
	
	return strlen(pattern);
}

unsigned long get_ms(void){
	return 1000.0 * clock() / CLOCKS_PER_SEC + 0.5;
}

int beep_on(int on){
	static LPVOID lpSound = 0;
	static int current_on = 0;
	
	if(lpSound == 0){
		HANDLE fh;
		DWORD dummy;
		fh = CreateFile("440Hz.wav", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		lpSound = GlobalAlloc(GPTR, GetFileSize(fh, NULL));
		ReadFile(fh, lpSound, GetFileSize(fh, NULL), &dummy, NULL);
		CloseHandle(fh);
		current_on = 0;
	}
	
	if(current_on != on){
		sndPlaySound(on ? lpSound : 0, SND_ASYNC|SND_MEMORY|SND_LOOP);
		current_on = on;
	}

	return current_on;
}

int main(int argc, char *argv[]){
	char chr;
	char *str;
	int c_count;
	unsigned long ms0, ms1;
	const int ms_duration = 250;
	
	if(argc == 1){
		str = "PARIS";
	}else{
		str = argv[1];
	}
	
	c_count = 0;
	ms0 = get_ms();

	while((chr = *str++)){
		if(chr <= 0x20){
			beep_on(0);
			Sleep(ms_duration * 7);
			c_count += 7;
		}else{
			char pattern[18];
			char pattern_length;
			int i;

			pattern_length = ctomorse(chr, pattern);
			printf("%c => %s\n", chr, pattern);
			
			for(i = 0; i < pattern_length; i++){
				beep_on(isspace(pattern[i]) ? 0 : 1);
				Sleep(ms_duration);
			}

			beep_on(0);
			Sleep(ms_duration * 3);
	
			c_count += pattern_length + 3;
		}
	}

	ms1 = get_ms();
	printf("%d / %.1lfs\n", c_count, (ms1 - ms0) / 1000.0);
	return 0;
}

/*
int main(int argc, char *argv[]){
	char pattern[18];
	char pattern_length;
	
	if(argc == 2){
		pattern_length = ctomorse(argv[1][0], pattern);
	}else{
		pattern_length = ctomorse('A', pattern);
	}

	strcat(pattern, "000");
	pattern_length += 3;
	
	while(1){
		static int i0 = -1;
		unsigned long ms = get_ms();
		int i = (ms / 250) % pattern_length;

		if(i != i0){
			beep_on(pattern[i] & 0x01);
			i0 = i;
		}
		
		if(10000 < ms){
			break;
		}
	}
	
	beep_on(0);	//! ‚È‚­‚Ä‚à—Ç‚¢
	return 0;
}
*/
