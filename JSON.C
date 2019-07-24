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
