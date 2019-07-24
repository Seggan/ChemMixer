/*Create Mario Royale World*/
void CreateJSONWorld(FILE* stream, char* fileOut, size_t wide, size_t tall) {
	/*Use register, they are faster*/
	size_t aReg = 0;
	size_t bReg = 0;
	if (fileOut == NULL) {
		die("Invalid fileOut in CreateJSONWorld",7);
	}
	if (strlen(fileOut) == 0 || fileOut[0] == '\0') {
		die("Unlenght fileOut in CreateJSONWorld",8);
	}
	/*Open file in write mode*/
	stream = fopen(fileOut, "wb");
	if (!stream) {
		die("File is locked",9);
	}
	/*Write the default header*/
	fprintf(stream, "\t{\n\t%ctype%c:%cgame%c,\n\t%cresource%c: [\n\t\t{\n\t\t\t%cid%c:%cmap%c,\n", 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22);
	fprintf(stream, "\t\t\t%csrc%c:%cimg/game/smb_map.png%c\n\t\t},\n\t\t{\n\t\t\t%cid%c:%cobj%c,", 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22);
	fprintf(stream, "\n\t\t\t%csrc%c:%cimg/game/smb_obj.png%c\n\t\t}\n\t],\n\t%cinitial%c:0,\n\t%c", 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22);
	fprintf(stream, "world%c:[\n\t\t{\n\t\t\t%cid%c:0,\n\t\t\t%cname%c:%cneworld%c,\n\t\t\t%cinitial%c", 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22);
	fprintf(stream, ":0,\n\t\t\t%czone%c:[{\n\t\t\t\t%cid%c:0,\n\t\t\t\t%cinitial%c:0,", 0x22, 0x22, 0x22, 0x22, 0x22, 0x22);
	fprintf(stream, "\n\t\t\t\t%cmusic%c:%chttp://marioroyale.cyuubi.gq/audio/music/main0.mp3%c,", 0x22, 0x22, 0x22, 0x22);
	fprintf(stream, "\n\t\t\t\t%ccolor%c:%c#5986ED%c,\n\t\t\t\t%cdata%c:[", 0x22, 0x22, 0x22, 0x22, 0x22, 0x22);
	/*Write data*/
	/*Here goes the function to write the data*/
	for (aReg = 0; aReg < tall; aReg++) {
		fprintf(stream, "\n\t\t\t\t\t[");
		for (bReg = 0; bReg < wide; bReg++) {
			fprintf(stream, "\n\t\t\t\t\t\t ");
			if (bReg != wide-1) {
				fprintf(stream, "28,");
			}
			else {
				fprintf(stream, "28");
			}
		}
		fprintf(stream, "\n\t\t\t\t\t");
		if (aReg != tall-1) {
			fprintf(stream, "],");
		}
		else {
			fprintf(stream, "]");
		}
	}
	/*Footer*/
	fprintf(stream, "\n\t\t\t\t],\n\t\t\t\t%cobj%c:[],\n\t\t\t\t%cwarp%c:[]\n\t\t\t\t}\n\t\t\t]\n\t\t}\n\t]\n}",0x22,0x22,0x22,0x22);
	/*Finaly, close the file*/
	fclose(stream);
	return;
}

/*Not for arrays*/
char * ReadJSON(FILE *stream) {
	char * value;
	unsigned char a = 0;
	char read = 0;
	if (!stream) {
		die("Unusable file",2);
	}
	value = (char *)malloc(255);
	/*Check if we are null*/
	if (value == NULL) {
		die("Allocation for VALUE in RJ null", 38);
	}
	/*Set all to null, even if we put the
	null terminator, printf can mess up
	with our string*/
	for (a = 0; a < 255; a++) {
		value[a] = 0x0;
	}
	/*Then reset A*/
	a = 0;
	while (read != EOF) {
		read = fgetc(stream);
		if (read == 0x22) {
			while (value[a - 1] != EOF) {
				value[a] = fgetc(stream);
				if (value[a] == 0x22) {
					/*Set the null terminator!*/
					value[a] = 0x0;
					return value;
				}
				a++;
			}
		}
	}
	return 0;
}